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

int usernumber;


int usr_login(MSG * m,int sendfd,char *thisuser);
int regsiter_add(MSG * m,int sendfd);
int regsiter_del(MSG * m,int sendfd);
int regsiter_change(MSG * m,int sendfd);
int usr_search(MSG * m,int sendfd);
int usr_quit(MSG * m,int sendfd);
int usr_changpassword(MSG * m,int sendfd);
void *receive(void *p);








