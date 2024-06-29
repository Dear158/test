#include"Sever.h"

int onlinefd[3];//正在游戏的玩家
int sum = 0;//准备的玩家数--控制发牌
int flag = 0;//连入的客户端--控制能否再连接客户端
int call_;//庄家编号
int Cards[55] = { 0 };//记录每张牌是否被用过( 丢弃cards[0]）

int GameSever(void)
{
	//声明服务器监听描述符和客户链接描述符
	int i, n, ret, maxi, maxfd, listenfd, connfd, sockfd;

	socklen_t clilen;
	pthread_t pid;
	//套接字选项
	int opt = 1;

	//声明服务器地址和客户地址结构
	struct sockaddr_in servaddr, cliaddr;

	//声明消息变量
	Message message;
	//声明描述符集
	fd_set rset, allset;
	//nready为当前可用的描述符数量  
	int nready, client_sockfd[3];
	//声明消息缓冲区
	char buf[MAX_LINE];

	pthread_create(&pid, NULL, reinterpret_cast<void* (__cdecl*)(void*)>(fapai), NULL);//控制发牌的线程

	//(1) 创建套接字
	if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		perror("socket error.\n");
		exit(1);
	}//if

	//(2) 初始化地址结构
	memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(PORT);

	//(3) 绑定套接字和端口
	setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, (char *)( & opt), sizeof(opt));
	//SOL_SOCKET 套接字通用选项 SO_REUSEADDR 允许重用本地地址和端口
	//有两种套接口的选项：一种是布尔型选项，允许或禁止一种特性；另一种是整形或结构选项。允许一个布尔型选项，则将optval指向非零整形数；禁止一个选 项optval指向一个等于零的整形数。

	if (bind(listenfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0)
	{
		perror("bind error.\n");
		exit(1);
	}//if

	//(4) 监听
	if (listen(listenfd, LISTENEQ) < 0)
	{
		perror("listen error.\n");
		exit(1);
	}//if	

	//(5) 首先初始化客户端描述符集
	maxfd = listenfd;
	maxi = -1;
	for (i = 0; i < 3; ++i)
	{
		client_sockfd[i] = -1;
		onlinefd[i] = -1;
	}//for

	//清空allset描述符集
	FD_ZERO(&allset);

	//将监听描述符加到allset中
	FD_SET(listenfd, &allset);

	//(6) 接收客户链接
	while (1)
	{
		rset = allset;
		//得到当前可读的文件描述符数
		nready = select(maxfd + 1, &rset, NULL, NULL, 0);

		//确定一个或多个套接口的状态，如需要则等待。

//测试listenfd是否在rset描述符集中
		if (FD_ISSET(listenfd, &rset))
		{
			//接收客户端的请求
			clilen = sizeof(cliaddr);
			if ((connfd = accept(listenfd, (struct sockaddr*)&cliaddr, &clilen)) < 0)
			{
				perror("accept error.\n");
				exit(1);
			}//if

			printf("服务器: %s 加入游戏\n", inet_ntoa(cliaddr.sin_addr));
			flag++;

			//查找空闲位置，设置客户链接描述符
			for (i = 0; i < FD_SETSIZE; ++i)
			{
				if (client_sockfd[i] < 0)
				{
					client_sockfd[i] = connfd; //将处理该客户端的链接套接字设置在该位置
					break;
				}//if
			}//for

			if (i == FD_SETSIZE)
			{
				perror("too many connection.\n");
				exit(1);
			}//if

			if (flag >= 4)//连入的客户端数
			{
				perror("游戏已开始");
				sprintf(buf, "%s", "游戏人数已满\n");
				send(connfd, buf, sizeof(buf), 0);
			}

			else
			{
				sprintf(buf, "%s", "已进入房间,等待所有玩家准备完毕\n");
				send(connfd, buf, sizeof(buf), 0);
				// 将来自客户的连接connfd加入描述符集 
				FD_SET(connfd, &allset);

				//新的连接描述符 -- for select
				if (connfd > maxfd)
					maxfd = connfd;

				//max index in client_sockfd[]
				if (i > maxi)
					maxi = i;

				//判断有没有请求需要处理
				if (--nready <= 0)
					continue;
			}
		}//if	
		//接下来逐个处理连接描述符（请求）
		for (i = 0; i <= maxi; ++i)
		{
			if ((sockfd = client_sockfd[i]) < 0)
				continue;
			if (FD_ISSET(sockfd, &rset))
			{
				//如果当前没有可以读的套接字，退出循环
				if (--nready < 0)
					break;
				pthread_create(&pid, NULL, reinterpret_cast<void* (__cdecl*)(void*)>(handleRequest), (void*)&sockfd);

				Sleep(1000);//睡眠,让创建线程比删除先执行
				//清除处理完的链接描述符
				FD_CLR(sockfd, &allset);
				client_sockfd[i] = -1;
			}//if

		}//for
	}//while

	close(listenfd);
	return 0;
}

//处理客户请求的线程
void* handleRequest(int* fd)
{
	int sockfd, ret, n, i;
	//声明消息变量
	Message message;
	//声明消息缓冲区
	char buf[MAX_LINE];

	sockfd = *fd;

	memset(buf, 0, MAX_LINE);
	memset(&message, 0, sizeof(message));
	//接收用户发送的消息
	n = recv(sockfd, buf, sizeof(buf) + 1, 0);
	if (n <= 0)
	{
		//关闭当前描述符，并清空描述符数组 
		fflush(stdout);
		close(sockfd);
		*fd = -1;
		printf("来自%s的退出请求！\n", inet_ntoa(message.sendAddr.sin_addr));
		return NULL;
	}//if				
	else {
		memcpy(&message, buf, sizeof(buf));
		switch (message.msgType)
		{
		case 1:
		{
			printf("%s 已经准备！\n", inet_ntoa(message.sendAddr.sin_addr));
			//查找空闲位置，放置在线的链接套接字
			for (i = 0; i < 3; ++i)
			{
				if (onlinefd[i] < 0)
				{
					onlinefd[i] = sockfd;
					sum++;//准备的玩家数加一
					break;
				}//if
			}//for
		//	enterGame(&onlinefd[i]);
			break;
		}//case	
		case 2:
		{
			printf("%s 已经退出！\n", inet_ntoa(message.sendAddr.sin_addr));
			flag--;
			break;
		}//case	
		default:
			printf("未知操作\n");
			break;
		}//switch					
	}//else				

}

//控制发牌的线程
void* fapai()
{
	int i;
	char buf[MAX_LINE];
	while (1)
	{
		if (sum == 3)
		{
			for (i = 0; i < 3; ++i)
			{
				sprintf(buf, "%s", "游戏开始\n");
				send(onlinefd[i], buf, sizeof(buf), 0);
			}
			call_ = rand() % 3;//随机产生一个庄家
			sendCards();//发牌
			for (i = 1; i < 55; ++i)//初始化标志数组
			{
				Cards[i] = 0;
			}
			sum = 0;
		}
	}
}

void sendCards()
{

}
int getOneCard()//通过随机数取得一张没用过的牌
{
	int iRandNum;
	srand(time(0));//取时间随机数（种子）
	iRandNum = rand() % 54 + 1; //rand()%m,产生0- m-1的随机数
	while (Cards[iRandNum] == 1)//被用过的牌会重新取随机数
	{
		iRandNum = rand() % 54 + 1;
	}
	Cards[iRandNum] = 1;
	return iRandNum;
}
void showCards(int* Player, int Count)//打印玩家的牌
{

}
void changeCard(int Card)//将数字和纸牌进行对换
{
	int temp = Card / 4.0000001;//用于控制0和13的越界
	switch (temp)
	{
	case 0:printf("3 "); break;
	case 1:printf("4 "); break;
	case 2:printf("5 "); break;
	case 3:printf("6 "); break;
	case 4:printf("7 "); break;
	case 5:printf("8 "); break;
	case 6:printf("9 "); break;;
	case 7:printf("10 "); break;
	case 8:printf("J "); break;
	case 9:printf("Q "); break;
	case 10:printf("K "); break;
	case 11:printf("A "); break;
	case 12:printf("2 "); break;
	}
}
void sortCards(int* Nums, int Length)//排序手中的牌
{
	int i, j, iTemp;
	for (i = 1; i <= Length - 1; i++)
	{
		for (j = 1; j <= Length - 2; j++)
		{
			if (Nums[j] > Nums[j + 1])
			{
				iTemp = Nums[j];
				Nums[j] = Nums[j + 1];
				Nums[j + 1] = iTemp;
			}
		}
	}
}