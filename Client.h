#pragma once

typedef struct _cardlevel{
	int kind;//牌型
	int level;//大小
}Cardlevel;



#define MAX_LINE  8192
#define PORT  8888
#define LISTENEQ  6000

//函数声明

int GameSever(const char* ipAddress, int & fd, int matchId, int userId);

void enterGame(int* sockfd);

void mainInterface();

void gameInterface();

void showCards(int* Player, int Count);//打印玩家的牌

void changeCard(int Card);//将数字和纸牌进行对换

void sortCards(int* Nums, int Length);//排序手中的牌

void delCard(int cards[], int number, int length);//删除手牌

int canSend(int card[], int lastcard[], int length, int lastlength);//判断能不能出牌

Cardlevel whatlevel(int sendcard[], int length);//判断牌的大小