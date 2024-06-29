
///****************************************************************************
/// @author  : Dongyang Wang                                                         
/// @date    : 2024年6月24日                                                                
/// @file    :                                                                 
/// @brief   :                                                                 
///----------------------------------------------------------------------------
///                           Change History                                   
///----------------------------------------------------------------------------
///                              
///****************************************************************************
#include "Config.h"
#include "Client.h"
#include <mysql.h>
#include <chrono>
#include <thread>
#include <mutex>
#include <string>
#include <vector>
#include <ctime>
#include "GameMain.h"
#include "SingleMatch.h"
#include "GameMatch.h"


std::mutex mtx;

// 通知用户匹配成功
void NotifyUserMatchSuccess(int matchId, int userId, bool isHost) {
    std::wstring message = isHost ? L"匹配成功！（房主）" : L"匹配成功！（用户）";
    message += L"\n用户ID: " + std::to_wstring(userId);
    MessageBox(NULL, message.c_str(), L"提示", MB_OK | MB_ICONINFORMATION);
    int fd;
    GameSever("127.0.0.1", fd, matchId, userId);
    //ShowGameMatchPage(matchId, userId);
}

// 单人匹配逻辑
void SingleMatchLogic() {
    MYSQL* conn;
    MYSQL_ROW row;
    MYSQL_RES* res;

    // 初始化MySQL连接
    conn = mysql_init(0);
    if (mysql_real_connect(conn, "localhost", "root", "root", "GameProject", 3306, NULL, 0)) {
        char query[512];
        sprintf_s(query, "SELECT match_id, num, start_time FROM matches WHERE status = 'waiting' AND num < 3");
        if (mysql_query(conn, query) == 0) {
            res = mysql_store_result(conn);
            if (res) {
                // 如果没有等待中的对局，则创建新对局
                if (mysql_num_rows(res) == 0) {
                    char insertMatchQuery[256];
                    time_t now;
                    time(&now);

                    struct tm ltm;
                    localtime_s(&ltm, &now);

                    // 格式化当前时间
                    char formattedTime[20];
                    sprintf_s(formattedTime, 20, "%d-%02d-%02d %02d:%02d:%02d",
                        ltm.tm_year + 1900, ltm.tm_mon + 1, ltm.tm_mday,
                        ltm.tm_hour, ltm.tm_min, ltm.tm_sec);

                    // 插入新对局到数据库
                    sprintf_s(insertMatchQuery, 256, "INSERT INTO matches (start_time, status) VALUES ('%s', 'waiting')", formattedTime);

                    if (mysql_query(conn, insertMatchQuery) == 0) {
                        int newMatchId = (int)mysql_insert_id(conn);
                        int currentUserId = GetCurrentUserId();
                        char insertUserMatchQuery[256];
                        sprintf_s(insertUserMatchQuery, "INSERT INTO user_matches (user_id, match_id) VALUES (%d, %d)", currentUserId, newMatchId);
                        if (mysql_query(conn, insertUserMatchQuery) == 0) {
                            bool matched = false;
                            auto startTime = std::chrono::system_clock::now();
                            // 检查5秒内是否匹配成功
                            while (std::chrono::duration<double>(std::chrono::system_clock::now() - startTime).count() < 5) {
                                std::this_thread::sleep_for(std::chrono::seconds(1));
                                sprintf_s(query, "SELECT COUNT(*) FROM user_matches WHERE match_id = %d", newMatchId);
                                if (mysql_query(conn, query) == 0) {
                                    MYSQL_RES* checkRes = mysql_store_result(conn);
                                    if (checkRes) {
                                        row = mysql_fetch_row(checkRes);
                                        int count = atoi(row[0]);
                                        mysql_free_result(checkRes);
                                        if (count == 3) {
                                            matched = true;
                                            break;
                                        }
                                    }
                                }
                            }
                            // 如果匹配失败，删除相关记录
                            if (!matched) {
                                MessageBox(NULL, L"匹配失败，请重试！", L"提示", MB_OK | MB_ICONERROR);
                                mtx.lock();
                                sprintf_s(query, "DELETE FROM user_matches WHERE match_id = %d", newMatchId);
                                mysql_query(conn, query);
                                sprintf_s(query, "UPDATE matches SET num = num - 1 WHERE match_id = %d", newMatchId);
                                mysql_query(conn, query);
                                sprintf_s(query, "DELETE FROM matches WHERE match_id = %d", newMatchId);
                                mysql_query(conn, query);
                                mtx.unlock();
                            }
                            // 如果匹配成功，通知当前用户
                            else {
                                NotifyUserMatchSuccess(newMatchId, currentUserId, true);
                            }
                        }
                    }
                }
                // 如果已有等待中的对局，加入该对局
                else {
                    row = mysql_fetch_row(res);
                    int matchId = atoi(row[0]);
                    int currentNum = atoi(row[1]);
                    auto matchStartTime = row[2];
                    int currentUserId = GetCurrentUserId();
                    char insertQuery[256];
                    sprintf_s(insertQuery, "INSERT INTO user_matches (user_id, match_id) VALUES (%d, %d)", currentUserId, matchId);
                    if (mysql_query(conn, insertQuery) == 0) {
                        char updateQuery[256];
                        sprintf_s(updateQuery, "UPDATE matches SET num = %d WHERE match_id = %d", currentNum + 1, matchId);
                        if (mysql_query(conn, updateQuery) == 0) {
                            // 如果匹配人数未达到3人，等待匹配人数达到3人
                            if (currentNum + 1 < 3) {
                                bool matched = false;
                                auto startTime = std::chrono::system_clock::now();
                                while (std::chrono::duration<double>(std::chrono::system_clock::now() - startTime).count() < 5) {
                                    std::this_thread::sleep_for(std::chrono::seconds(1));
                                    sprintf_s(query, "SELECT COUNT(*) FROM user_matches WHERE match_id = %d", matchId);
                                    if (mysql_query(conn, query) == 0) {
                                        MYSQL_RES* checkRes = mysql_store_result(conn);
                                        if (checkRes) {
                                            row = mysql_fetch_row(checkRes);
                                            int count = atoi(row[0]);
                                            mysql_free_result(checkRes);
                                            if (count == 3) {
                                                matched = true;
                                                break;
                                            }
                                        }
                                    }
                                }
                                // 如果匹配人数达到3人，更新对局状态并通知用户
                                if (matched) {
                                    char updateStatusQuery[256];
                                    sprintf_s(updateStatusQuery, "UPDATE matches SET status = 'ongoing' WHERE match_id = %d", matchId);
                                    if (mysql_query(conn, updateStatusQuery) == 0) {
                                        NotifyUserMatchSuccess(matchId, currentUserId, false);
                                    }
                                }
                                else {
                                    // 超时未匹配到3人，删除相关记录
                                    MessageBox(NULL, L"匹配失败，请重试！", L"提示", MB_OK | MB_ICONERROR);
                                    mtx.lock();
                                    sprintf_s(query, "DELETE FROM user_matches WHERE match_id = %d", matchId);
                                    mysql_query(conn, query);
                                    sprintf_s(query, "UPDATE matches SET num = num - 1 WHERE match_id = %d", matchId);
                                    mysql_query(conn, query);
                                    mtx.unlock();
                                }
                            }
                            // 如果匹配人数达到3人，更新对局状态并通知用户
                            else {
                                char updateStatusQuery[256];
                                sprintf_s(updateStatusQuery, "UPDATE matches SET status = 'ongoing' WHERE match_id = %d", matchId);
                                if (mysql_query(conn, updateStatusQuery) == 0) {
                                    NotifyUserMatchSuccess(matchId, currentUserId, false);
                                }
                            }
                        }
                    }
                }

                mysql_free_result(res);
            }
        }
        mysql_close(conn);
    }
}
