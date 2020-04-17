#include "clince.h"

int regsiter_change(int input,int sendfd)//修改函数
{
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

