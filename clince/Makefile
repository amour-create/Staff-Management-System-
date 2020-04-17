objects = main.o RA.o RC.o RD.o UC.o FL.o UQ.o US.o MU.o
 
clinces : $(objects)
	cc -o clinces $(objects) -pthread -lsqlite3
 
main.o : clince.h
RA.o : clince.h
RC.o : clince.h
RD.o : clince.h
UC.o : clince.h
FL.o : clince.h
UQ.o : clince.h
US.o : clince.h
MU.o : clince.h
.PHONY : clean
clean :
	rm clinces $(objects)

