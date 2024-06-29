// Login.h
#ifndef LOGIN_H
#define LOGIN_H
#include <string>
#include "EasyTextBox.h"
// �����ⲿ���ӵı���
extern EasyTextBox usernameBox;
extern EasyTextBox passwordBox;
extern bool rememberMeChecked;
extern std::wstring currentUsername;
void showLoginScreen(bool loadSavedCredentials);
void resetLoginInfo(); // �������õ�¼��Ϣ�ĺ���
void SwitchAccount();

#endif
#pragma once
