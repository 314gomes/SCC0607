/**
 * Grupo 8
 * João Pedro Gomes - 13839069
 * Luana Hartmann Franco da Cruz - 13676350
*/
#include <stdio.h>

typedef struct {
    char *origem;
    int peso;
} Aresta;

// Estrutura para representar uma tecnologia
typedef struct {
    char *destino;
    int grupo;
    int grau_entrada;
    int grau_saida;
    int grau_total;
    Aresta* arestas; // Lista de arestas
    int num_arestas; // Número de arestas
} Vertice;