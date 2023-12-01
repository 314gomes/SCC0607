#include "arquivos/tipos.h"

void escreverLixoRegistro(FILE *arquivo, Registro *r);

void escreverCampoChar(FILE *a, char c);

void escreverCampoInt(FILE *a, int c);

void escreverCampoStringVariavel(FILE *a, StringVariavel c);

void escreverDadosRegistro(FILE *arquivo, Registro *r);

void escreverRegistro(FILE *arquivo, Registro *r);

void escreverCabecalho(FILE *arquivo, Cabecalho *h);