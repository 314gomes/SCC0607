# Grupo 8
# João Pedro Gomes - 13839069
# Luana Hartmann Franco da Cruz - 13676350

RED := "\033[31;49m"
BLUE := "\033[0;34m"
GREEN := "\033[37;42m"
DEFALUT := "\033[39;49m"
casos := $(wildcard casos_introdutorio/*.in)
testes = $(casos:.in=.myout)

all:
	gcc src/*.c  src/*/*.c -I include -g -Wall -ggdb -o main.o

run:
	./main.o

debug:
	gdb main.o

casos_introdutorio/%.myout: casos_introdutorio/%.in all
	@echo $(BLUE)
	@echo %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
	@echo "%% NOW TESTING $<"
	@echo %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
	@echo $(DEFALUT)
	@cd casos_introdutorio; cat $*.in | ../main.o > $*.myout

	@echo $(RED)
	@diff --strip-trailing-cr $@ casos_introdutorio/$*.out || :
	@echo $(DEFALUT)

test: $(testes)
	echo $(testes)