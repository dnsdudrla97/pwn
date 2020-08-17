CC	=	gcc
CFLAGS	=	-I../include -ggdb -Os -fomit-frame-pointer -mpreferred-stack-boundary=2 

all: exec.o
	$(CC) $(CFLAGS) -shared -o exec.a exec.c
	gcc prog.c ./exec.a -o exec
clean:
	rm -f *.o
