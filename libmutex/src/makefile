FILES= mymutex.c mycond.c mytest.c
FLAGS= -lmythread -I . -L .

all: test

test: $(FILES)
	gcc -g $(FILES) $(FLAGS) -o runMe

clean: 
	rm -f runMe
