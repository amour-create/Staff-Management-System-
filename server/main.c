#include"server.h"


void *receive(void *p)//线程实现接收函数
{
	int recsockfd=*((int *)p);
	printf("pthread recive success! connfd = %d\n", recsockfd);
	int choice;
	MSG a;
	int n;
	char sql[BUFFERSIZE] = {0};
	char thisuser[20] = {0};
	char *errmsg;
	while(1)
	{
		n = recv(recsockfd, &a, sizeof(a), 0);
		if (n == 0)
		{
			printf("对方已下线\n");
			sprintf(sql,"update user set available=1 where number=%d;",usernumber);
			//将available值置1，允许再次访问
			if(sqlite3_exec(mydb, sql, NULL, NULL, &errmsg) != 0)	 //执行插入信息
			{
				perror("update available 0 to 1");
			}
			break;
		}
		choice = a.type;
		usernumber = a.info.no;
		switch(choice)
		{
			case 0:
			//执行用户登陆函数
			usr_login(&a,recsockfd,thisuser);
			break;
			case 1:
			//执行管理员增加用户函数
			regsiter_add(&a,recsockfd);
			break;
			case 2:
			//执行管理员删减用户函数
			regsiter_del(&a,recsockfd);
			break;
			case 3:
			//执行管理员修改用户函数
			regsiter_change(&a,recsockfd);
			break;
			case 4:
			//执行用户查询函数
			usr_search(&a,recsockfd);
			break;
			case 5:
			//执行用户退出函数
			usr_quit(&a,recsockfd);
			break;
			case 6:
			//执行用户修改密码函数
			usr_changpassword(&a,recsockfd);
			break;
		}
	}

	
}


int main(int argc, char *argv[])
{
	if(argc<4)
	{
		printf("too few arguments\n");
		printf("Usage: %s <ipaddr> <port> <database>\n",argv[0]);
		exit(0);
	}

	
	int listenfd,connfd;
	struct sockaddr_in servaddr,cliaddr;
	socklen_t peerlen;
	char buf[BUFFERSIZE];
	pthread_t pth;
	/*建立socket链接*/
	if((listenfd=socket(AF_INET,SOCK_STREAM,0))==-1)
	{
		perror("socket");
		exit(-1);
	}
	printf("listenfd=%d\n",listenfd);

	/*设置sockaddr_in结构体中相关参数*/
	bzero(&servaddr,sizeof(servaddr));
	servaddr.sin_family=AF_INET;
	servaddr.sin_port=htons(atoi(argv[2]));
	servaddr.sin_addr.s_addr=inet_addr(argv[1]);
	
	/*绑定函数bind()*/
	if(bind(listenfd,(struct sockaddr *)&servaddr,sizeof(servaddr))<0)
	{
		perror("bind");
		exit(-1);
	}
	printf("Bind success\n");

	/*调用listen()函数，设置监听模式*/
	if(listen(listenfd,10)==-1)
	{
		perror("listen");
		exit(-1);
	}
	printf("Listening……\n");

	peerlen=sizeof(cliaddr);
	if(sqlite3_open(argv[3],&mydb)!=0)//打开数据库
	{
		perror("open userdb");
		exit(-1);
	}
	while(1)
	{
		if((connfd=accept(listenfd,(struct sockaddr*)&cliaddr,&peerlen))<0)
		{
			perror("accept");
			exit(-1);
		}

		if((pthread_create(&pth,NULL,receive,&connfd))!=0)
		{
			perror("创建线程");
			exit(-1);
		}
	}
	sqlite3_close(mydb);//关闭数据库
	close(connfd);
	close(listenfd);
}

