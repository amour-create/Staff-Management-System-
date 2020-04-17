# Staff-Management-System-
基于sqlite3、Linux操作系统研发的员工管理系统

数据库 mydb.db:

头：
create table user (number interger PRIMARY KEY,admint interger,name text,passwd text,age interger,salary interger,phone text,available interger)；

已有管理员用户编号：1
	密码：1

运行make生成执行文件：
	服务器端：servers
	客户端：clinces

运行可执行文件：
	服务器端：./servers <服务器IP> <端口号> <数据库文件> 
		eg: ./servers 192.168.1.56 50000 mydb.db
	客户端：./clince <服务器IP> <端口号> <数据库文件> 
		eg: ./clinces 192.168.1.56 50000 mydb.db
