# Grupo 8
# João Pedro Gomes - 13839069
# Luana Hartmann Franco da Cruz - 13676350

RED := "\033[31;49m"
BLUE := "\033[0;34m"
GREEN := "\033[37;42m"
DEFALUT := "\033[39;49m"
casos := $(wildcard casos_introdutorio/*.in) $(wildcard casos_pratico1/*.in)
testes = $(casos:.in=.myout)

all:
	gcc src/*.c  src/*/*.c -I include -g -Wall -ggdb -o main.o

run%:
	cd casos_$*/; ../main.o

run:
	./main.o

debug%:
	cd casos_$*/; gdb ../main.o

%.myout: %.in all
	@echo $(BLUE)%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
	@echo "%% NOW TESTING $<"
	@echo %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
	@cd $(*D); cat $(notdir $*).in | ../main.o > $(notdir $*).myout

	@echo $(RED)
	@diff --strip-trailing-cr $@ $*.out || :
	@echo $(DEFALUT)

test: $(testes)

zip:
	rm *.zip || :
	zip -r ediii.zip include/ src/ Makefile