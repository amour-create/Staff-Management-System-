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
		if(strcmp(recvbuf,"login user success")==0)
		{
			loginflag=1;
			//printf("loginflag%d\n",loginflag);//测试输出
		}
		if(strcmp(recvbuf,"login register success")==0)
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

void menu1()//欢迎菜单函数
{
	printf("******************************\n");
	printf("欢迎使用用户管理系统\n");
	printf("******************************\n");
	printf("请登录！\n");
	printf("******************************\n");
	
}

void menu_usr()//欢迎菜单函数
{
	printf("******************************\n");
	printf("请选择：\n");
	printf("1 查询用户	2 修改密码   3.退出\n");
	printf("******************************\n");
}

void menu_register()//欢迎菜单函数
{
	printf("******************************\n");
	printf("请选择：\n");
	printf("1 增加用户	2 删除用户   3.修改用户\n");
	printf("4.查询用户  5.退出\n");
	printf("******************************\n");
}

int fun_login(int sendfd)//登陆函数
{
	MSG a;
	char *pass;
	a.type=0;
	printf("请输入用户编号:");
	scanf("%d",&a.info.no);
	usernumber = a.info.no;
	strcpy(thisuser,a.user);
	pass=getpass("请输入密码:");//隐藏输入密码
	strcpy(a.pass,pass);
	send(sendfd,&a,sizeof(a),0);
	return 0;
}

int regsiter_add(int input,int sendfd)//增加用户
{
	MSG a;
	char *pass1;//注册用确定密码
	a.type=input;
	printf("请输入用户名:");
	scanf("%s",a.user);
	strcpy(a.info.name,a.user);
	pass1=getpass("请输入密码:");//隐藏输入密码
	strcpy(a.pass,pass1);
	printf("请输入新增员工编号:");
	scanf("%d",&a.info.no);
	printf("请输入新增员工年龄:");
	scanf("%d",&a.info.age);
	printf("请输入新增员工手机号:");
	scanf("%s",a.info.phone);
	printf("请输入新增员工工资:");
	scanf("%d",&a.info.salary);
	printf("请输入新增员工是否为管理员【0,不是/1，是】:");
	scanf("%d",&a.info.type);
	send(sendfd,&a,sizeof(a),0);
	return 0;
}

int regsiter_del(int input,int sendfd)//删除函数
{
	MSG a;
	char *pass;
	a.type=input;
	printf("请输入要删除的用户编号:");
	scanf("%d",&a.info.no);
	printf("请输入要删除的用户名:");
	scanf("%s",a.user);
	strcpy(thisuser,a.user);
	send(sendfd,&a,sizeof(a),0);
	return 0;
}

int regsiter_change(int input,int sendfd)//修改函数
{
	printf("I'm in regsiter_change\n");
	MSG a;
	char *pass1;//注册用确定密码
	a.type=input;
	printf("请输入要修改的用户名:");
	scanf("%s",a.user);
	strcpy(a.info.name,a.user);
	pass1=getpass("请输入要修改的密码:");//隐藏输入密码
	strcpy(a.pass,pass1);
	printf("请输入要修改新增员工编号:");
	scanf("%d",&a.info.no);
	printf("请输入要修改新增员工年龄:");
	scanf("%d",&a.info.age);
	printf("请输入要修改新增员工手机号:");
	scanf("%s",a.info.phone);
	printf("请输入要修改新增员工工资:");
	scanf("%d",&a.info.salary);
	printf("请输入要修改新增员工是否为管理员【0,不是/1，是】:");
	scanf("%d",&a.info.type);
	send(sendfd,&a,sizeof(a),0);
	return 0;
}

int usr_search(int input,int sendfd)//寻找函数
{
	printf("I'm in usr_search\n");
	MSG a;
	a.type=4;
	printf("请输入要查询的编号：\n");
	scanf("%d",&a.info.no);
	strcpy(thisuser,a.user);
	send(sendfd,&a,sizeof(a),0);
	return 0;
}

int usr_changpassword(int input,int sendfd)//更改函数
{
	printf("I'm in usr_changpassword\n");
	MSG a;
	char *pass;
	a.type=6;
	printf("请输入要更改的密码:");
	scanf("%s",a.pass);
	a.info.no = usernumber;
	strcpy(thisuser,a.user);
	send(sendfd,&a,sizeof(a),0);
	return 0;
}

int usr_quit(int input,int sendfd)//退出函数
{
	printf("I'm in usr_quit\n");
	MSG a;
	a.type=5;
	a.info.no = usernumber;
	strcpy(a.pass,"NULL");
	strcpy(a.user,"NULL");
	send(sendfd,&a,sizeof(a),0);
	return 0;
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

