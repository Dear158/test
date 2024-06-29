
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
#include "EasyTextBox.h"
#include  "Login.h"  
#include <mysql.h> 
#include <stdio.h>

// 定义全局变量来存储注册信息
EasyTextBox usernameBox2;
EasyTextBox passwordBox2;
EasyTextBox emailBox;

// 初始化窗口
void initWindow2() {
    initgraph(800, 600);
}

// 绘制背景色
void drawBackground2() {
    setbkcolor(WHITE);
    cleardevice();
}

// 绘制标题
void drawTitle2(int y) {
    settextstyle(30, 0, _T("微软雅黑"));
    settextcolor(BLACK);
    int titleWidth = textwidth(_T("用户注册"));
    outtextxy((800 - titleWidth) / 2, y, _T("用户注册"));
}

// 绘制标签
void drawLabel2(int x, int y, const wchar_t* label) {
    settextstyle(20, 0, _T("宋体"));
    settextcolor(BLACK);
    outtextxy(x, y, label);
}

// 绘制按钮
void drawButton2(int x, int y, const wchar_t* buttonText) {
    setfillcolor(DARKGRAY);
    solidrectangle(x, y, x + 100, y + 30);
    settextstyle(20, 0, _T("微软雅黑"));
    settextcolor(WHITE);
    setbkmode(TRANSPARENT);
    int textWidth = textwidth(buttonText);
    int textHeight = textheight(buttonText);
    outtextxy(x + (100 - textWidth) / 2, y + (30 - textHeight) / 2, buttonText);
}
void registerUser() {
    // 获取用户输入
    const wchar_t* username = usernameBox2.Text();
    const wchar_t* password = passwordBox2.Text();
    const wchar_t* email = emailBox.Text();
    // 检查用户名和密码是否为空
    if (wcslen(username) == 0 || wcslen(password) == 0) {
        MessageBox(GetHWnd(), _T("用户名和密码不能为空！"), _T("错误"), MB_OK | MB_ICONEXCLAMATION);
        return; // 如果用户名或密码为空，则返回并不执行后续代码
    }
    // TODO: 验证输入数据的有效性

    // TODO: 加密密码

    // 连接数据库
    MYSQL* conn;
    conn = mysql_init(0);
    conn = mysql_real_connect(conn, "localhost", "root", "root", "GameProject", 3306, NULL, 0);

    // 检查连接是否成功
    if (conn) {
        // 构建 SQL 插入语句
        char query[1024];
        if (wcslen(email) == 0) {
            // 如果邮箱为空，构建 SQL 语句时使用 NULL
            sprintf_s(query, sizeof(query), "INSERT INTO users (username, password, email) VALUES ('%ls', '%ls', NULL)", username, password);
        }
        else {
            // 如果邮箱不为空，包含邮箱信息
            sprintf_s(query, sizeof(query), "INSERT INTO users (username, password, email) VALUES ('%ls', '%ls', '%ls')", username, password, email);
        }

        // 执行 SQL 语句
        int state = mysql_query(conn, query);

        // 检查是否插入成功
        if (state == 0) {
            MessageBox(GetHWnd(), _T("注册成功！"), _T("提示"), MB_OK);
            // 获取新插入的用户 ID
            int userId = (int)mysql_insert_id(conn);
            // 为新用户在 currency 表中创建金豆数量记录
            sprintf_s(query, sizeof(query), "INSERT INTO currency (user_id, beans) VALUES (%d, 0)", userId);
            state = mysql_query(conn, query);

            if (state != 0) {
                // 如果创建金豆记录失败，显示错误消息
                MessageBox(GetHWnd(), _T("无法创建金豆记录！"), _T("错误"), MB_OK);
            }
        }
        else {
            // 获取错误代码和错误消息
            unsigned int errCode = mysql_errno(conn);
            const char* errMsg = mysql_error(conn);

            // 根据错误代码提供具体的错误信息
            if (errCode == 1062) { // MySQL 错误代码 1062 表示重复的条目
                MessageBox(GetHWnd(), _T("用户名已存在！"), _T("错误"), MB_OK);
            }
            else {
                // 其他错误
                wchar_t errBuff[512];
                swprintf(errBuff, sizeof(errBuff) / sizeof(wchar_t), _T("注册失败！错误代码：%u，错误信息：%S"), errCode, errMsg);
                MessageBox(GetHWnd(), errBuff, _T("错误"), MB_OK);
            }
        }
        // 关闭数据库连接
        mysql_close(conn);
    }
    else {
        MessageBox(GetHWnd(), _T("无法连接到数据库！"), _T("错误"), MB_OK);
    }
}

// 显示注册界面的函数
void showRegistrationScreen() {
    // 初始化窗口
    initWindow2();

    // 绘制注册界面
    drawBackground2();

    // 计算输入框和标签的位置
    int boxWidth = 200, boxHeight = 30;
    int labelWidth = textwidth(_T("邮箱："));
    int totalWidth = labelWidth + 20 + boxWidth;
    int startX = (800 - totalWidth) / 2;
    int startY = (600 - (boxHeight * 3 + 30 * 2 + 30)) / 2;

    // 绘制标题，位置稍微靠近输入框
    drawTitle2(startY - 60);

    // 创建并显示用户名、密码和邮箱输入框
    drawLabel2(startX, startY, _T("用户名："));
    usernameBox2.Create(startX + labelWidth + 20, startY, startX + labelWidth + 20 + boxWidth, startY + boxHeight, 20);

    drawLabel2(startX, startY + boxHeight + 30, _T("密码："));
    passwordBox2.Create(startX + labelWidth + 20, startY + boxHeight + 30, startX + labelWidth + 20 + boxWidth, startY + boxHeight + 30 + boxHeight, 20);

    drawLabel2(startX, startY + (boxHeight + 30) * 2, _T("邮箱："));
    emailBox.Create(startX + labelWidth + 20, startY + (boxHeight + 30) * 2, startX + labelWidth + 20 + boxWidth, startY + (boxHeight + 30) * 2 + boxHeight, 20);

    // 计算两个按钮的总宽度（包括按钮之间的距离）
    int buttonsTotalWidth = 100 + 20 + 100; // 注册按钮宽度 + 两个按钮之间的距离 + 登录按钮宽度
    // 计算注册按钮的左边缘位置
    int registerButtonLeft = (800 - buttonsTotalWidth) / 2;
    // 绘制注册按钮
    drawButton2(registerButtonLeft, startY + (boxHeight + 30) * 3, _T("注册"));
    // 绘制登录按钮，位于注册按钮右侧 20 个像素点的位置
    drawButton2(registerButtonLeft + 100 + 20, startY + (boxHeight + 30) * 3, _T("登录"));

    // 循环等待用户输入
    while (true) {
        // 检测鼠标事件
        if (MouseHit()) {
            MOUSEMSG msg = GetMouseMsg();
            if (msg.uMsg == WM_LBUTTONDOWN) {
                // 检测是否点击了注册按钮
                if (msg.x >= registerButtonLeft && msg.x <= registerButtonLeft + 100 && msg.y >= startY + (boxHeight + 30) * 3 && msg.y <= startY + (boxHeight + 30) * 3 + 30) {
                    registerUser();
                }
                // 检测是否点击了登录按钮
                else if (msg.x >= registerButtonLeft + 100 + 20 && msg.x <= registerButtonLeft + 100 + 20 + 100 && msg.y >= startY + (boxHeight + 30) * 3 && msg.y <= startY + (boxHeight + 30) * 3 + 30) {
                    resetLoginInfo(); // 调用Login.cpp中的函数来重置登录信息
                    closegraph(); // 关闭当前窗口
                    showLoginScreen(false); // 跳转到登录界面
                    return; // 退出注册界面的显示函数
                }
                // 检测是否点击了用户名输入框
                else if (usernameBox2.Check(msg.x, msg.y)) {
                    usernameBox2.OnMessage(); // 输入用户名
                }
                // 检测是否点击了密码输入框
                else if (passwordBox2.Check(msg.x, msg.y)) {
                    passwordBox2.OnMessage(); // 输入密码
                }
                // 检测是否点击了邮箱输入框
                else if (emailBox.Check(msg.x, msg.y)) {
                    emailBox.OnMessage(); // 输入邮箱
                }
            }
        }
    }

    // 关闭窗口
    closegraph();
}
