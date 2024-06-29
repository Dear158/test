
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

#include <graphics.h>
#include <conio.h>
#include <string>
#include <mysql.h>
#include "FriendList.h"
#include "GameMain.h"

void DrawButton3(int x, int y, int width, int height, const wchar_t* text, int id) {
    setfillcolor(LIGHTGRAY);
    solidrectangle(x, y, x + width, y + height);
    settextcolor(BLACK);
    settextstyle(20, 0, _T("微软雅黑"));
    setbkmode(TRANSPARENT); // 设置文字背景为透明
    outtextxy(x + (width - textwidth(text)) / 2, y + (height - textheight(text)) / 2, text);
}

int GetUserIdByUsername(const char* username) {
    MYSQL* conn;
    MYSQL_ROW row;
    MYSQL_RES* res;
    int userId = -1; // 如果查询失败或找不到用户，返回 -1

    conn = mysql_init(0);
    if (mysql_real_connect(conn, "localhost", "root", "root", "GameProject", 3306, NULL, 0)) {
        char query[256];
        sprintf_s(query, "SELECT user_id FROM users WHERE username = '%s'", username);
        if (mysql_query(conn, query) == 0) {
            res = mysql_store_result(conn);
            if (res) {
                row = mysql_fetch_row(res);
                if (row && row[0]) {
                    userId = atoi(row[0]);
                }
                mysql_free_result(res);
            }
        }
        mysql_close(conn);
    }
    return userId;
}

void ProcessPendingRequests(int currentUserId) {
    MYSQL* conn;
    conn = mysql_init(0);
    if (mysql_real_connect(conn, "localhost", "root", "root", "GameProject", 3306, NULL, 0)) {
        char updateQuery[1024];
        sprintf_s(updateQuery, "UPDATE friends SET status = 'accepted' WHERE friend_id = %d AND status = 'pending'", currentUserId);
        if (mysql_query(conn, updateQuery) != 0) {
            MessageBox(GetHWnd(), _T("无法更新好友请求状态！"), _T("错误"), MB_OK);
        }
        else {
            // 检查是否有行受到影响
            if (mysql_affected_rows(conn) == 0) {
                MessageBox(GetHWnd(), _T("暂无未处理的好友申请"), _T("消息"), MB_OK);
            }
            else {
                MessageBox(GetHWnd(), _T("所有待处理的好友请求已被接受。"), _T("成功"), MB_OK);
            }
        }
        mysql_close(conn);
    }
}


void ShowFriendListPage() {
    initgraph(800, 600);
    setbkcolor(WHITE);
    cleardevice();

    int currentUserId = GetCurrentUserId();
    MYSQL* conn;
    MYSQL_ROW row;
    MYSQL_RES* res;
    conn = mysql_init(0);
    if (mysql_real_connect(conn, "localhost", "root", "root", "GameProject", 3306, NULL, 0)) {
        char query[1024];
        sprintf_s(query, "SELECT users.username, users.is_active, friends.status FROM users "
            "INNER JOIN friends ON (users.user_id = friends.friend_id OR users.user_id = friends.user_id) "
            "AND users.user_id != %d " // 排除当前用户
            "WHERE (friends.user_id = %d OR friends.friend_id = %d) AND friends.status = 'accepted'", currentUserId, currentUserId, currentUserId); if (mysql_query(conn, query) == 0) {
            res = mysql_store_result(conn);
            if (res) {
                settextcolor(BLACK);
                settextstyle(40, 0, _T("黑体"));
                outtextxy(300, 50, _T("好友列表"));

                settextstyle(24, 0, _T("黑体"));
                int labelY = 100;
                outtextxy(210, labelY, _T("用户名"));
                outtextxy(350, labelY, _T("在线状态"));
                outtextxy(550, labelY, _T("好友状态"));

                int y = 140;
                while ((row = mysql_fetch_row(res))) {
                    std::wstring friendName(row[0], row[0] + strlen(row[0]));
                    std::wstring isActive = (strcmp(row[1], "1") == 0) ? L"在线" : L"离线";
                    std::wstring status = (strcmp(row[2], "pending") == 0) ? L"待处理申请" : L"已接受";

                    settextstyle(20, 0, _T("黑体"));
                    outtextxy(210 + (80 - textwidth(friendName.c_str())) / 2, y, friendName.c_str());
                    outtextxy(350 + (100 - textwidth(isActive.c_str())) / 2, y, isActive.c_str());
                    outtextxy(530 + (150 - textwidth(status.c_str())) / 2, y, status.c_str());
                    y += 30;
                }
                mysql_free_result(res);
            }
        }
        mysql_close(conn);
    }

    int buttonWidth = 100;
    int buttonHeight = 30;
    int buttonSpacing = 20;
    int totalButtonWidth = 2 * buttonWidth + buttonSpacing;
    int firstButtonX = (800 - totalButtonWidth) / 2;
    int buttonY = 550;

    DrawButton3(firstButtonX, buttonY, buttonWidth, buttonHeight, _T("添加"), -1);
    DrawButton3(firstButtonX + buttonWidth + buttonSpacing, buttonY, buttonWidth, buttonHeight, _T("返回"), -1);
    // 添加处理申请按钮
    int processButtonX = 550; // 处理申请按钮的x坐标
    int processButtonY = 50; // 处理申请按钮的y坐标
    DrawButton3(processButtonX, processButtonY, buttonWidth, buttonHeight, _T("处理申请"), -2);

    while (true) {
        if (MouseHit()) {
            MOUSEMSG msg = GetMouseMsg();
            if (msg.uMsg == WM_LBUTTONDOWN) {
                // 检查是否点击了处理申请按钮
                if (msg.x >= processButtonX && msg.x <= processButtonX + buttonWidth && msg.y >= processButtonY && msg.y <= processButtonY + buttonHeight) {
                    ProcessPendingRequests(currentUserId);
                    ShowFriendListPage(); // 刷新好友列表页面
                    break;
                }
                else if (msg.x >= firstButtonX && msg.x <= firstButtonX + buttonWidth && msg.y >= buttonY && msg.y <= buttonY + buttonHeight) {
                    wchar_t friendUsernameW[100];
                    char friendUsername[100];
                    size_t convertedChars;

                    InputBox(friendUsernameW, 100, L"请输入要添加的用户名:");
                    wcstombs_s(&convertedChars, friendUsername, sizeof(friendUsername), friendUsernameW, _TRUNCATE);

                    conn = mysql_init(0);
                    if (mysql_real_connect(conn, "localhost", "root", "root", "GameProject", 3306, NULL, 0)) {
                        int friendId = GetUserIdByUsername(friendUsername);
                        if (friendId != -1) {
                            // 检查是否已经是好友
                            char checkQuery[1024];
                            sprintf_s(checkQuery, "SELECT * FROM friends WHERE (user_id = %d AND friend_id = %d) OR (user_id = %d AND friend_id = %d)", currentUserId, friendId, friendId, currentUserId);
                            if (mysql_query(conn, checkQuery) == 0) {
                                MYSQL_RES* res = mysql_store_result(conn);
                                if (res && mysql_num_rows(res) > 0) { // 如果找到记录，说明已经是好友
                                    MessageBox(GetHWnd(), _T("该用户已经是您的好友！"), _T("提示"), MB_OK);
                                }
                                else {
                                    // 插入新的好友请求
                                    char insertQuery[1024];
                                    sprintf_s(insertQuery, "INSERT INTO friends (user_id, friend_id, status) VALUES (%d, %d, 'pending')", currentUserId, friendId);
                                    if (mysql_query(conn, insertQuery) != 0) {
                                        MessageBox(GetHWnd(), _T("申请失败！"), _T("错误"), MB_OK);
                                    }
                                }
                                if (res) {
                                    mysql_free_result(res);
                                }
                            }
                        }
                        mysql_close(conn);
                    }
                }
                else if (msg.x >= firstButtonX + buttonWidth + buttonSpacing && msg.x <= firstButtonX + totalButtonWidth && msg.y >= buttonY && msg.y <= buttonY + buttonHeight) {
                    closegraph();
                    ShowGameMainPage();
                    break;
                }
            }
        }
    }

    int ch = _getch();
    closegraph();
}
