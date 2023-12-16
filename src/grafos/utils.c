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

/// @brief  Alocar memoria e atribuir valores padrao a um buffer Vertice
/// @return Ponteiro para novo Vertice
Vertice *novo_vertice () {

    Vertice* v_buffer;
    v_buffer = (Vertice*) malloc(sizeof(Vertice));

    v_buffer->origem = NULL;
    v_buffer->grupo = 0;
    v_buffer->grau_entrada = 0;
    v_buffer->grau_saida = 0;
    v_buffer->grau_total = 0;
    v_buffer->num_arestas = 0;

    v_buffer->arestas = (Aresta*) malloc(NUM_ARESTAS*sizeof(Aresta));

    return v_buffer;

}

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

/*
/// @brief Libera memoria de Vertice *
/// @param v Ponteiro de Vertice a ter sua memoria liberada
void free_vertice (Vertice *vertices, int n) {

    if (vertices == NULL) {
        return;
    }

    for (int i = 0; i < n; ++i) 
        free(vertices[i].arestas);
        free(vertices[i].origem);
    }

    free(vertices);
}
*/