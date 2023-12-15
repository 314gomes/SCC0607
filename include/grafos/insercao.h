#include "arquivos/utils.h"

void insereGrafo (Vertice *vertices, Registro *r_buffer, int n);

void ordenarDestinos(Vertice *vertices, int n);

Vertice *criaGrafo (FILE* bin, Vertice *vertices, int n, int bool);