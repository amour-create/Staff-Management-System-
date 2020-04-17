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

int usernumber; //用来存储当前登录用户的用户编号
int loginflag;//是否成功登陆，0失败，1为普通用户，2为管理员用户
char thisuser[20];//当前用户名

void menu1();//欢迎菜单函数
void menu_usr();//欢迎菜单函数
void menu_register();//欢迎菜单函数
int fun_login(int sendfd);//登陆函数
int regsiter_add(int input,int sendfd);//增加用户
int regsiter_del(int input,int sendfd);//删除函数
int regsiter_change(int input,int sendfd);//修改函数
int usr_search(int input,int sendfd);//寻找函数
int usr_changpassword(int input,int sendfd);
int usr_quit(int input,int sendfd);//退出函数




