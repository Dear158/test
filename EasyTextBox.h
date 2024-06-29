#ifndef EASYTEXTBOX_H
#define EASYTEXTBOX_H

#include <graphics.h>

class EasyTextBox {
private:
    int left, top, right, bottom; // 控件坐标
    wchar_t* text;                // 控件内容
    size_t maxlen;                // 文本框最大内容长度

public:
    EasyTextBox();                // 构造函数
    ~EasyTextBox();               // 析构函数
    void Create(int x1, int y1, int x2, int y2, int max);
    wchar_t* Text();
    bool Check(int x, int y);
    void Show();
    void OnMessage();
    void SetText(const wchar_t* newText); // 设置文本框内容
};

#endif // EASYTEXTBOX_H
#pragma once
