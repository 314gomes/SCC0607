#include "arquivos/utils.h"

int indiceVertice (Vertice *vertices, char *string, int n); 

void menorCaminho(Vertice *vertices, char *inicio, int* ant, int* dist, int n);

void dfsPilha (Pilha *pilha, Vertice *vertices, int ini, int *visitado, int n);

void dfsComponentes(Vertice *vT, int indice, int *verificados, int n);

int kosaraju (Vertice *v, Vertice *vT, int n);
/*
void DFS(Vertice* vertices, int vertice, int* visitados, Pilha* pilha, int n);

void kosaraju(Vertice *vertices, int n, FILE* bin);
*/