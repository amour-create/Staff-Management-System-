#include"server.h"



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

