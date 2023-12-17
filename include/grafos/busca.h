/**
 * Grupo 8
 * João Pedro Gomes - 13839069
 * Luana Hartmann Franco da Cruz - 13676350
*/

#include "arquivos/utils.h"

int indiceVertice (Vertice *vertices, char *string, int n); 

int menorDistancia(int *valorDistancia, int *verificado, int n);

void menorCaminho(Vertice *vertices, char *inicio, int* ant, int* dist, int n);

void dfsPilha(Pilha *pilha, Vertice *vertices, int indiceInicial, int *visitado, int n, int bool);

int kosaraju (Vertice *vertices, Vertice *verticesT, int n);
