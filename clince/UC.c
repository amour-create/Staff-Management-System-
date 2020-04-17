#include "clince.h"

int usr_changpassword(int input,int sendfd)//更改函数
{
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

