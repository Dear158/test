//卡牌绘图类的定义
#pragma once
#ifndef CARDGRAPH_H
#define CARDGRAPH_H

#include "Cards.h"
#include <graphics.h>
#pragma comment(lib, "MSIMG32.LIB")

class CardGraph {
public:
    CardGraph(const SingleCard& _sc);
    ~CardGraph();
    void Draw(int _x, int _y, bool _looked = true);
    static void CreateCardbk();
    static void putTimage(int x, int y, IMAGE* srcimg, IMAGE* dstimg = nullptr, UINT transparentcolor = 0);

private:
    static void DrawHeart(int _x, int _y, int _dxy, bool _inverse = false);
    IMAGE* img;

public:
    static IMAGE* cardbk;
    static const int cardW = 50;
    static const int cardH = 80;
};

class CardGraph3 {
public:
    CardGraph3() = default;
    CardGraph3(const Cards3& _cd3);
    ~CardGraph3();
    void Draw(int _x, int _y, bool _looked = true);

private:
    CardGraph* cg3;
};

#endif // CARDGRAPH_H

