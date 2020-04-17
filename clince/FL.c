#include "clince.h"

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

