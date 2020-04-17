#include"server.h"


int regsiter_add(MSG * m,int sendfd)
{
	char sql[BUFFERSIZE]={0};
	char **resultp;
	int n_h=0,n_l=0;
	char *errmsg;
	printf("phone:%s\n",m->info.phone);
	sprintf(sql,"select * from user where number= %d;",m->info.no);
		
	if(sqlite3_get_table(mydb,sql,&resultp,&n_h,&n_l,&errmsg)!=0)
	{
		perror("get table");
		return -1;
	}
	else
	{
		if(n_h>0)//找到的结果行>0，说明有结果，注册的用户名重复
		{
			send(sendfd,"用户名重复！",strlen("用户名重复！"),0);
			printf("注册失败\n");
			return -1;
		}
		else//找到的结果行=0，说明无结果，可以注册
		{
			memset(sql,0,BUFFERSIZE);
			sprintf(sql,"insert into user values(%d,%d,'%s','%s',%d,%d,'%s',1);",
				m->info.no,m->info.type,m->user,m->pass,m->info.age,m->info.salary,
				m->info.phone);
			if(sqlite3_exec(mydb, sql, NULL, NULL, &errmsg) != 0)	 //执行插入信息
			{
				perror("insert");
				return -1;
			}
			memset(sql,0,BUFFERSIZE);
			sprintf(sql,"select * from user where name='%s';",m->user);
			if(sqlite3_get_table(mydb,sql,&resultp,&n_h,&n_l,&errmsg)!=0)
			{
				perror("get table");
				return -1;
			}
			else
			{
				if(n_h>0)
				{	
					send(sendfd,"注册成功",strlen("注册成功"),0);
					printf("注册成功\n");
					return 0;
				}
				else
				{
					send(sendfd,"注册失败",strlen("注册失败"),0);
					printf("注册失败\n");
					return -1;
				}
			}
		}
	}

}

