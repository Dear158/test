// Login.h
#ifndef LOGIN_H
#define LOGIN_H
#include <string>
#include "EasyTextBox.h"
// 声明外部链接的变量
extern EasyTextBox usernameBox;
extern EasyTextBox passwordBox;
extern bool rememberMeChecked;
extern std::wstring currentUsername;
void showLoginScreen(bool loadSavedCredentials);
void resetLoginInfo(); // 声明重置登录信息的函数
void SwitchAccount();

#endif
#pragma once
