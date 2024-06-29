
///****************************************************************************
/// @author  : Dongyang Wang                                                         
/// @date    : 2024��6��19��                                                                
/// @file    :                                                                 
/// @brief   :                                                                 
///----------------------------------------------------------------------------
///                           Change History                                   
///----------------------------------------------------------------------------
///                              
///****************************************************************************

#include <graphics.h>
#include <conio.h>
#include "EasyTextBox.h"
#include  "Login.h"  
#include <mysql.h> 
#include <stdio.h>

// ����ȫ�ֱ������洢ע����Ϣ
EasyTextBox usernameBox2;
EasyTextBox passwordBox2;
EasyTextBox emailBox;

// ��ʼ������
void initWindow2() {
    initgraph(800, 600);
}

// ���Ʊ���ɫ
void drawBackground2() {
    setbkcolor(WHITE);
    cleardevice();
}

// ���Ʊ���
void drawTitle2(int y) {
    settextstyle(30, 0, _T("΢���ź�"));
    settextcolor(BLACK);
    int titleWidth = textwidth(_T("�û�ע��"));
    outtextxy((800 - titleWidth) / 2, y, _T("�û�ע��"));
}

// ���Ʊ�ǩ
void drawLabel2(int x, int y, const wchar_t* label) {
    settextstyle(20, 0, _T("����"));
    settextcolor(BLACK);
    outtextxy(x, y, label);
}

// ���ư�ť
void drawButton2(int x, int y, const wchar_t* buttonText) {
    setfillcolor(DARKGRAY);
    solidrectangle(x, y, x + 100, y + 30);
    settextstyle(20, 0, _T("΢���ź�"));
    settextcolor(WHITE);
    setbkmode(TRANSPARENT);
    int textWidth = textwidth(buttonText);
    int textHeight = textheight(buttonText);
    outtextxy(x + (100 - textWidth) / 2, y + (30 - textHeight) / 2, buttonText);
}
void registerUser() {
    // ��ȡ�û�����
    const wchar_t* username = usernameBox2.Text();
    const wchar_t* password = passwordBox2.Text();
    const wchar_t* email = emailBox.Text();
    // ����û����������Ƿ�Ϊ��
    if (wcslen(username) == 0 || wcslen(password) == 0) {
        MessageBox(GetHWnd(), _T("�û��������벻��Ϊ�գ�"), _T("����"), MB_OK | MB_ICONEXCLAMATION);
        return; // ����û���������Ϊ�գ��򷵻ز���ִ�к�������
    }
    // TODO: ��֤�������ݵ���Ч��

    // TODO: ��������

    // �������ݿ�
    MYSQL* conn;
    conn = mysql_init(0);
    conn = mysql_real_connect(conn, "localhost", "root", "root", "GameProject", 3306, NULL, 0);

    // ��������Ƿ�ɹ�
    if (conn) {
        // ���� SQL �������
        char query[1024];
        if (wcslen(email) == 0) {
            // �������Ϊ�գ����� SQL ���ʱʹ�� NULL
            sprintf_s(query, sizeof(query), "INSERT INTO users (username, password, email) VALUES ('%ls', '%ls', NULL)", username, password);
        }
        else {
            // ������䲻Ϊ�գ�����������Ϣ
            sprintf_s(query, sizeof(query), "INSERT INTO users (username, password, email) VALUES ('%ls', '%ls', '%ls')", username, password, email);
        }

        // ִ�� SQL ���
        int state = mysql_query(conn, query);

        // ����Ƿ����ɹ�
        if (state == 0) {
            MessageBox(GetHWnd(), _T("ע��ɹ���"), _T("��ʾ"), MB_OK);
            // ��ȡ�²�����û� ID
            int userId = (int)mysql_insert_id(conn);
            // Ϊ���û��� currency ���д�����������¼
            sprintf_s(query, sizeof(query), "INSERT INTO currency (user_id, beans) VALUES (%d, 0)", userId);
            state = mysql_query(conn, query);

            if (state != 0) {
                // ��������𶹼�¼ʧ�ܣ���ʾ������Ϣ
                MessageBox(GetHWnd(), _T("�޷������𶹼�¼��"), _T("����"), MB_OK);
            }
        }
        else {
            // ��ȡ�������ʹ�����Ϣ
            unsigned int errCode = mysql_errno(conn);
            const char* errMsg = mysql_error(conn);

            // ���ݴ�������ṩ����Ĵ�����Ϣ
            if (errCode == 1062) { // MySQL ������� 1062 ��ʾ�ظ�����Ŀ
                MessageBox(GetHWnd(), _T("�û����Ѵ��ڣ�"), _T("����"), MB_OK);
            }
            else {
                // ��������
                wchar_t errBuff[512];
                swprintf(errBuff, sizeof(errBuff) / sizeof(wchar_t), _T("ע��ʧ�ܣ�������룺%u��������Ϣ��%S"), errCode, errMsg);
                MessageBox(GetHWnd(), errBuff, _T("����"), MB_OK);
            }
        }
        // �ر����ݿ�����
        mysql_close(conn);
    }
    else {
        MessageBox(GetHWnd(), _T("�޷����ӵ����ݿ⣡"), _T("����"), MB_OK);
    }
}

// ��ʾע�����ĺ���
void showRegistrationScreen() {
    // ��ʼ������
    initWindow2();

    // ����ע�����
    drawBackground2();

    // ���������ͱ�ǩ��λ��
    int boxWidth = 200, boxHeight = 30;
    int labelWidth = textwidth(_T("���䣺"));
    int totalWidth = labelWidth + 20 + boxWidth;
    int startX = (800 - totalWidth) / 2;
    int startY = (600 - (boxHeight * 3 + 30 * 2 + 30)) / 2;

    // ���Ʊ��⣬λ����΢���������
    drawTitle2(startY - 60);

    // ��������ʾ�û�������������������
    drawLabel2(startX, startY, _T("�û�����"));
    usernameBox2.Create(startX + labelWidth + 20, startY, startX + labelWidth + 20 + boxWidth, startY + boxHeight, 20);

    drawLabel2(startX, startY + boxHeight + 30, _T("���룺"));
    passwordBox2.Create(startX + labelWidth + 20, startY + boxHeight + 30, startX + labelWidth + 20 + boxWidth, startY + boxHeight + 30 + boxHeight, 20);

    drawLabel2(startX, startY + (boxHeight + 30) * 2, _T("���䣺"));
    emailBox.Create(startX + labelWidth + 20, startY + (boxHeight + 30) * 2, startX + labelWidth + 20 + boxWidth, startY + (boxHeight + 30) * 2 + boxHeight, 20);

    // ����������ť���ܿ�ȣ�������ť֮��ľ��룩
    int buttonsTotalWidth = 100 + 20 + 100; // ע�ᰴť��� + ������ť֮��ľ��� + ��¼��ť���
    // ����ע�ᰴť�����Եλ��
    int registerButtonLeft = (800 - buttonsTotalWidth) / 2;
    // ����ע�ᰴť
    drawButton2(registerButtonLeft, startY + (boxHeight + 30) * 3, _T("ע��"));
    // ���Ƶ�¼��ť��λ��ע�ᰴť�Ҳ� 20 �����ص��λ��
    drawButton2(registerButtonLeft + 100 + 20, startY + (boxHeight + 30) * 3, _T("��¼"));

    // ѭ���ȴ��û�����
    while (true) {
        // �������¼�
        if (MouseHit()) {
            MOUSEMSG msg = GetMouseMsg();
            if (msg.uMsg == WM_LBUTTONDOWN) {
                // ����Ƿ�����ע�ᰴť
                if (msg.x >= registerButtonLeft && msg.x <= registerButtonLeft + 100 && msg.y >= startY + (boxHeight + 30) * 3 && msg.y <= startY + (boxHeight + 30) * 3 + 30) {
                    registerUser();
                }
                // ����Ƿ����˵�¼��ť
                else if (msg.x >= registerButtonLeft + 100 + 20 && msg.x <= registerButtonLeft + 100 + 20 + 100 && msg.y >= startY + (boxHeight + 30) * 3 && msg.y <= startY + (boxHeight + 30) * 3 + 30) {
                    resetLoginInfo(); // ����Login.cpp�еĺ��������õ�¼��Ϣ
                    closegraph(); // �رյ�ǰ����
                    showLoginScreen(false); // ��ת����¼����
                    return; // �˳�ע��������ʾ����
                }
                // ����Ƿ������û��������
                else if (usernameBox2.Check(msg.x, msg.y)) {
                    usernameBox2.OnMessage(); // �����û���
                }
                // ����Ƿ��������������
                else if (passwordBox2.Check(msg.x, msg.y)) {
                    passwordBox2.OnMessage(); // ��������
                }
                // ����Ƿ��������������
                else if (emailBox.Check(msg.x, msg.y)) {
                    emailBox.OnMessage(); // ��������
                }
            }
        }
    }

    // �رմ���
    closegraph();
}
