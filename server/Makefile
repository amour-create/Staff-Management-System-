objects = main.o RA.o RC.o RD.o UC.o UL.o UQ.o US.o
 
servers : $(objects)
	cc -o servers $(objects) -pthread -lsqlite3
 
main.o : server.h
RA.o : server.h
RC.o : server.h
RD.o : server.h
UC.o : server.h
UL.o : server.h
UQ.o : server.h
US.o : server.h
.PHONY : clean
clean :
	rm servers $(objects)
