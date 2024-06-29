
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
#include <mysql.h>
#include <stdio.h>
#include <fstream>
#include <string>
#include <codecvt>
#include "EasyTextBox.h"
#include "Register.h"
#include "GameMain.h"
#include <iostream>

// ����ȫ�ֱ������洢�û���������
EasyTextBox usernameBox;
EasyTextBox passwordBox;
std::wstring currentUsername;
bool rememberMeChecked = false; // ��ס���븴ѡ���״̬

// ��ʼ������
void initWindow() {
    initgraph(800, 600);
}

// ���Ʊ���ɫ
void drawBackground() {
    setbkcolor(WHITE);
    cleardevice();
}

// ���Ʊ���
void drawTitle(int y) {
    settextstyle(30, 0, _T("΢���ź�"));
    settextcolor(BLACK);
    int titleWidth = textwidth(_T("�û���¼"));
    outtextxy((800 - titleWidth) / 2, y, _T("�û���¼"));
}

// ���Ʊ�ǩ
void drawLabel(int x, int y, const wchar_t* label) {
    settextstyle(20, 0, _T("����"));
    settextcolor(BLACK);
    outtextxy(x, y, label);
}

// ���Ƶ�¼��ť
void drawLoginButton(int x, int y) {
    setfillcolor(DARKGRAY); // ���ð�ť����Ϊ����ɫ
    solidrectangle(x, y, x + 100, y + 30); // ���ư�ť����

    settextstyle(20, 0, _T("΢���ź�"));
    settextcolor(WHITE);
    setbkmode(TRANSPARENT); // �������ֱ���Ϊ͸��

    // ���������ڰ�ť�еľ���λ��
    int textWidth = textwidth(_T("��¼"));
    int textHeight = textheight(_T("��¼"));
    outtextxy(x + (100 - textWidth) / 2, y + (30 - textHeight) / 2, _T("��¼"));
}

// ����ע�ᰴť
void drawRegisterButton(int x, int y) {
    setfillcolor(DARKGRAY); // ���ð�ť����Ϊ����ɫ
    solidrectangle(x, y, x + 100, y + 30); // ���ư�ť����

    settextstyle(20, 0, _T("΢���ź�"));
    settextcolor(WHITE);
    setbkmode(TRANSPARENT); // �������ֱ���Ϊ͸��

    // ���������ڰ�ť�еľ���λ��
    int textWidth = textwidth(_T("ע��"));
    int textHeight = textheight(_T("ע��"));
    outtextxy(x + (100 - textWidth) / 2, y + (30 - textHeight) / 2, _T("ע��"));
}

// ���ơ���ס���롱��ѡ��
void drawRememberMeCheckbox(int x, int y, bool checked) {
    setfillcolor(checked ? DARKGRAY : WHITE); // ���ѡ�У�����䰵��ɫ����������ɫ
    solidrectangle(x, y, x + 15, y + 15); // ����������

    setlinecolor(BLACK); // ���ñ߿���ɫΪ��ɫ
    rectangle(x, y, x + 15, y + 15); // ���Ʊ߿�

    settextcolor(BLACK); // ����������ɫΪ��ɫ
    outtextxy(x + 20, y - 3, _T("��ס����")); // �������
}

// ����ļ��Ƿ���ڵĺ���
bool fileExists(const std::string& filename) {
    std::ifstream file(filename.c_str());
    return file.good();
}
// ����ƾ�ݵ��ļ�
void saveCredentials(const wchar_t* username, const wchar_t* password) {
    std::ofstream out("credentials.txt");
    std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
    // �����ַ��ַ���ת��Ϊ UTF-8 ������ַ���
    std::string username_utf8 = converter.to_bytes(username);
    std::string password_utf8 = converter.to_bytes(password);

    // д�� UTF-8 ������ַ���
    out << username_utf8 << std::endl;
    out << password_utf8 << std::endl;
    out.close();
}

// ���ļ�����ƾ��
bool loadCredentials(std::wstring& username, std::wstring& password) {
    // ����ļ��Ƿ����
    if (!fileExists("credentials.txt")) {
        return false; // �ļ������ڣ�ֱ�ӷ���false
    }
    std::wifstream in("credentials.txt");
    if (!in) {
        // �����ļ���ʧ�ܵ����
        MessageBox(GetHWnd(), _T("�޷����ļ��Զ�ȡƾ�ݣ�"), _T("����"), MB_OK);
        return false;
    }
    std::wstring line;
    if (std::getline(in, line)) {
        username = line;
        if (std::getline(in, line)) {
            password = line;
            in.close();
            return true;
        }
    }
    in.close();
    return false;
}

// ɾ��ƾ���ļ�
void deleteCredentials() {
    if (fileExists("credentials.txt")) {
        std::remove("credentials.txt");
    }
}

// ���õ�¼��Ϣ
void resetLoginInfo() {
    usernameBox.SetText(L""); // ����û��������
    passwordBox.SetText(L""); // ������������
    rememberMeChecked = false; // ���ü�ס����ѡ��
}

// ��¼��֤����
bool verifyLogin(const wchar_t* username, const wchar_t* password) {
    // �������ݿ�
    MYSQL* conn;
    conn = mysql_init(0);
    conn = mysql_real_connect(conn, "localhost", "root", "root", "GameProject", 3306, NULL, 0);

    if (conn) {
        // ���� SQL ��ѯ���
        char query[1024];
        char username_c[256];
        char password_c[256];
        size_t username_converted;
        size_t password_converted;
        errno_t username_result = wcstombs_s(&username_converted, username_c, sizeof(username_c), username, _TRUNCATE);
        errno_t password_result = wcstombs_s(&password_converted, password_c, sizeof(password_c), password, _TRUNCATE);
        sprintf_s(query, sizeof(query), "SELECT * FROM users WHERE username='%s' AND password='%s'", username_c, password_c);

        // ִ�� SQL ��ѯ
        int state = mysql_query(conn, query);
        if (state == 0) {
            MYSQL_RES* result = mysql_store_result(conn);
            if (result) {
                // ����Ƿ���ƥ��ļ�¼
                MYSQL_ROW row = mysql_fetch_row(result);
                mysql_free_result(result);
                mysql_close(conn);
                return row != nullptr; // �����ƥ��ļ�¼������ true
            }
        }
        mysql_close(conn);
    }
    return false; // ���û��ƥ��ļ�¼������ʧ�ܣ����� false
}

// �����û���Ծ״̬�ĺ���
void UpdateUserActiveStatus(const wchar_t* username, bool isActive) {
    // �������ݿ�
    MYSQL* conn;
    conn = mysql_init(0);
    conn = mysql_real_connect(conn, "localhost", "root", "root", "GameProject", 3306, NULL, 0);

    if (conn) {
        // ���� SQL �������
        char query[1024];
        char username_c[256];
        size_t username_converted;
        errno_t username_result = wcstombs_s(&username_converted, username_c, sizeof(username_c), username, _TRUNCATE);
        sprintf_s(query, sizeof(query), "UPDATE users SET is_active = %d WHERE username='%s'", isActive ? 1 : 0, username_c);

        // ִ�� SQL ����
        int state = mysql_query(conn, query);
        if (state != 0) {
            std::cerr << "Error updating user active status: " << mysql_error(conn) << std::endl;
        }

        mysql_close(conn);
    }
    else {
        std::cerr << "Error connecting to database: " << mysql_error(conn) << std::endl;
    }
}

// ��ʾ��¼����ĺ���
void showLoginScreen(bool loadSavedCredentials) {
    // ��ʼ������
    initWindow();

    // ���Ƶ�¼����
    drawBackground();

    // ���������ͱ�ǩ��λ��
    int boxWidth = 200, boxHeight = 30;
    int labelWidth = textwidth(_T("�û���"));
    int totalWidth = labelWidth + 20 + boxWidth;
    int startX = (800 - totalWidth) / 2;
    int startY = (600 - (boxHeight * 2 + 30 + 30 + 30 + 30)) / 2;

    drawTitle(startY - 60);

    // ��������ʾ�û��������������
    drawLabel(startX - 10, startY, _T("�û�����"));
    usernameBox.Create(startX + labelWidth + 20, startY, startX + labelWidth + 20 + boxWidth, startY + boxHeight, 20);

    drawLabel(startX - 10, startY + boxHeight + 30, _T("���룺"));
    passwordBox.Create(startX + labelWidth + 20, startY + boxHeight + 30, startX + labelWidth + 20 + boxWidth, startY + boxHeight + 30 + boxHeight, 20);

    // �����¼��ť��λ��
    int loginButtonX = startX;
    int loginButtonY = startY + boxHeight * 2 + 60; // ������ֱλ���Ա����ص�
    drawLoginButton(loginButtonX, loginButtonY);

    // ����ע�ᰴť��λ�ã�ȷ�����¼��ť���㹻�ĺ�����
    int registerButtonX = loginButtonX + 100 + 50; // �ڵ�¼��ť�Ҳ�����50���ؼ��
    int registerButtonY = loginButtonY;
    drawRegisterButton(registerButtonX, registerButtonY);

    // ���ơ���ס���롱��ѡ��ȷ�����¼��ť���㹻�ĺ�����
    int checkboxX = registerButtonX + 100 + 20; // ��ע�ᰴť�Ҳ�����20���ؼ��
    int checkboxY = loginButtonY;
    drawRememberMeCheckbox(checkboxX, checkboxY, rememberMeChecked);

    // ���ݴ���Ĳ��������Ƿ����ƾ��
    if (loadSavedCredentials) {
        std::wstring savedUsername;
        std::wstring savedPassword;
        if (loadCredentials(savedUsername, savedPassword)) {
            usernameBox.SetText(savedUsername.c_str());
            passwordBox.SetText(savedPassword.c_str());
            rememberMeChecked = true;
            drawRememberMeCheckbox(checkboxX, checkboxY, rememberMeChecked);
        }
    }

    // ѭ���ȴ��û�����
    while (true) {
        // �������¼�
        if (MouseHit()) {
            MOUSEMSG msg = GetMouseMsg();
            if (msg.uMsg == WM_LBUTTONDOWN) {
                // ����Ƿ������û��������
                if (usernameBox.Check(msg.x, msg.y)) {
                    usernameBox.OnMessage(); // �����û���
                }
                // ����Ƿ��������������
                else if (passwordBox.Check(msg.x, msg.y)) {
                    passwordBox.OnMessage(); // ��������
                }
                // ����Ƿ����˵�¼��ť
                else if (msg.x >= loginButtonX && msg.x <= loginButtonX + 100 && msg.y >= startY + boxHeight * 2 + 30 + 30 && msg.y <= startY + boxHeight * 2 + 30 + 30 + 30) {
                    // ��֤�û���������
                    if (verifyLogin(usernameBox.Text(), passwordBox.Text())) {
                        currentUsername = usernameBox.Text(); // ���µ�ǰ�û���
                        MessageBox(GetHWnd(), _T("��¼�ɹ���"), _T("��ʾ"), MB_OK);
                        // �����û���Ծ״̬
                        UpdateUserActiveStatus(usernameBox.Text(), true);
                        if (rememberMeChecked) {
                            saveCredentials(usernameBox.Text(), passwordBox.Text());
                        }
                        else
                        {
                            deleteCredentials(); // ����û�ѡ�񲻼�ס���룬ɾ��ƾ���ļ�
                        }
                        closegraph(); // �رյ�ǰ����
                        ShowGameMainPage();//��ʾ��Ϸ��ҳ��
                        return; // �˳���¼�������ʾ����
                    }
                    else {
                        MessageBox(GetHWnd(), _T("�û������������"), _T("��ʾ"), MB_OK | MB_ICONEXCLAMATION);                     
                    }
                }
                else if (msg.x >= checkboxX && msg.x <= checkboxX + 15 && msg.y >= checkboxY && msg.y <= checkboxY + 15) {
                    rememberMeChecked = !rememberMeChecked;
                    drawRememberMeCheckbox(checkboxX, checkboxY, rememberMeChecked);
                }
                // ����Ƿ�����ע�ᰴť
                else if (msg.x >= registerButtonX&& msg.x <= registerButtonX + 100 && msg.y >= startY + boxHeight * 2 + 30 + 30 && msg.y <= startY + boxHeight * 2 + 30 + 30 + 30) {
                        closegraph(); // �رյ�ǰ����
                        showRegistrationScreen(); // ��ʾע�����
                        return; // �˳���¼�������ʾ����
                 }
                
            }
        }
    }

    // �رմ���
    closegraph();
}

// �л��˻��ĺ���
void SwitchAccount() {
    // ���ȣ�����ǰ��Ծ�û��� is_active �ֶ�����Ϊ false
    UpdateUserActiveStatus(usernameBox.Text(), false);
    // ���õ�¼��Ϣ
    resetLoginInfo();
    // ��ʾ��¼���沢�����û������µ��û���������
    showLoginScreen(false); // �����ر����ƾ��
}

int main()
{
    showLoginScreen(true);
    return 0;
}
