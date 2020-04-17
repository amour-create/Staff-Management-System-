#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sqlite3.h>
#include<sys/socket.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<time.h>

#define BUFFERSIZE 256

typedef struct
{
	char name[BUFFERSIZE];//姓名
	int  age;//年龄
	int  no;//编号
	int  salary;//工资
	char phone[BUFFERSIZE];//手机号
	int type;//员工级别
}INFO;



typedef struct{
	int sign;
	int type;//判断操作类型 
	char user[BUFFERSIZE];//发送消息的户
	INFO info;//员工信息结构体
	char pass[BUFFERSIZE];//用户密码在第一次登录时使用）
	char data[BUFFERSIZE];//操作成功或者失败的消息
}MSG;

sqlite3 *mydb;
int usernumber = 0;

int usr_login(MSG * m,int sendfd,char *thisuser)
{
	char sql[BUFFERSIZE]={0};
	char **resultp;
	int n_h=0,n_l=0;
	char *errmsg;
	int adm = 0;//用户的权限
	usernumber = m->info.no;
	printf("writer user number in login: %d\n", usernumber);
	
	sprintf(sql,"select * from user where number=%d and passwd='%s' and available=1;",m->info.no,m->pass);
	if(sqlite3_get_table(mydb,sql,&resultp,&n_h,&n_l,&errmsg)!=0)
	{
		perror("get table");
		return -1;
	}
	else
	{
		if(n_h>0)//找到的结果行>0，说明有结果，登陆成功
		{
			adm = atoi(resultp[9]);
			printf("admint result(0/1): %d\n", adm);
			if (adm == 0)
				send(sendfd,"login user success",strlen("login user success"),0);
			if (adm == 1)
				send(sendfd,"login register success",strlen("login register success"),0);
			strcpy(thisuser,m->user);
			memset(sql,0,sizeof(sql));
			sprintf(sql,"update user set available=0 where number=%d;",m->info.no);
			//将available值置0，表示有客户访问此账号，其他客户不允许访问
			if(sqlite3_exec(mydb, sql, NULL, NULL, &errmsg) != 0)	 //执行插入信息
			{
				perror("update available 1 to 0");
				//return -1;
			}
			printf("登陆成功\n");
		}
		else//找到的结果行=0，说明无结果，登陆失败
		{
			send(sendfd,"login failed",strlen("login failed"),0);
			printf("登陆失败\n");
		}
	}
}


int regsiter_add(MSG * m,int sendfd)
{
	char sql[BUFFERSIZE]={0};
	char **resultp;
	int n_h=0,n_l=0;
	char *errmsg;
	printf("phone:%s\n",m->info.phone);
	sprintf(sql,"select * from user where number= %d;",m->info.no);
		
	if(sqlite3_get_table(mydb,sql,&resultp,&n_h,&n_l,&errmsg)!=0)
	{
		perror("get table");
		return -1;
	}
	else
	{
		if(n_h>0)//找到的结果行>0，说明有结果，注册的用户名重复
		{
			send(sendfd,"用户名重复！",strlen("用户名重复！"),0);
			printf("注册失败\n");
			return -1;
		}
		else//找到的结果行=0，说明无结果，可以注册
		{
			memset(sql,0,BUFFERSIZE);
			sprintf(sql,"insert into user values(%d,%d,'%s','%s',%d,%d,'%s',1);",
				m->info.no,m->info.type,m->user,m->pass,m->info.age,m->info.salary,
				m->info.phone);
			if(sqlite3_exec(mydb, sql, NULL, NULL, &errmsg) != 0)	 //执行插入信息
			{
				perror("insert");
				return -1;
			}
			memset(sql,0,BUFFERSIZE);
			sprintf(sql,"select * from user where name='%s';",m->user);
			if(sqlite3_get_table(mydb,sql,&resultp,&n_h,&n_l,&errmsg)!=0)
			{
				perror("get table");
				return -1;
			}
			else
			{
				if(n_h>0)
				{	
					send(sendfd,"register success",strlen("register success"),0);
					printf("注册成功\n");
					return 0;
				}
				else
				{
					send(sendfd,"register failed",strlen("register failed"),0);
					printf("注册失败\n");
					return -1;
				}
			}
		}
	}

}

int regsiter_del(MSG * m,int sendfd)
{
	char sql[BUFFERSIZE]={0};
	char **resultp;
	int n_h=0,n_l=0;
	char *errmsg;
	sprintf(sql,"select * from user where number= %d and name= '%s';",m->info.no,m->user);
		
	if(sqlite3_get_table(mydb,sql,&resultp,&n_h,&n_l,&errmsg)!=0)
	{
		perror("get table");
		return -1;
	}
	else
	{
		if(n_h == 0)//找到的结果行>0，说明无结果，删除的用户名重复
		{
			send(sendfd,"用户不存在或编号与用户名不匹配！",strlen("用户不存在或编号与用户名不匹配！"),0);
			printf("删除失败\n");
			return -1;
		}
		else//找到的结果行=1，说明有结果，可以删除
		{
			memset(sql,0,BUFFERSIZE);
			sprintf(sql,"delete from user where number = %d;",m->info.no);
			if(sqlite3_exec(mydb, sql, NULL, NULL, &errmsg) != 0)	 //执行删除信息
			{
				perror("delete");
				return -1;
			}
			memset(sql,0,BUFFERSIZE);
			sprintf(sql,"select * from user where number='%d';",m->info.no);
			if(sqlite3_get_table(mydb,sql,&resultp,&n_h,&n_l,&errmsg)!=0)
			{
				perror("get table");
				return -1;
			}
			else
			{
				if(n_h == 0)
				{	
					send(sendfd,"delete success",strlen("delete success"),0);
					printf("删除成功\n");
					return 0;
				}
				else
				{
					send(sendfd,"delete failed",strlen("delete failed"),0);
					printf("删除失败\n");
					return -1;
				}
			}
		}
	}

}

int regsiter_change(MSG * m,int sendfd)
{
	char sql[BUFFERSIZE]={0};
	char **resultp;
	int n_h=0,n_l=0;
	char *errmsg;
	sprintf(sql,"select * from user where number= %d;",m->info.no);
		
	if(sqlite3_get_table(mydb,sql,&resultp,&n_h,&n_l,&errmsg)!=0)
	{
		perror("get table");
		return -1;
	}
	else
	{
		if(n_h == 0)//找到的结果行>0，说明无结果，不可修改
		{
			send(sendfd,"用户名不存在，不可修改！",strlen("用户名不存在，不可修改！"),0);
			printf("修改失败\n");
			return -1;
		}
		else//找到的结果行=0，说明有结果，可以修改
		{
			memset(sql,0,BUFFERSIZE);
			sprintf(sql,"update user set admint= %d,name= '%s',passwd= '%s',age= %d,salary= %d,phone= '%s' where number = %d;",
				m->info.type,m->user,m->pass,m->info.age,m->info.salary,m->info.phone,m->info.no);
			if(sqlite3_exec(mydb, sql, NULL, NULL, &errmsg) != 0)	 //执行插入信息
			{
				perror("change:");
				return -1;
			}
			memset(sql,0,BUFFERSIZE);
			sprintf(sql,"select * from user where name='%s' and number = %d;",
				m->user, m->info.no);
			if(sqlite3_get_table(mydb,sql,&resultp,&n_h,&n_l,&errmsg)!=0)
			{
				perror("get table");
				return -1;
			}
			else
			{
				if(n_h>0)
				{	
					send(sendfd,"register success",strlen("register success"),0);
					printf("修改成功\n");
					return 0;
				}
				else
				{
					send(sendfd,"register failed",strlen("register failed"),0);
					printf("修改失败\n");
					return -1;
				}
			}
		}
	}

}

int usr_search(MSG * m,int sendfd)
{

	char sql[BUFFERSIZE]={0};
	char **resultp;
	int n_h=0,n_l=0;
	char *errmsg;
	sprintf(sql,"select * from user where number= %d;",m->info.no);
		
	if(sqlite3_get_table(mydb,sql,&resultp,&n_h,&n_l,&errmsg)!=0)
	{
		perror("get table");
		return -1;
	}
	else
	{
		if(n_h == 0)//找到的结果行>0，说明无结果
		{
			send(sendfd,"用户名不存在！",strlen("用户名不存在！"),0);
			printf("查找失败\n");
			return -1;
		}
		else//找到的结果行=1，说明有结果，可以发送
		{
			printf("查找成功\n");
			memset(sql,0,BUFFERSIZE);
			sprintf(sql,"user ：\n编号： %s\n权限： %s\n名字； %s\n年龄: %s\n工资：%s\n电话：%s\n",
				resultp[8],resultp[9],resultp[10],resultp[12],resultp[13],resultp[14]);
			send(sendfd,sql,strlen(sql),0);//结果发送给客户端

		}
	}
	

}

int usr_quit(MSG * m,int sendfd)
{

	return 0;
}

int usr_changpassword(MSG * m,int sendfd)
{
	printf("I'm in usr_changpassword fun!\n");
	char sql[BUFFERSIZE]={0};
	char **resultp;
	int n_h=0,n_l=0;
	char *errmsg;
	sprintf(sql,"select * from user where number= %d;",m->info.no);
		
	if(sqlite3_get_table(mydb,sql,&resultp,&n_h,&n_l,&errmsg)!=0)
	{
		perror("get table first");
		return -1;
	}
	else
	{
		if(n_h == 0)//找到的结果行>0，说明无结果，不可修改
		{
			send(sendfd,"用户名不存在，不可修改！",strlen("用户名不存在，不可修改！"),0);
			printf("修改失败\n");
			return -1;
		}
		else//找到的结果行=0，说明有结果，可以修改
		{
			memset(sql,0,BUFFERSIZE);
			sprintf(sql,"update user set passwd= '%s' where number = %d;",m->pass,m->info.no);
			if(sqlite3_exec(mydb, sql, NULL, NULL, &errmsg) != 0)	 //执行插入信息
			{
				perror("change:");
				return -1;
			}
			memset(sql,0,BUFFERSIZE);
			sprintf(sql,"select * from user where pass='%s' and number = %d;",
				m->pass, m->info.no);
			if(sqlite3_get_table(mydb,sql,&resultp,&n_h,&n_l,&errmsg)!=0)
			{
				perror("get table second");
				return -1;
			}
			else
			{
				if(n_h>0)
				{	
					send(sendfd,"register success",strlen("register success"),0);
					printf("修改成功\n");
					return 0;
				}
				else
				{
					send(sendfd,"register failed",strlen("register failed"),0);
					printf("修改失败\n");
					return -1;
				}
			}
		}
	}
	return 0;
}


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
