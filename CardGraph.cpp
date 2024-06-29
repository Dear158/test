
///****************************************************************************
/// @author  : Dongyang Wang                                                         
/// @date    : 2024年6月24日                                                                
/// @file    :                                                                 
/// @brief   :                                                                 
///----------------------------------------------------------------------------
///                           Change History                                   
///----------------------------------------------------------------------------
///                              
///****************************************************************************

//卡牌绘图类的实现
#include "CardGraph.h"

IMAGE* CardGraph::cardbk = nullptr;

CardGraph::CardGraph(const SingleCard& _sc) {
    img = new IMAGE(cardW, cardH);
    SetWorkingImage(img);
    setbkmode(TRANSPARENT);
    setlinecolor(DARKGRAY);
    setfillcolor(WHITE);
    setlinestyle(PS_SOLID, 1);
    fillroundrect(0, 0, cardW - 1, cardH - 1, 10, 10);
    settextstyle(48, 0, _T("Arial"));
    LOGFONT f;
    gettextstyle(&f);
    f.lfQuality = ANTIALIASED_QUALITY;
    settextstyle(&f);
    wchar_t cc = 0x2660;
    if (_sc.type == CardColor::SPADE) {
        cc = 0x2660;
        settextcolor(RGB(1, 0, 0));
        outtextxy(cardW - 4 - textwidth(cc), cardH - 2 - textheight(cc), cc);
        f.lfHeight = 26;
        settextstyle(&f);
        outtextxy(5, 24, cc);
    }
    else if (_sc.type == CardColor::HEART) {
        cc = 0x2665;
        settextcolor(RGB(255, 0, 0));
        outtextxy(cardW - 4 - textwidth(cc), cardH - 2 - textheight(cc), cc);
        f.lfHeight = 26;
        settextstyle(&f);
        outtextxy(5, 24, cc);
    }
    else if (_sc.type == CardColor::DIAMOND) {
        cc = 0x2666;
        settextcolor(RGB(255, 0, 0));
        outtextxy(cardW - 4 - textwidth(cc), cardH - 2 - textheight(cc), cc);
        f.lfHeight = 26;
        settextstyle(&f);
        outtextxy(5, 24, cc);
    }
    else {
        cc = 0x2663;
        settextcolor(RGB(1, 0, 0));
        outtextxy(cardW - 4 - textwidth(cc), cardH - 2 - textheight(cc), cc);
        f.lfHeight = 26;
        settextstyle(&f);
        outtextxy(5, 24, cc);
    }
    f.lfHeight = 24;
    settextstyle(&f);
    TCHAR tcs[3] = { 0 };
    switch (_sc.key) {
    case 'A':
        tcs[0] = '1';
        tcs[1] = '0';
        break;
    case 'B':
        tcs[0] = 'J'; break;
    case 'C':
        tcs[0] = 'Q'; break;
    case 'D':
        tcs[0] = 'K'; break;
    case 'E':
        tcs[0] = 'A'; break;
    default:
        tcs[0] = _sc.key;
    }
    outtextxy(4, 4, tcs);
    SetWorkingImage();
}

CardGraph::~CardGraph() {
    delete img;
    img = nullptr;
}

void CardGraph::Draw(int _x, int _y, bool _looked) {
    putTimage(_x, _y, _looked ? img : cardbk);
}

void CardGraph::CreateCardbk() {
    cardbk = new IMAGE(cardW, cardH);
    SetWorkingImage(cardbk);
    setlinecolor(DARKGRAY);
    setfillcolor(LIGHTGRAY);
    setlinestyle(PS_SOLID, 1);
    fillroundrect(0, 0, cardW - 1, cardH - 1, 10, 10);
    setlinecolor(BROWN);
    DrawHeart(25, 40, 50 / 4);
    DrawHeart(25, 40, 50 / 4, true);
    setlinecolor(RGB(255, 0, 0));
    rectangle(5, 5, 50 - 5, 80 - 5);
    SetWorkingImage();
}

void CardGraph::putTimage(int x, int y, IMAGE* srcimg, IMAGE* dstimg, UINT transparentcolor) {
    if (!srcimg) {
        // 如果 srcimg 是空指针，则输出错误信息并返回
        std::cerr << "Error: srcimg is nullptr!" << std::endl;
        return;
    }

    HDC dstDC = GetImageHDC(dstimg);
    HDC srcDC = GetImageHDC(srcimg);

    if (!dstDC || !srcDC) {
        // 如果获取 HDC 失败，则输出错误信息并返回
        std::cerr << "Error: Failed to get HDC!" << std::endl;
        return;
    }

    int w = srcimg->getwidth();
    int h = srcimg->getheight();

    TransparentBlt(dstDC, x, y, w, h, srcDC, 0, 0, w, h, transparentcolor);
}


void CardGraph::DrawHeart(int _x, int _y, int _dxy, bool _inverse) {
    int inv = _inverse ? -1 : 1;
    line(_x, _y, _x - _dxy, _y - _dxy * inv);
    line(_x, _y, _x + _dxy, _y - _dxy * inv);
    POINT ps1[4];
    ps1[0] = { _x - _dxy, _y - _dxy * inv };
    ps1[1] = { _x - _dxy * 2, _y - _dxy * 2 * inv };
    ps1[2] = { _x - _dxy / 2, _y - _dxy * 3 * inv };
    ps1[3] = { _x, _y - (_dxy + _dxy / 2) * inv };
    polybezier(ps1, 4);
    POINT ps2[4];
    ps2[0] = { _x + _dxy, _y - _dxy * inv };
    ps2[1] = { _x + _dxy * 2, _y - _dxy * 2 * inv };
    ps2[2] = { _x + _dxy / 2, _y - _dxy * 3 * inv };
    ps2[3] = { _x, _y - (_dxy + _dxy / 2) * inv };
    polybezier(ps2, 4);
}

CardGraph3::CardGraph3(const Cards3& _cd3) {
    cg3 = new CardGraph[3]{ _cd3.cards[0],_cd3.cards[1],_cd3.cards[2] };
}

CardGraph3::~CardGraph3() {
    delete[] cg3;
    cg3 = nullptr;
}

void CardGraph3::Draw(int _x, int _y, bool _looked) {
    for (int i = 0; i < 3; ++i) {
        cg3[i].Draw(_x + i * (CardGraph::cardW + 10), _y, _looked);
    }
}
