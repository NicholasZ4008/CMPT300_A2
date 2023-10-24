CC = gcc

all: s-talk

s-talk: driver.c LinkedList/list.c socket_communications.c thread_functions.c -lpthread
	$(CC) -g $^ -lpthread -o $@

clean:
	rm -f s-talk
