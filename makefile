#outputFile = program

all: program

program: list.o main.o
	gcc -Wall -Werror -o program list.o main.o

list.o: list.c list.h
	gcc -Wall -Werror -c list.c

main.o: main.c list.h
	gcc -Wall -Werror -c main.c

clean:
	rm -f program *.o
