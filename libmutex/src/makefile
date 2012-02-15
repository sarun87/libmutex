###########################################################
##MAKE FILE - P5
###########################################################


FILES= mymutex.c mycond.c mytest.c
FLAGS= -g -I . -L .
LIB=mythread.a

all: test

test: $(FILES)
	gcc -c $(FILES) $(FLAGS)
	gcc -o mytest $(FILES:.c=.o) $(LIB) $(FLAGS) 

clean: 
	rm -f mytest
	rm -f *.o
