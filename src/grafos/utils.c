/**
 * Grupo 8
 * João Pedro Gomes - 13839069
 * Luana Hartmann Franco da Cruz - 13676350
*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "grafos/utils.h"
#include "grafos/defines.h"

/// @brief Funcao utilizada para inicializar um vetor de vertices.
/// @param n Numero de vertices.
/// @return Retorna um vetor de vertices.
Vertice *novo_vetorVertice (int n) {
    Vertice *vertices = (Vertice*) malloc(n*sizeof(Vertice));

    for (int i = 0; i < n; ++i) {
        vertices[i].origem = NULL;  // Substitua pelos valores desejados
        vertices[i].grupo = 0;
        vertices[i].grau_entrada = 0;
        vertices[i].grau_saida = 0;
        vertices[i].grau_total = 0;
        vertices[i].num_arestas = 0;
        
        // Aloca memória para o array de arestas
        vertices[i].arestas = (Aresta*)malloc(NUM_ARESTAS * sizeof(Aresta));
    }

    return vertices;
}


/// @brief Libera memoria de um vetor de vertices.
/// @param v Ponteiro de Vertice a ter sua memoria liberada.
/// @param n Numero de vertices.
void free_vertice (Vertice *vertices, int n) {

    if (vertices == NULL) {
        return;
    }

    for (int i = 0; i < n; ++i) {
        free(vertices[i].arestas);
        free(vertices[i].origem);
    }

    free(vertices);
}
