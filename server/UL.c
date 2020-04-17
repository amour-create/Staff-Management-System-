#include"server.h"


int usr_login(MSG * m,int sendfd,char *thisuser)
{
	char sql[BUFFERSIZE]={0};
	char **resultp;
	int n_h=0,n_l=0;
	char *errmsg;
	int adm = 0;//用户的权限
	usernumber = m->info.no;
	
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
				send(sendfd,"普通用户",strlen("普通用户"),0);
			if (adm == 1)
				send(sendfd,"管理员用户",strlen("管理员用户"),0);
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

