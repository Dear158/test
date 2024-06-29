#ifndef EASYTEXTBOX_H
#define EASYTEXTBOX_H

#include <graphics.h>

class EasyTextBox {
private:
    int left, top, right, bottom; // �ؼ�����
    wchar_t* text;                // �ؼ�����
    size_t maxlen;                // �ı���������ݳ���

public:
    EasyTextBox();                // ���캯��
    ~EasyTextBox();               // ��������
    void Create(int x1, int y1, int x2, int y2, int max);
    wchar_t* Text();
    bool Check(int x, int y);
    void Show();
    void OnMessage();
    void SetText(const wchar_t* newText); // �����ı�������
};

#endif // EASYTEXTBOX_H
#pragma once
