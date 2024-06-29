
///****************************************************************************
/// @author  : Dongyang Wang                                                         
/// @date    : 2024年6月19日                                                                
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

// 定义全局变量来存储用户名和密码
EasyTextBox usernameBox;
EasyTextBox passwordBox;
std::wstring currentUsername;
bool rememberMeChecked = false; // 记住密码复选框的状态

// 初始化窗口
void initWindow() {
    initgraph(800, 600);
}

// 绘制背景色
void drawBackground() {
    setbkcolor(WHITE);
    cleardevice();
}

// 绘制标题
void drawTitle(int y) {
    settextstyle(30, 0, _T("微软雅黑"));
    settextcolor(BLACK);
    int titleWidth = textwidth(_T("用户登录"));
    outtextxy((800 - titleWidth) / 2, y, _T("用户登录"));
}

// 绘制标签
void drawLabel(int x, int y, const wchar_t* label) {
    settextstyle(20, 0, _T("宋体"));
    settextcolor(BLACK);
    outtextxy(x, y, label);
}

// 绘制登录按钮
void drawLoginButton(int x, int y) {
    setfillcolor(DARKGRAY); // 设置按钮背景为暗黑色
    solidrectangle(x, y, x + 100, y + 30); // 绘制按钮背景

    settextstyle(20, 0, _T("微软雅黑"));
    settextcolor(WHITE);
    setbkmode(TRANSPARENT); // 设置文字背景为透明

    // 计算文字在按钮中的居中位置
    int textWidth = textwidth(_T("登录"));
    int textHeight = textheight(_T("登录"));
    outtextxy(x + (100 - textWidth) / 2, y + (30 - textHeight) / 2, _T("登录"));
}

// 绘制注册按钮
void drawRegisterButton(int x, int y) {
    setfillcolor(DARKGRAY); // 设置按钮背景为暗黑色
    solidrectangle(x, y, x + 100, y + 30); // 绘制按钮背景

    settextstyle(20, 0, _T("微软雅黑"));
    settextcolor(WHITE);
    setbkmode(TRANSPARENT); // 设置文字背景为透明

    // 计算文字在按钮中的居中位置
    int textWidth = textwidth(_T("注册"));
    int textHeight = textheight(_T("注册"));
    outtextxy(x + (100 - textWidth) / 2, y + (30 - textHeight) / 2, _T("注册"));
}

// 绘制“记住密码”复选框
void drawRememberMeCheckbox(int x, int y, bool checked) {
    setfillcolor(checked ? DARKGRAY : WHITE); // 如果选中，则填充暗灰色，否则填充白色
    solidrectangle(x, y, x + 15, y + 15); // 绘制填充矩形

    setlinecolor(BLACK); // 设置边框颜色为黑色
    rectangle(x, y, x + 15, y + 15); // 绘制边框

    settextcolor(BLACK); // 设置文字颜色为黑色
    outtextxy(x + 20, y - 3, _T("记住密码")); // 输出文字
}

// 检查文件是否存在的函数
bool fileExists(const std::string& filename) {
    std::ifstream file(filename.c_str());
    return file.good();
}
// 保存凭据到文件
void saveCredentials(const wchar_t* username, const wchar_t* password) {
    std::ofstream out("credentials.txt");
    std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
    // 将宽字符字符串转换为 UTF-8 编码的字符串
    std::string username_utf8 = converter.to_bytes(username);
    std::string password_utf8 = converter.to_bytes(password);

    // 写入 UTF-8 编码的字符串
    out << username_utf8 << std::endl;
    out << password_utf8 << std::endl;
    out.close();
}

// 从文件加载凭据
bool loadCredentials(std::wstring& username, std::wstring& password) {
    // 检查文件是否存在
    if (!fileExists("credentials.txt")) {
        return false; // 文件不存在，直接返回false
    }
    std::wifstream in("credentials.txt");
    if (!in) {
        // 处理文件打开失败的情况
        MessageBox(GetHWnd(), _T("无法打开文件以读取凭据！"), _T("错误"), MB_OK);
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

// 删除凭据文件
void deleteCredentials() {
    if (fileExists("credentials.txt")) {
        std::remove("credentials.txt");
    }
}

// 重置登录信息
void resetLoginInfo() {
    usernameBox.SetText(L""); // 清空用户名输入框
    passwordBox.SetText(L""); // 清空密码输入框
    rememberMeChecked = false; // 重置记住密码选项
}

// 登录验证函数
bool verifyLogin(const wchar_t* username, const wchar_t* password) {
    // 连接数据库
    MYSQL* conn;
    conn = mysql_init(0);
    conn = mysql_real_connect(conn, "localhost", "root", "root", "GameProject", 3306, NULL, 0);

    if (conn) {
        // 构建 SQL 查询语句
        char query[1024];
        char username_c[256];
        char password_c[256];
        size_t username_converted;
        size_t password_converted;
        errno_t username_result = wcstombs_s(&username_converted, username_c, sizeof(username_c), username, _TRUNCATE);
        errno_t password_result = wcstombs_s(&password_converted, password_c, sizeof(password_c), password, _TRUNCATE);
        sprintf_s(query, sizeof(query), "SELECT * FROM users WHERE username='%s' AND password='%s'", username_c, password_c);

        // 执行 SQL 查询
        int state = mysql_query(conn, query);
        if (state == 0) {
            MYSQL_RES* result = mysql_store_result(conn);
            if (result) {
                // 检查是否有匹配的记录
                MYSQL_ROW row = mysql_fetch_row(result);
                mysql_free_result(result);
                mysql_close(conn);
                return row != nullptr; // 如果有匹配的记录，返回 true
            }
        }
        mysql_close(conn);
    }
    return false; // 如果没有匹配的记录或连接失败，返回 false
}

// 更新用户活跃状态的函数
void UpdateUserActiveStatus(const wchar_t* username, bool isActive) {
    // 连接数据库
    MYSQL* conn;
    conn = mysql_init(0);
    conn = mysql_real_connect(conn, "localhost", "root", "root", "GameProject", 3306, NULL, 0);

    if (conn) {
        // 构建 SQL 更新语句
        char query[1024];
        char username_c[256];
        size_t username_converted;
        errno_t username_result = wcstombs_s(&username_converted, username_c, sizeof(username_c), username, _TRUNCATE);
        sprintf_s(query, sizeof(query), "UPDATE users SET is_active = %d WHERE username='%s'", isActive ? 1 : 0, username_c);

        // 执行 SQL 更新
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

// 显示登录界面的函数
void showLoginScreen(bool loadSavedCredentials) {
    // 初始化窗口
    initWindow();

    // 绘制登录界面
    drawBackground();

    // 计算输入框和标签的位置
    int boxWidth = 200, boxHeight = 30;
    int labelWidth = textwidth(_T("用户名"));
    int totalWidth = labelWidth + 20 + boxWidth;
    int startX = (800 - totalWidth) / 2;
    int startY = (600 - (boxHeight * 2 + 30 + 30 + 30 + 30)) / 2;

    drawTitle(startY - 60);

    // 创建并显示用户名和密码输入框
    drawLabel(startX - 10, startY, _T("用户名："));
    usernameBox.Create(startX + labelWidth + 20, startY, startX + labelWidth + 20 + boxWidth, startY + boxHeight, 20);

    drawLabel(startX - 10, startY + boxHeight + 30, _T("密码："));
    passwordBox.Create(startX + labelWidth + 20, startY + boxHeight + 30, startX + labelWidth + 20 + boxWidth, startY + boxHeight + 30 + boxHeight, 20);

    // 计算登录按钮的位置
    int loginButtonX = startX;
    int loginButtonY = startY + boxHeight * 2 + 60; // 调整垂直位置以避免重叠
    drawLoginButton(loginButtonX, loginButtonY);

    // 计算注册按钮的位置，确保与登录按钮有足够的横向间隔
    int registerButtonX = loginButtonX + 100 + 50; // 在登录按钮右侧增加50像素间隔
    int registerButtonY = loginButtonY;
    drawRegisterButton(registerButtonX, registerButtonY);

    // 绘制“记住密码”复选框，确保与登录按钮有足够的横向间隔
    int checkboxX = registerButtonX + 100 + 20; // 在注册按钮右侧增加20像素间隔
    int checkboxY = loginButtonY;
    drawRememberMeCheckbox(checkboxX, checkboxY, rememberMeChecked);

    // 根据传入的参数决定是否加载凭据
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

    // 循环等待用户输入
    while (true) {
        // 检测鼠标事件
        if (MouseHit()) {
            MOUSEMSG msg = GetMouseMsg();
            if (msg.uMsg == WM_LBUTTONDOWN) {
                // 检测是否点击了用户名输入框
                if (usernameBox.Check(msg.x, msg.y)) {
                    usernameBox.OnMessage(); // 输入用户名
                }
                // 检测是否点击了密码输入框
                else if (passwordBox.Check(msg.x, msg.y)) {
                    passwordBox.OnMessage(); // 输入密码
                }
                // 检测是否点击了登录按钮
                else if (msg.x >= loginButtonX && msg.x <= loginButtonX + 100 && msg.y >= startY + boxHeight * 2 + 30 + 30 && msg.y <= startY + boxHeight * 2 + 30 + 30 + 30) {
                    // 验证用户名和密码
                    if (verifyLogin(usernameBox.Text(), passwordBox.Text())) {
                        currentUsername = usernameBox.Text(); // 更新当前用户名
                        MessageBox(GetHWnd(), _T("登录成功！"), _T("提示"), MB_OK);
                        // 更新用户活跃状态
                        UpdateUserActiveStatus(usernameBox.Text(), true);
                        if (rememberMeChecked) {
                            saveCredentials(usernameBox.Text(), passwordBox.Text());
                        }
                        else
                        {
                            deleteCredentials(); // 如果用户选择不记住密码，删除凭据文件
                        }
                        closegraph(); // 关闭当前窗口
                        ShowGameMainPage();//显示游戏主页面
                        return; // 退出登录界面的显示函数
                    }
                    else {
                        MessageBox(GetHWnd(), _T("用户名或密码错误！"), _T("提示"), MB_OK | MB_ICONEXCLAMATION);                     
                    }
                }
                else if (msg.x >= checkboxX && msg.x <= checkboxX + 15 && msg.y >= checkboxY && msg.y <= checkboxY + 15) {
                    rememberMeChecked = !rememberMeChecked;
                    drawRememberMeCheckbox(checkboxX, checkboxY, rememberMeChecked);
                }
                // 检测是否点击了注册按钮
                else if (msg.x >= registerButtonX&& msg.x <= registerButtonX + 100 && msg.y >= startY + boxHeight * 2 + 30 + 30 && msg.y <= startY + boxHeight * 2 + 30 + 30 + 30) {
                        closegraph(); // 关闭当前窗口
                        showRegistrationScreen(); // 显示注册界面
                        return; // 退出登录界面的显示函数
                 }
                
            }
        }
    }

    // 关闭窗口
    closegraph();
}

// 切换账户的函数
void SwitchAccount() {
    // 首先，将当前活跃用户的 is_active 字段设置为 false
    UpdateUserActiveStatus(usernameBox.Text(), false);
    // 重置登录信息
    resetLoginInfo();
    // 显示登录界面并允许用户输入新的用户名和密码
    showLoginScreen(false); // 不加载保存的凭据
}

int main()
{
    showLoginScreen(true);
    return 0;
}
