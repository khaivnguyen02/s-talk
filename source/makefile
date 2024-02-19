CFLAGS = -Wall -g -std=c99 -D _POSIX_C_SOURCE=200809L -Werror

all: build

build: list.o
	gcc $(CFLAGS) main.c receiver.c write.c send.c read.c list.o -lpthread -o s-talk

run: build
	./s-talk

valgrind: build
	valgrind --leak-check=full ./s-talk

clean: 
	rm -f s-talk