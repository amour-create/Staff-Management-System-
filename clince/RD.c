#include "clince.h"

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

