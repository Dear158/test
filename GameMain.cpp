
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

// GameMain.cpp
#include"Config.h"
#include"Client.h"
#include "GameMain.h"
#include <graphics.h>
#include <conio.h>
#include <string>
#include <mysql.h>
#include "Login.h"
#include "LeaderBoard.h"
#include "FriendList.h"
#include "SingleMatch.h"
#include <vector>
#include <chrono>
#include <thread>
#include <mutex>
#include <ctime>
//#include"Client.h"
//#include"Config.h"


// 游戏主页面的按钮ID
enum ButtonID {
    SINGLE_MATCH,
    TEAM_MATCH,
    FRIEND_LIST,
    LEADERBOARD,
    LOGOUT,
    CONNECT
};

// 绘制按钮的函数
void DrawButton(int x, int y, int width, int height, const wchar_t* text, int id) {
    setfillcolor(LIGHTGRAY);
    solidrectangle(x, y, x + width, y + height);
    settextcolor(BLACK);
    settextstyle(20, 0, _T("微软雅黑"));
    setbkmode(TRANSPARENT); // 设置文字背景为透明
    outtextxy(x + (width - textwidth(text)) / 2, y + (height - textheight(text)) / 2, text);
}

// 从数据库获取当前用户的 ID
int GetCurrentUserId() {
    MYSQL* conn;
    MYSQL_ROW row;
    MYSQL_RES* res;
    int userId = -1; // 如果查询失败，返回 -1

    conn = mysql_init(0);
    if (mysql_real_connect(conn, "localhost", "root", "root", "GameProject", 3306, NULL, 0)) {
        // 将 std::wstring 转换为 std::string
        std::string username(currentUsername.begin(), currentUsername.end());
        char query[256];
        sprintf_s(query, "SELECT user_id FROM users WHERE username = '%s'", username.c_str());
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

// 从数据库获取当前用户的金豆数量
int GetUserBeans(int userId) {
    MYSQL* conn;
    MYSQL_ROW row;
    MYSQL_RES* res;
    int beans = 0;

    conn = mysql_init(0);
    if (mysql_real_connect(conn, "localhost", "root", "root", "GameProject", 3306, NULL, 0)) {
        char query[256];
        sprintf_s(query, "SELECT beans FROM currency WHERE user_id = %d", userId);
        if (mysql_query(conn, query) == 0) {
            res = mysql_store_result(conn);
            if (res) {
                row = mysql_fetch_row(res);
                if (row && row[0]) {
                    beans = atoi(row[0]);
                }
                mysql_free_result(res);
            }
        }
        mysql_close(conn);
    }
    return beans;
}

// 显示游戏主页面的函数
void ShowGameMainPage() {
    initgraph(800, 600); // 初始化图形窗口
    setbkcolor(WHITE);   // 设置背景颜色
    cleardevice();       // 清屏

    // 获取当前用户的 ID 和金豆数量
    int currentUserId = GetCurrentUserId();
    int userBeans = GetUserBeans(currentUserId);
    // 绘制当前用户信息
    settextstyle(20, 0, _T("微软雅黑"));
    std::wstring currentUserText = L"当前用户：" + currentUsername;
    std::wstring userBeansText = L"金豆数量：" + std::to_wstring(userBeans);
    settextcolor(BLACK);
    outtextxy(10, 10, currentUserText.c_str()); // 在左上角显示当前用户
    outtextxy(10, 40, userBeansText.c_str()); // 在当前用户信息下方显示金豆数量


    // 绘制游戏主页面的各个部分
    settextstyle(40, 0, _T("微软雅黑"));
    outtextxy(300, 50, _T("游戏主界面"));

    // 绘制按钮
    DrawButton(300, 150, 200, 50, _T("单人匹配"), SINGLE_MATCH);
    DrawButton(300, 220, 200, 50, _T("组队开黑"), TEAM_MATCH);
    DrawButton(300, 290, 200, 50, _T("好友列表"), FRIEND_LIST);
    DrawButton(300, 360, 200, 50, _T("排行榜"), LEADERBOARD);
    DrawButton(300, 430, 200, 50, _T("切换账户"), LOGOUT);
    DrawButton(100, 100, 200, 50, _T("连接服务器"), CONNECT);

    // 游戏主循环
    while (true) {
        // 处理用户输入和游戏逻辑
        if (_kbhit()) { // 如果有键盘输入
            char ch = _getch(); // 获取按键
            if (ch == 27) { // 如果按下ESC键
                break; // 退出循环
            }
        }

        // 处理鼠标点击事件
        if (MouseHit()) {
            MOUSEMSG msg = GetMouseMsg();
            if (msg.uMsg == WM_LBUTTONDOWN) {
                // 根据点击位置判断哪个按钮被点击
                int mouseX = msg.x;
                int mouseY = msg.y;
                if (mouseX >= 100 && mouseX <= 300) {
                    if (mouseY >= 100 && mouseY <= 150) {
                        const char* ipAddress =  "127.0.0.1";
                        //int fd = 0;
                        //GameSever(ipAddress, fd);
                       // int currentUserId = GetCurrentUserId();
                       // char buf[MAX_LINE];
                        //memset(buf, 0, MAX_LINE);
                       // MessageBox(NULL,L"fd", L"提示", MB_OK | MB_ICONERROR);
                        //send(fd, buf, sizeof(buf), 0);
                        
                    }
                }                    
                // 判断点击的按钮
                if (mouseX >= 300 && mouseX <= 500) {
                    if (mouseY >= 150 && mouseY <= 200) {
                        // 单人匹配按钮逻辑
                        // 获取当前用户的 ID 和金豆数量
                        int currentUserId = GetCurrentUserId();
                        int userBeans = GetUserBeans(currentUserId);
                        SingleMatchLogic();
                    }
                    else if (mouseY >= 220 && mouseY <= 270) {
                        // 组队开黑按钮逻辑
                    }
                    else if (mouseY >= 290 && mouseY <= 340) {
                        // 好友列表按钮逻辑
                        ShowFriendListPage();
                    }
                    else if (mouseY >= 360 && mouseY <= 410) {
                        // 排行榜按钮逻辑
                        ShowLeaderboardPage(1); // 显示排行榜界面
                    }
                    else if (mouseY >= 430 && mouseY <= 480) {
                        // 切换账户按钮逻辑
                        SwitchAccount(); // 调用切换账户的函数
                        closegraph(); // 关闭当前窗口
                    }
                }
            }
        }
    }

    closegraph(); // 关闭图形窗口
}
