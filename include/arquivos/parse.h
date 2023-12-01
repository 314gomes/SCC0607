#include <stdio.h>
#include "arquivos/tipos.h"

int isempty(const char *s);

void parseTokenInt(char* tkn, int *n);

void parseTokenStringVariavel(char* tkn, StringVariavel *str);

void parseLinhaCSV(char *CSV_line, Registro *r_buffer);

void parseCSV(FILE *CSV_in, FILE *BIN_out, Cabecalho *c_buffer);