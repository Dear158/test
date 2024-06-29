
///****************************************************************************
/// @author  : Dongyang Wang                                                         
/// @date    : 2024��6��20��                                                           
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

// ���ư�ť�ĺ���
void DrawButton2(int x, int y, int width, int height, const wchar_t* text, int id) {
    setfillcolor(LIGHTGRAY);
    solidrectangle(x, y, x + width, y + height);
    settextcolor(BLACK);
    settextstyle(20, 0, _T("΢���ź�"));
    setbkmode(TRANSPARENT); // �������ֱ���Ϊ͸��
    outtextxy(x + (width - textwidth(text)) / 2, y + (height - textheight(text)) / 2, text);
}

// �����ݿ��ȡ���а����ݵĺ���
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

// ��ʾ���а����ĺ���
void ShowLeaderboardPage(int page) {
    // ��ʼ��ͼ�δ���
    initgraph(800, 600);
    setbkcolor(WHITE);
    cleardevice();

    // ��ȡ���а�����
    std::vector<std::pair<std::wstring, int>> leaderboard = GetLeaderboardData();

    // �������а����
    settextcolor(BLACK);
    settextstyle(40, 0, _T("΢���ź�"));
    outtextxy(300, 50, _T("���а� TOP 100"));

    // ���Ʊ�ǩ "����  �û��� ������"
    settextstyle(24, 0, _T("����"));
    int labelY = 100; // ��ǩ�� y ����
    outtextxy(210, labelY, _T("����"));
    outtextxy(390, labelY, _T("�û���"));
    outtextxy(530, labelY, _T("������"));

    // ������ʾ����ʼ�ͽ�������
    size_t startRank = (static_cast<size_t>(page) - 1) * 10;
    size_t endRank = startRank + 10;
    endRank = (endRank > leaderboard.size()) ? leaderboard.size() : endRank;

    // �������а�����
    settextstyle(20, 0, _T("����"));
    int y = 140; // ���ݵĳ�ʼ y ����
    for (size_t i = startRank; i < endRank; ++i) {
        // ��������
        std::wstring rankText = std::to_wstring(i + 1);
        outtextxy(200 + (50 - textwidth(rankText.c_str())) / 2, y, rankText.c_str());

        // �����û���
        std::wstring nameText = leaderboard[i].first;
        outtextxy(350 + (150 - textwidth(nameText.c_str())) / 2, y, nameText.c_str());

        // ���ƽ�����
        std::wstring beansText = std::to_wstring(leaderboard[i].second);
        outtextxy(500 + (150 - textwidth(beansText.c_str())) / 2, y, beansText.c_str());

        y += 30; // ���� y ����
    }

    // ��ҳ����Ͷ���ʾ��ǰ��¼�û����������
    std::wstring currentUserRank = L"����������δ�ϰ�";
    for (size_t i = 0; i < leaderboard.size(); ++i) {
        if (leaderboard[i].first == currentUsername) {
            currentUserRank = L"����������" + std::to_wstring(i + 1);
            break;
        }
    }
    // ������ʾ��ǰ�û�����
    int currentUserRankWidth = textwidth(currentUserRank.c_str());
    int currentUserRankX = (800 - currentUserRankWidth) / 2;
    outtextxy(currentUserRankX, 550, currentUserRank.c_str());

    // ��ʾ��һҳ����һҳ��ť
    if (page > 1) {
        DrawButton2(100, 500, 100, 30, _T("��һҳ"), -1); // ��һҳ��ť
    }
    if (endRank < leaderboard.size()) {
        DrawButton2(600, 500, 100, 30, _T("��һҳ"), -1); // ��һҳ��ť
    }

    // ��ʾ���ذ�ť�������ڵ�ǰ��¼�û������������ˮƽ�����ҷ��ʵ�����
    int returnButtonWidth = 100;
    int returnButtonHeight = 30;
    int returnButtonX = currentUserRankX + currentUserRankWidth + 20; // �ڵ�ǰ�û�������Ϣ���Ҳ�20���ش�
    int returnButtonY = 550;
    DrawButton2(returnButtonX, returnButtonY, returnButtonWidth, returnButtonHeight, _T("����"), -1);

    // ����������¼���֧�ַ�ҳ�ͷ��ذ�ť
    while (true) {
        if (MouseHit()) {
            MOUSEMSG msg = GetMouseMsg();
            if (msg.uMsg == WM_LBUTTONDOWN) {
                if (msg.x >= 100 && msg.x <= 200 && msg.y >= 500 && msg.y <= 530 && page > 1) {
                    closegraph(); // �رյ�ǰ����
                    ShowLeaderboardPage(page - 1); // ��ʾ��һҳ
                    break;
                }
                else if (msg.x >= 600 && msg.x <= 700 && msg.y >= 500 && msg.y <= 530 && endRank < leaderboard.size()) {
                    closegraph(); // �رյ�ǰ����
                    ShowLeaderboardPage(page + 1); // ��ʾ��һҳ
                    break;
                }
                else if (msg.x >= returnButtonX && msg.x <= returnButtonX + returnButtonWidth && msg.y >= returnButtonY && msg.y <= returnButtonY + returnButtonHeight) {
                    closegraph(); // �رյ�ǰ����
                    ShowGameMainPage(); // ������ҳ��
                    break;
                }
            }
        }
    }

    // �ȴ��û���������
    int ch = _getch(); // ��ȡ��������ֵ������ch
    closegraph(); // �ر�ͼ�δ���
}
