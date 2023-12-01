#include <stdio.h>
#include "arquivos/tipos.h"

void leCampoInt(FILE* bin, int *i);

void leCampoChar(FILE* bin, char *c);

void leCampoStringVariavel(FILE* bin, StringVariavel *s);

void leStatusRegistro(FILE *bin, Registro *r);

void leConteudoRegistro(FILE *bin, Registro *r);

void leCabecalho (FILE *bin, Cabecalho *c);

StatusDeRetorno le_RRN(FILE *bin, int RRN, Registro *r);