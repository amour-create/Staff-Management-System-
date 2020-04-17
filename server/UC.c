#include"server.h"


int usr_changpassword(MSG * m,int sendfd)
{
	printf("I'm in usr_changpassword fun!\n");
	char sql[BUFFERSIZE]={0};
	char **resultp;
	int n_h=0,n_l=0;
	char *errmsg;
	sprintf(sql,"select * from user where number= %d;",m->info.no);
		
	if(sqlite3_get_table(mydb,sql,&resultp,&n_h,&n_l,&errmsg)!=0)
	{
		perror("get table first");
		return -1;
	}
	else
	{
		if(n_h == 0)//找到的结果行>0，说明无结果，不可修改
		{
			send(sendfd,"用户名不存在，不可修改！",strlen("用户名不存在，不可修改！"),0);
			printf("修改失败\n");
			return -1;
		}
		else//找到的结果行=0，说明有结果，可以修改
		{
			memset(sql,0,BUFFERSIZE);
			sprintf(sql,"update user set passwd= '%s' where number = %d;",m->pass,m->info.no);
			if(sqlite3_exec(mydb, sql, NULL, NULL, &errmsg) != 0)	 //执行插入信息
			{
				perror("change:");
				return -1;
			}
			memset(sql,0,BUFFERSIZE);
			sprintf(sql,"select * from user where pass='%s' and number = %d;",
				m->pass, m->info.no);
			if(sqlite3_get_table(mydb,sql,&resultp,&n_h,&n_l,&errmsg)!=0)
			{
				perror("get table second");
				return -1;
			}
			else
			{
				if(n_h>0)
				{	
					send(sendfd,"修改成功",strlen("修改成功"),0);
					printf("修改成功\n");
					return 0;
				}
				else
				{
					send(sendfd,"修改失败",strlen("修改失败"),0);
					printf("修改失败\n");
					return -1;
				}
			}
		}
	}
	return 0;
}

