#include "clince.h"

int usr_search(int input,int sendfd)//寻找函数
{
	MSG a;
	a.type=4;
	printf("请输入要查询的编号：\n");
	scanf("%d",&a.info.no);
	strcpy(thisuser,a.user);
	send(sendfd,&a,sizeof(a),0);
	return 0;
}

