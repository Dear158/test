#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <memory.h> 
#include <time.h>
#include <io.h>
#include <errno.h>
#include <fcntl.h>
#include <winsock2.h>
#include<Windows.h>
#include <WS2tcpip.h>
#include<pthread.h>


#include <sys/types.h>
#include <thread>

#pragma comment(lib, "ws2_32.lib")


/*FD_SETSIZE定义描述符集的大小，定义在sys/types.h中*/
#ifndef FD_SETSIZE
#define FD_SETSIZE 256
#endif

#define MAX_LINE  8192
#define PORT  8888
#define LISTENEQ  6000


typedef SSIZE_T ssize_t;
typedef unsigned short uint16_t;


/*定义服务器 -- 客户端 消息传送结构体*/
typedef struct _Message{
	int msgType;	/*消息类型 */
	int card[3];   /*卡牌*/
	struct sockaddr_in sendAddr; /*发送者IP*/
	int turn;	/*出牌序号*/
}Message;