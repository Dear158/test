
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
    settextstyle(20, 0, _T("΢���ź�"));
    setbkmode(TRANSPARENT); // �������ֱ���Ϊ͸��
    outtextxy(x + (width - textwidth(text)) / 2, y + (height - textheight(text)) / 2, text);
}

int GetUserIdByUsername(const char* username) {
    MYSQL* conn;
    MYSQL_ROW row;
    MYSQL_RES* res;
    int userId = -1; // �����ѯʧ�ܻ��Ҳ����û������� -1

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
            MessageBox(GetHWnd(), _T("�޷����º�������״̬��"), _T("����"), MB_OK);
        }
        else {
            // ����Ƿ������ܵ�Ӱ��
            if (mysql_affected_rows(conn) == 0) {
                MessageBox(GetHWnd(), _T("����δ����ĺ�������"), _T("��Ϣ"), MB_OK);
            }
            else {
                MessageBox(GetHWnd(), _T("���д�����ĺ��������ѱ����ܡ�"), _T("�ɹ�"), MB_OK);
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
            "AND users.user_id != %d " // �ų���ǰ�û�
            "WHERE (friends.user_id = %d OR friends.friend_id = %d) AND friends.status = 'accepted'", currentUserId, currentUserId, currentUserId); if (mysql_query(conn, query) == 0) {
            res = mysql_store_result(conn);
            if (res) {
                settextcolor(BLACK);
                settextstyle(40, 0, _T("����"));
                outtextxy(300, 50, _T("�����б�"));

                settextstyle(24, 0, _T("����"));
                int labelY = 100;
                outtextxy(210, labelY, _T("�û���"));
                outtextxy(350, labelY, _T("����״̬"));
                outtextxy(550, labelY, _T("����״̬"));

                int y = 140;
                while ((row = mysql_fetch_row(res))) {
                    std::wstring friendName(row[0], row[0] + strlen(row[0]));
                    std::wstring isActive = (strcmp(row[1], "1") == 0) ? L"����" : L"����";
                    std::wstring status = (strcmp(row[2], "pending") == 0) ? L"����������" : L"�ѽ���";

                    settextstyle(20, 0, _T("����"));
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

    DrawButton3(firstButtonX, buttonY, buttonWidth, buttonHeight, _T("���"), -1);
    DrawButton3(firstButtonX + buttonWidth + buttonSpacing, buttonY, buttonWidth, buttonHeight, _T("����"), -1);
    // ��Ӵ������밴ť
    int processButtonX = 550; // �������밴ť��x����
    int processButtonY = 50; // �������밴ť��y����
    DrawButton3(processButtonX, processButtonY, buttonWidth, buttonHeight, _T("��������"), -2);

    while (true) {
        if (MouseHit()) {
            MOUSEMSG msg = GetMouseMsg();
            if (msg.uMsg == WM_LBUTTONDOWN) {
                // ����Ƿ����˴������밴ť
                if (msg.x >= processButtonX && msg.x <= processButtonX + buttonWidth && msg.y >= processButtonY && msg.y <= processButtonY + buttonHeight) {
                    ProcessPendingRequests(currentUserId);
                    ShowFriendListPage(); // ˢ�º����б�ҳ��
                    break;
                }
                else if (msg.x >= firstButtonX && msg.x <= firstButtonX + buttonWidth && msg.y >= buttonY && msg.y <= buttonY + buttonHeight) {
                    wchar_t friendUsernameW[100];
                    char friendUsername[100];
                    size_t convertedChars;

                    InputBox(friendUsernameW, 100, L"������Ҫ��ӵ��û���:");
                    wcstombs_s(&convertedChars, friendUsername, sizeof(friendUsername), friendUsernameW, _TRUNCATE);

                    conn = mysql_init(0);
                    if (mysql_real_connect(conn, "localhost", "root", "root", "GameProject", 3306, NULL, 0)) {
                        int friendId = GetUserIdByUsername(friendUsername);
                        if (friendId != -1) {
                            // ����Ƿ��Ѿ��Ǻ���
                            char checkQuery[1024];
                            sprintf_s(checkQuery, "SELECT * FROM friends WHERE (user_id = %d AND friend_id = %d) OR (user_id = %d AND friend_id = %d)", currentUserId, friendId, friendId, currentUserId);
                            if (mysql_query(conn, checkQuery) == 0) {
                                MYSQL_RES* res = mysql_store_result(conn);
                                if (res && mysql_num_rows(res) > 0) { // ����ҵ���¼��˵���Ѿ��Ǻ���
                                    MessageBox(GetHWnd(), _T("���û��Ѿ������ĺ��ѣ�"), _T("��ʾ"), MB_OK);
                                }
                                else {
                                    // �����µĺ�������
                                    char insertQuery[1024];
                                    sprintf_s(insertQuery, "INSERT INTO friends (user_id, friend_id, status) VALUES (%d, %d, 'pending')", currentUserId, friendId);
                                    if (mysql_query(conn, insertQuery) != 0) {
                                        MessageBox(GetHWnd(), _T("����ʧ�ܣ�"), _T("����"), MB_OK);
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
