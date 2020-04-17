#include "clince.h"

int usr_quit(int input,int sendfd)//退出函数
{
	printf("I'm quit\n");
	MSG a;
	a.type=5;
	a.info.no = usernumber;
	strcpy(a.pass,"NULL");
	strcpy(a.user,"NULL");
	send(sendfd,&a,sizeof(a),0);
	return 0;
}

