/**
 * Grupo 8
 * João Pedro Gomes - 13839069
 * Luana Hartmann Franco da Cruz - 13676350
*/

#include "arquivos/utils.h"

int compararArestas(const void *a, const void *b);

void ordenarArestas(Vertice *vertice);

int compararDestinos(const void *a, const void *b);

void ordenarDestinos(Vertice *vertices, int n);


void atualizaVertice (int index, Vertice *vertices, Registro* r_buffer, char* origem, char* destino, int n, int flag, int bool);

void insereGrafo (Vertice *vertices, Registro *r_buffer, int n);

void insereGrafoTransposto (Vertice *vertices, Registro *r_buffer, int n);

Vertice *criaGrafo (FILE* bin, Vertice *vertices, int n, int bool);