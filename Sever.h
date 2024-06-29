#pragma once
#include"Config.h"



#define MAX_LINE  8192
#define PORT  8888
#define LISTENEQ  6000

void enterGame(int* sockfd);

void* handleRequest(int* fd);//处理请求的线程

void* fapai();//控制发牌的线程

int getOneCard();//通过随机数取得一张没用过的牌

void showCards(int* Player, int Count);//打印玩家的牌

void changeCard(int Card);//将数字和纸牌进行对换

void sortCards(int* Nums, int Length);//排序手中的牌

void sendCards();//发牌函数