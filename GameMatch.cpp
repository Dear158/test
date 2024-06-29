#include "Config.h"
#include "GameMatch.h"
#include <graphics.h>
#include <string>
#include <mysql.h>
#include <vector>
#include <chrono>
#include <thread>
#include <mutex>
#include <ctime>
#include <conio.h>
#include "CardGraph.h"

// 全局变量用于存储玩家的牌组
Cards3 playerCards[3];
bool isFirstRun = true;  // 标记是否是第一次运行

// 绘制规则按钮
void drawRulesButton(int x, int y) {
    setfillcolor(DARKGRAY); // 设置按钮背景为暗黑色
    solidrectangle(x, y, x + 100, y + 30); // 绘制按钮背景

    settextstyle(20, 0, _T("微软雅黑"));
    settextcolor(WHITE);
    setbkmode(TRANSPARENT); // 设置文字背景为透明

    // 计算文字在按钮中的居中位置
    int textWidth = textwidth(_T("规则"));
    int textHeight = textheight(_T("规则"));
    outtextxy(x + (100 - textWidth) / 2, y + (30 - textHeight) / 2, _T("规则"));
}

void drawWinButton(int x, int y) {
    setfillcolor(DARKGRAY);
    solidrectangle(x, y, x + 100, y + 30);

    settextstyle(20, 0, _T("微软雅黑"));
    settextcolor(WHITE);
    setbkmode(TRANSPARENT);
    int textWidth = textwidth(_T("获胜"));
    int textHeigh = textheight(_T("获胜"));
    outtextxy(x + (100 - textWidth) / 2, y + (30 - textHeigh) / 2, _T("获胜"));

}

// 绘制关闭按钮
void drawCloseButton(int x, int y) {
    setfillcolor(DARKGRAY); // 设置按钮背景为暗黑色
    solidrectangle(x, y, x + 100, y + 30); // 绘制按钮背景

    settextstyle(20, 0, _T("微软雅黑"));
    settextcolor(WHITE);
    setbkmode(TRANSPARENT); // 设置文字背景为透明

    // 计算文字在按钮中的居中位置
    int textWidth = textwidth(_T("关闭"));
    int textHeight = textheight(_T("关闭"));
    outtextxy(x + (100 - textWidth) / 2, y + (30 - textHeight) / 2, _T("关闭"));
}

// 检查是否点击关闭按钮
bool isCloseButtonClicked(int x, int y, int buttonX, int buttonY) {
    return (x >= buttonX && x <= buttonX + 100 && y >= buttonY && y <= buttonY + 30);
}

bool isWinButtonClicked(int x, int y, int buttonX, int buttonY) {
    return (x >= buttonX && x <= buttonX + 100 && y >= buttonY && y <= buttonY + 30);
}


// 显示炸金花规则的弹窗
void showRulesDialog() {
    // 这里可以自定义弹窗大小和位置
    int dialogWidth = 600;
    int dialogHeight = 400;
    int startX = (1280 - dialogWidth) / 2;
    int startY = (720 - dialogHeight) / 2;

    setfillcolor(LIGHTGRAY); // 设置弹窗背景颜色
    solidrectangle(startX, startY, startX + dialogWidth, startY + dialogHeight); // 绘制弹窗背景

    settextstyle(20, 0, _T("微软雅黑"));
    settextcolor(BLACK);
    setbkmode(TRANSPARENT); // 设置文字背景为透明

    std::wstring rules =
        L"炸金花规则：\n"
        L"1. 游戏使用一副52张牌（不含大小王）。\n"
        L"2. 每位玩家发3张牌，比牌型、点数。\n"
        L"3. 牌型从大到小依次为：豹子、顺金、金花、顺子、对子、单牌。\n"
        L"4. 豹子最大，顺金次之，依此类推。\n"
        L"5. 若牌型相同，按点数比大小。\n"
        L"6. 游戏中可以选择看牌、比牌、弃牌等操作。";

    RECT textRect = { startX + 20, startY + 20, startX + dialogWidth - 20, startY + dialogHeight - 70 };
    drawtext(rules.c_str(), &textRect, DT_WORDBREAK); // 自动换行显示文本

    // 绘制关闭按钮
    int closeButtonX = startX + (dialogWidth - 100) / 2;
    int closeButtonY = startY + dialogHeight - 50;
    drawCloseButton(closeButtonX, closeButtonY);
}

// 检查是否点击规则按钮
bool isRulesButtonClicked(int x, int y, int buttonX, int buttonY) {
    return (x >= buttonX && x <= buttonX + 100 && y >= buttonY && y <= buttonY + 30);
}

// 定义玩家信息结构体
struct PlayerInfo {
    int userId;
    std::wstring username;
    int beans;
};

// 从数据库获取匹配成功的玩家信息
std::vector<PlayerInfo> GetMatchPlayersInfo(int matchId) {
    MYSQL* conn;
    MYSQL_ROW row;
    MYSQL_RES* res;
    std::vector<PlayerInfo> players;

    conn = mysql_init(0);
    if (mysql_real_connect(conn, "localhost", "root", "root", "GameProject", 3306, NULL, 0)) {
        char query[256];
        sprintf_s(query, "SELECT users.user_id, users.username, currency.beans FROM user_matches "
            "JOIN users ON user_matches.user_id = users.user_id "
            "JOIN currency ON users.user_id = currency.user_id "
            "WHERE user_matches.match_id = %d", matchId);
        if (mysql_query(conn, query) == 0) {
            res = mysql_store_result(conn);
            if (res) {
                while ((row = mysql_fetch_row(res))) {
                    PlayerInfo player;
                    player.userId = atoi(row[0]);
                    std::string username_str(row[1]);
                    player.username = std::wstring(username_str.begin(), username_str.end());
                    player.beans = atoi(row[2]);
                    players.push_back(player);
                }
                mysql_free_result(res);
            }
        }
        mysql_close(conn);
    }
    return players;
}

// 绘制玩家信息的函数
void DrawPlayerInfo(int x, int y, const PlayerInfo& player, bool isCurrentUser) {
    std::wstring playerText = L"玩家：" + player.username + L" | 金豆：" + std::to_wstring(player.beans);
    settextcolor(isCurrentUser ? BLUE : BLACK);
    outtextxy(x, y, playerText.c_str());
}

void ShowGameMatchPage(int matchId, int currentUserId, int & fd) {
    initgraph(1280, 720);  // 使用1280x720分辨率
    setbkcolor(WHITE);
    cleardevice();

    // 获取匹配成功的玩家信息
    std::vector<PlayerInfo> players = GetMatchPlayersInfo(matchId);

    settextstyle(30, 0, _T("微软雅黑"));
    outtextxy(480, 20, _T("游戏对局界面"));  // 标题居中显示

    // 仅在第一次运行时发牌给玩家
    if (isFirstRun) {
        Cards52 deck = Cards52::GetInstance();
        deck.Shuffle();
        for (int i = 0; i < 3; ++i) {
            playerCards[i] = deck.Deal3();
        }
        isFirstRun = false;
    }

    // 绘制玩家的牌和信息
    if (players.size() >= 3) {
        for (size_t i = 0; i < 3; ++i) {
            int x = 0, y = 0;
            bool isCurrentUser = (players[i].userId == currentUserId);
            if (i == 0) {
                x = 50;
                y = 50;
            }
            else if (i == 1) {
                x = 930;  // 右上角位置
                y = 50;
            }
            else if (i == 2) {
                x = 480;  // 中下位置
                y = 520;
            }

            DrawPlayerInfo(x, y, players[i], isCurrentUser);

            // 绘制玩家的牌
            CardGraph3 cg(playerCards[i]);
            cg.Draw(x, y + 50, isCurrentUser);  // 牌的位置在玩家信息下方
        }
    }

    // 绘制规则按钮
    int rulesButtonX = 1150;
    int rulesButtonY = 20;
    drawRulesButton(rulesButtonX, rulesButtonY);

    bool isRulesDialogShown = false;

    int winButtonX = 500;
    int winButtonY = 50;;
    drawWinButton(winButtonX, winButtonY);

    while (true) {
    Message message;
    char buf[8192];
        if (_kbhit()) {
            char ch = _getch();
            if (ch == 27) {  // ESC键退出
                break;
            }
        }
        if (MouseHit()) {
            MOUSEMSG msg = GetMouseMsg();
            if (msg.uMsg == WM_LBUTTONDOWN) {
                if (isRulesDialogShown) {
                    int closeButtonX = (1280 - 600) / 2 + (600 - 100) / 2;
                    int closeButtonY = (720 - 400) / 2 + 400 - 50;
                    if (isCloseButtonClicked(msg.x, msg.y, closeButtonX, closeButtonY)) {
                        // 关闭规则弹窗
                        isRulesDialogShown = false;
                        cleardevice();
                        ShowGameMatchPage(matchId, currentUserId,fd);
                        break;
                    }
                }
                else {
                    if (isRulesButtonClicked(msg.x, msg.y, rulesButtonX, rulesButtonY)) {
                        showRulesDialog();
                        isRulesDialogShown = true;
                    }
                   else if (isWinButtonClicked(msg.x, msg.y, winButtonX, winButtonY)) {
                        memset(buf, 0, sizeof(buf));
                        memset(&message, 0, sizeof(message));
                        message.msgType = 4;
                        //message.winner = currentUserId;
                        message.turn = currentUserId;
                        //sprintf(buf, "%s", message);
                        memcpy(buf, &message, sizeof(message));
                        //printf("buf = : %s", buf);
                        send(fd, buf, sizeof(buf), 0);
                    }
                }
            }
        }
    }
  

    closegraph();
}

