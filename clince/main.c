#include "clince.h"

void *receive(void *p)//线程实现接收函数
{
	int funlistenfd=*((int *)p);
	//printf("pthread recive success! connfd = %d\n", funlistenfd);
	char recvbuf[BUFFERSIZE]={0};
	while(1)
	{
		if((recv(funlistenfd,recvbuf,BUFFERSIZE,0))==0)
		{
			printf("recv failed\n");
			break;
		}
		printf("接收到服务器信息:%s\n",recvbuf);
		if(strcmp(recvbuf,"普通用户")==0)
		{
			loginflag=1;
			//printf("loginflag%d\n",loginflag);//测试输出
		}
		if(strcmp(recvbuf,"管理员用户")==0)
		{
			loginflag=2;
			//printf("loginflag%d\n",loginflag);//测试输出
		}
		if(strcmp(recvbuf,"#")==0)
		{
			loginflag=0;
			//printf("loginflag is %d\n",loginflag);//测试输出
		}
		bzero(recvbuf,BUFFERSIZE);
	}
}


int main(int argc, char *argv[])
{
	if(argc<3)
	{
		printf("too few arguments\n");
		printf("Usage: %s <ipaddr> <port>\n",argv[0]);
		exit(0);
	}
	int sockfd,input;
	pthread_t pid;
	char buf[BUFFERSIZE]={0};
	struct sockaddr_in servaddr;
	/*创建socket*/
	if((sockfd=socket(AF_INET,SOCK_STREAM,0))==-1)
	{
		perror("socket");
		exit(-1);
	}
	/*设置sockaddr_in结构体中相关参数*/
	bzero(&servaddr,sizeof(servaddr));
	servaddr.sin_family=AF_INET;
	servaddr.sin_port=htons(atoi(argv[2]));
	servaddr.sin_addr.s_addr=inet_addr(argv[1]);
	/*调用connect()函数向服务器端建立TCP链接*/
	if(connect(sockfd,(struct sockaddr*)&servaddr,sizeof(servaddr))==-1)
	{
		perror("connect");
		exit(-1);
	}
	if((pthread_create(&pid,NULL,receive,&sockfd))!=0)
	{
		perror("创建线程");
		exit(-1);
	}

	while (1)
	{
		menu1();
		while (loginflag == 0)
		{
			fun_login(sockfd);
			sleep(1);
			if(loginflag != 0)
			{
				break;
			}
			printf("用户名错误或密码错误，请联系管理员！\n");
			memset(thisuser,0,sizeof(thisuser));
		}
		
		while(loginflag == 1)
		{
			menu_usr();
			scanf("%d",&input);
			switch(input)
			{
				case 1:
				//执行用户查询函数
				usr_search(input,sockfd);
				break;
				case 2:
				//执行用户修改密码函数
				usr_changpassword(input,sockfd);
				break;
				default:
				printf("程序即将退出\n");
				usr_quit(0,sockfd);
				exit(0);
			}
			sleep(1);
		}
		
		while(loginflag == 2)
		{
			menu_register();
			scanf("%d",&input);
			switch(input)
			{
				case 1:
				//执行管理员增加用户函数
				regsiter_add(input,sockfd);
				break;
				case 2:
				//执行管理员删减用户函数
				regsiter_del(input,sockfd);
				break;
				case 3:
				//执行管理员修改用户函数
				regsiter_change(input,sockfd);
				break;
				case 4:
				//执行用户查询函数
				usr_search(input,sockfd);
				break;
				default:
				printf("程序即将退出\n");
				usr_quit(0,sockfd);
				exit(0);
			}
			sleep(1);
		}
		sleep(1);
	}
	
	close(sockfd);
	exit(0);
}

