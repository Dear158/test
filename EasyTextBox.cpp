
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

#include "EasyTextBox.h"
// ʵ���ı���ؼ�
// ���캯��
EasyTextBox::EasyTextBox() : left(0), top(0), right(0), bottom(0), text(NULL), maxlen(0) {}

// ��������
EasyTextBox::~EasyTextBox() {
    if (text != NULL) {
        delete[] text;
    }
}

// �����ı���
void EasyTextBox::Create(int x1, int y1, int x2, int y2, int max) {
    maxlen = max;
    text = new wchar_t[maxlen];
    text[0] = 0;
    left = x1, top = y1, right = x2, bottom = y2;

    // �����û�����
    Show();
}

// ��ȡ�ı�����
wchar_t* EasyTextBox::Text() {
    return text;
}

// ��������Ƿ����ı�����
bool EasyTextBox::Check(int x, int y) {
    return (left <= x && x <= right && top <= y && y <= bottom);
}

// ��ʾ�ı���
void EasyTextBox::Show() {
    // ���ݻ���ֵ
    int oldlinecolor = getlinecolor();
    int oldbkcolor = getbkcolor();
    int oldfillcolor = getfillcolor();

    setlinecolor(LIGHTGRAY);    // ���û�����ɫ
    setbkcolor(0xeeeeee);       // ���ñ�����ɫ
    setfillcolor(0xeeeeee);     // ���������ɫ
    fillrectangle(left, top, right, bottom);
    outtextxy(left + 10, top + 5, text);

    // �ָ�����ֵ
    setlinecolor(oldlinecolor);
    setbkcolor(oldbkcolor);
    setfillcolor(oldfillcolor);
}

// �����ı�����ı�
void EasyTextBox::SetText(const wchar_t* newText) {
    if (wcslen(newText) < maxlen) {
        wcscpy_s(text, maxlen, newText);
        settextcolor(BLACK); // �����ı���ɫΪ��ɫ
        Show(); // �����ı�����ʾ
    }
}

// ������Ϣ
void EasyTextBox::OnMessage() {
    // ���ݻ���ֵ
    int oldlinecolor = getlinecolor();
    int oldbkcolor = getbkcolor();
    int oldfillcolor = getfillcolor();

    setlinecolor(BLACK);        // ���û�����ɫ
    setbkcolor(WHITE);          // ���ñ�����ɫ
    setfillcolor(WHITE);        // ���������ɫ
    fillrectangle(left, top, right, bottom);
    settextcolor(BLACK); // �����ı���ɫΪ��ɫ
    outtextxy(left + 10, top + 5, text);

    int width = textwidth(text);    // �ַ����ܿ���
    int counter = 0;                // �����˸������
    bool binput = true;             // �Ƿ�������

    ExMessage msg;
    while (binput) {
        while (binput && peekmessage(&msg, EX_MOUSE | EX_CHAR, false)) { // ��ȡ��Ϣ����������Ϣ�����ó�
            if (msg.message == WM_LBUTTONDOWN) {
                // ���������ı������棬�����ı�����
                if (msg.x < left || msg.x > right || msg.y < top || msg.y > bottom) {
                    binput = false;
                    break;
                }
            }
            else if (msg.message == WM_CHAR) {
                size_t len = wcslen(text);
                switch (msg.ch) {
                case '\b':      // �û����˸����ɾ��һ���ַ�
                    if (len > 0) {
                        text[len - 1] = 0;
                        width = textwidth(text);
                        counter = 0;
                        clearrectangle(left + 10 + width, top + 1, right - 1, bottom - 1);
                    }
                    break;
                case '\r':      // �û����س����������ı�����
                case '\n':
                    binput = false;
                    break;
                default:        // �û����������������ı�����
                    if (len < maxlen - 1) {
                        text[len++] = msg.ch;
                        text[len] = 0;

                        clearrectangle(left + 10 + width + 1, top + 3, left + 10 + width + 1, bottom - 3); // ������Ĺ��
                        width = textwidth(text);    // ���¼����ı������
                        counter = 0;
                        outtextxy(left + 10, top + 5, text);    // ����µ��ַ���
                    }
                }
            }
            peekmessage(NULL, EX_MOUSE | EX_CHAR); // ����Ϣ���������ոմ�������һ����Ϣ
        }

        // ���ƹ�꣨�����˸����Ϊ 20ms * 32��
        counter = (counter + 1) % 32;
        if (counter < 16) {
            line(left + 10 + width + 1, top + 3, left + 10 + width + 1, bottom - 3); // �����
        }
        else {
            clearrectangle(left + 10 + width + 1, top + 3, left + 10 + width + 1, bottom - 3); // �����
        }

        // ��ʱ 20ms
        Sleep(20);
    }

    clearrectangle(left + 10 + width + 1, top + 3, left + 10 + width + 1, bottom - 3); // �����

    // �ָ�����ֵ
    setlinecolor(oldlinecolor);
    setbkcolor(oldbkcolor);
    setfillcolor(oldfillcolor);

    Show();
}