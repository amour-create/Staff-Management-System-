#include"server.h"



int regsiter_del(MSG * m,int sendfd)
{
	char sql[BUFFERSIZE]={0};
	char **resultp;
	int n_h=0,n_l=0;
	char *errmsg;
	sprintf(sql,"select * from user where number= %d and name= '%s';",m->info.no,m->user);
		
	if(sqlite3_get_table(mydb,sql,&resultp,&n_h,&n_l,&errmsg)!=0)
	{
		perror("get table");
		return -1;
	}
	else
	{
		if(n_h == 0)//找到的结果行>0，说明无结果，删除的用户名重复
		{
			send(sendfd,"用户不存在或编号与用户名不匹配！",strlen("用户不存在或编号与用户名不匹配！"),0);
			printf("删除失败\n");
			return -1;
		}
		else//找到的结果行=1，说明有结果，可以删除
		{
			memset(sql,0,BUFFERSIZE);
			sprintf(sql,"delete from user where number = %d;",m->info.no);
			if(sqlite3_exec(mydb, sql, NULL, NULL, &errmsg) != 0)	 //执行删除信息
			{
				perror("delete");
				return -1;
			}
			memset(sql,0,BUFFERSIZE);
			sprintf(sql,"select * from user where number='%d';",m->info.no);
			if(sqlite3_get_table(mydb,sql,&resultp,&n_h,&n_l,&errmsg)!=0)
			{
				perror("get table");
				return -1;
			}
			else
			{
				if(n_h == 0)
				{	
					send(sendfd,"删除成功",strlen("删除成功"),0);
					printf("删除成功\n");
					return 0;
				}
				else
				{
					send(sendfd,"删除失败",strlen("删除失败"),0);
					printf("删除失败\n");
					return -1;
				}
			}
		}
	}

}

