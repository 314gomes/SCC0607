all:
	gcc src/*.c -I include -ggdb -o main.o

run:
	./main.o

debug:
	gdb main.o