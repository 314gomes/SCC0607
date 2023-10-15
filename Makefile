all:
	gcc src/*.c -I include -g -Wall -ggdb -o main.o

run:
	./main.o

debug:
	gdb main.o