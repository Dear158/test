#include"Config.h"
#include"Client.h"
#include"clientGame.h"
#include<iostream>
#include <graphics.h>
#include <conio.h>
#include"GameMain.h"
#include <string.h>
#include"GameMatch.h"
int GameSever(const char* ipAddress, int & fd, int matchId, int userId_)
{
	int sockfd;
	int choice;	//choice代表用户在主界面所做选择
	int ret;
	pthread_t pid;
	struct sockaddr_in servaddr;
	struct hostent* host;

	/*声明消息变量*/
	Message message;
	/*声明消息缓冲区*/
	char buf[MAX_LINE];

	while (1)
	{
		/*(1) 创建套接字*/
		if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
		{
			perror("socket error");
			exit(1);
		}
		else {
			fd = sockfd;
		}
		//std::cout << sockfd << std::endl;

		/*(2) 设置链接服务器地址结构*/
		memset(&servaddr, 0, sizeof(servaddr));		/*清空地址结构*/
		servaddr.sin_family = AF_INET;				/*使用IPV4通信域*/
		servaddr.sin_port = htons(PORT);			/*端口号转换为网络字节序*/
		//servaddr.sin_addr = *((struct in_addr *)host->h_addr);		/*可接受任意地址*/

		if (inet_pton(AF_INET, ipAddress, &servaddr.sin_addr) < 0)//判断ip地址格式是否正确
		{
			printf("inet_pton error for %s\n", ipAddress);
			exit(1);
		}//if

		 /*(3) 发送链接服务器请求*/
		if (connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0)
		{
			perror("connect error");
			exit(1);
		}//if	

		memset(buf, 0, MAX_LINE);//清空buf数组
		int userId = GetCurrentUserId();
		sprintf(buf, "%d", userId);
		//memcpy(buf, "1024",sizeof(str));

		send(sockfd, buf, sizeof(buf), 0);

		printf("%s", buf);


		recv(sockfd, buf, sizeof(buf), 0);
		printf("%s", buf);
		

		//if (strcmp(buf, "游戏人数已满\n") == 0)
		//{
		//	close(sockfd);
		//	printf("退出游戏!\n");
		//	exit(0);
		//}
		//TODO: 显示游戏界面 此处暂时使用终端显示
		//mainInterface();

		//setbuf(stdin, NULL); //是linux中的C函数，主要用于打开和关闭缓冲机制 这里用来清空缓存区
		//scanf("%d", &choice);//输入数字表示选择
		/*while (choice != 1 && choice != 2)
		{
			printf("未找到命令，请重新输入！\n");
			setbuf(stdin, NULL);
			scanf("%d", &choice);
		}
		*/
		//switch (choice)
		//{
		//case 1:	/*准备请求*/
			memset(&message, 0, sizeof(message));
			memset(buf, 0, MAX_LINE);

			message.msgType = 1;
			message.sendAddr = servaddr;

			/*向服务器发送准备请求*/
			memcpy(buf, &message, sizeof(message));
			send(sockfd, buf, sizeof(buf), 0);

		 while (strcmp(buf, "游戏开始\n") != 0)
			{
				memset(buf, 0, MAX_LINE);
				ret = recv(sockfd, buf, sizeof(buf), 0);
				if (ret <= 0)
				{
					printf("异常掉线，请重新登录！\n");

					memset(&message, 0, sizeof(message));
					memset(buf, 0, MAX_LINE);

					message.msgType = 3;
					message.sendAddr = servaddr;
					memcpy(buf, &message, sizeof(message));
					send(sockfd, buf, sizeof(buf), 0);
					close(sockfd);
					exit(0);
				}//if
			}
			system("reset");//清屏
			printf("%s", buf);
			int fd_ = sockfd;
			ShowGameMatchPage(matchId, userId_,fd_);
			//clientGame(&sockfd,matchId,userId_);
			//enterGame(&sockfd);
			//break;
			
		
//	case 2:/*退出请求*/
		/*	memset(&message, 0, sizeof(message));
			memset(buf, 0, MAX_LINE);

			message.msgType = 2;
			message.sendAddr = servaddr;
*/
			/*向服务器发送退出请求*/
		/*
			memcpy(buf, &message, sizeof(message));
			send(sockfd, buf, sizeof(buf), 0);
			close(sockfd);
			printf("退出游戏!\n");
		} */
			//exit(0);	/*用户退出*/
			/*
			break;
		default:
			printf("输入错误，请重新输入\n");
			break;
			*/
		//switch
		
	}//while	
	close(sockfd);
	return 0;
}