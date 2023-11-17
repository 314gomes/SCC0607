# Grupo 8
# João Pedro Gomes - 13839069
# Luana Hartmann Franco da Cruz - 13676350

all:
	gcc src/*.c  src/*/*.c -I include -g -Wall -ggdb -o main.o

run:
	./main.o

debug:
	gdb main.o