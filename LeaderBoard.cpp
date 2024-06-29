
///****************************************************************************
/// @author  : Dongyang Wang                                                         
/// @date    : 2024年6月20日                                                           
/// @file    :                                                                 
/// @brief   :                                                                 
///----------------------------------------------------------------------------
///                           Change History                                   
///----------------------------------------------------------------------------
///                              
///****************************************************************************

#include "LeaderBoard.h"
#include <graphics.h>
#include <conio.h>
#include <string>
#include <mysql.h>
#include "Login.h" 
#include "GameMain.h"
#include <vector>

// 绘制按钮的函数
void DrawButton2(int x, int y, int width, int height, const wchar_t* text, int id) {
    setfillcolor(LIGHTGRAY);
    solidrectangle(x, y, x + width, y + height);
    settextcolor(BLACK);
    settextstyle(20, 0, _T("微软雅黑"));
    setbkmode(TRANSPARENT); // 设置文字背景为透明
    outtextxy(x + (width - textwidth(text)) / 2, y + (height - textheight(text)) / 2, text);
}

// 从数据库获取排行榜数据的函数
std::vector<std::pair<std::wstring, int>> GetLeaderboardData() {
    MYSQL* conn;
    MYSQL_ROW row;
    MYSQL_RES* res;
    std::vector<std::pair<std::wstring, int>> leaderboard;

    conn = mysql_init(0);
    if (mysql_real_connect(conn, "localhost", "root", "root", "GameProject", 3306, NULL, 0)) {
        const char* query = "SELECT username, beans FROM users INNER JOIN currency ON users.user_id = currency.user_id ORDER BY beans DESC LIMIT 100";
        if (mysql_query(conn, query) == 0) {
            res = mysql_store_result(conn);
            while ((row = mysql_fetch_row(res))) {
                std::wstring username(row[0], row[0] + strlen(row[0]));
                int beans = atoi(row[1]);
                leaderboard.push_back(std::make_pair(username, beans));
            }
            mysql_free_result(res);
        }
        mysql_close(conn);
    }
    return leaderboard;
}

// 显示排行榜界面的函数
void ShowLeaderboardPage(int page) {
    // 初始化图形窗口
    initgraph(800, 600);
    setbkcolor(WHITE);
    cleardevice();

    // 获取排行榜数据
    std::vector<std::pair<std::wstring, int>> leaderboard = GetLeaderboardData();

    // 绘制排行榜标题
    settextcolor(BLACK);
    settextstyle(40, 0, _T("微软雅黑"));
    outtextxy(300, 50, _T("排行榜 TOP 100"));

    // 绘制标签 "排名  用户名 金豆数量"
    settextstyle(24, 0, _T("黑体"));
    int labelY = 100; // 标签的 y 坐标
    outtextxy(210, labelY, _T("排名"));
    outtextxy(390, labelY, _T("用户名"));
    outtextxy(530, labelY, _T("金豆数量"));

    // 计算显示的起始和结束排名
    size_t startRank = (static_cast<size_t>(page) - 1) * 10;
    size_t endRank = startRank + 10;
    endRank = (endRank > leaderboard.size()) ? leaderboard.size() : endRank;

    // 绘制排行榜数据
    settextstyle(20, 0, _T("黑体"));
    int y = 140; // 数据的初始 y 坐标
    for (size_t i = startRank; i < endRank; ++i) {
        // 绘制排名
        std::wstring rankText = std::to_wstring(i + 1);
        outtextxy(200 + (50 - textwidth(rankText.c_str())) / 2, y, rankText.c_str());

        // 绘制用户名
        std::wstring nameText = leaderboard[i].first;
        outtextxy(350 + (150 - textwidth(nameText.c_str())) / 2, y, nameText.c_str());

        // 绘制金豆数量
        std::wstring beansText = std::to_wstring(leaderboard[i].second);
        outtextxy(500 + (150 - textwidth(beansText.c_str())) / 2, y, beansText.c_str());

        y += 30; // 更新 y 坐标
    }

    // 在页面最低端显示当前登录用户的排名情况
    std::wstring currentUserRank = L"您的排名：未上榜";
    for (size_t i = 0; i < leaderboard.size(); ++i) {
        if (leaderboard[i].first == currentUsername) {
            currentUserRank = L"您的排名：" + std::to_wstring(i + 1);
            break;
        }
    }
    // 居中显示当前用户排名
    int currentUserRankWidth = textwidth(currentUserRank.c_str());
    int currentUserRankX = (800 - currentUserRankWidth) / 2;
    outtextxy(currentUserRankX, 550, currentUserRank.c_str());

    // 显示上一页和下一页按钮
    if (page > 1) {
        DrawButton2(100, 500, 100, 30, _T("上一页"), -1); // 上一页按钮
    }
    if (endRank < leaderboard.size()) {
        DrawButton2(600, 500, 100, 30, _T("下一页"), -1); // 下一页按钮
    }

    // 显示返回按钮，放置于当前登录用户的排名情况的水平方向右方适当距离
    int returnButtonWidth = 100;
    int returnButtonHeight = 30;
    int returnButtonX = currentUserRankX + currentUserRankWidth + 20; // 在当前用户排名信息的右侧20像素处
    int returnButtonY = 550;
    DrawButton2(returnButtonX, returnButtonY, returnButtonWidth, returnButtonHeight, _T("返回"), -1);

    // 处理鼠标点击事件以支持分页和返回按钮
    while (true) {
        if (MouseHit()) {
            MOUSEMSG msg = GetMouseMsg();
            if (msg.uMsg == WM_LBUTTONDOWN) {
                if (msg.x >= 100 && msg.x <= 200 && msg.y >= 500 && msg.y <= 530 && page > 1) {
                    closegraph(); // 关闭当前窗口
                    ShowLeaderboardPage(page - 1); // 显示上一页
                    break;
                }
                else if (msg.x >= 600 && msg.x <= 700 && msg.y >= 500 && msg.y <= 530 && endRank < leaderboard.size()) {
                    closegraph(); // 关闭当前窗口
                    ShowLeaderboardPage(page + 1); // 显示下一页
                    break;
                }
                else if (msg.x >= returnButtonX && msg.x <= returnButtonX + returnButtonWidth && msg.y >= returnButtonY && msg.y <= returnButtonY + returnButtonHeight) {
                    closegraph(); // 关闭当前窗口
                    ShowGameMainPage(); // 返回主页面
                    break;
                }
            }
        }
    }

    // 等待用户按键返回
    int ch = _getch(); // 读取按键并赋值给变量ch
    closegraph(); // 关闭图形窗口
}
