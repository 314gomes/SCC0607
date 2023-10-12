all:
	gcc src/*.c -I include -Wall -ggdb -o main.o

run:
	./main.o

debug:
	gdb main.o