#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "grafos/utils.h"
#include "grafos/impressao.h"

/// @brief Funcao que imprime o grafo por completo de acordo com a 
/// formatacao da documentacao do trabalho.
/// @param vertices Vetor de vertices.
/// @param n Numero de vertices.
void imprimeGrafo (Vertice* vertices, int n) {

    for (int i = 0; i < n; i ++) {
        for (int j = 0; j < vertices[i].num_arestas; j++) {
            printf("%s ", vertices[i].origem);
            printf("%d ", vertices[i].grupo);
            printf("%d ", vertices[i].grau_entrada);
            printf("%d ", vertices[i].grau_saida);
            printf("%d ", vertices[i].grau_total);
            printf("%s ", vertices[i].arestas[j].destino);
            printf("%d ", vertices[i].arestas[j].peso);
            printf("\n");
        }
        
    }
}

/// @brief Funcao que imprime somente um vertice e todas as
/// suas arestas de acordo com a formatacao da docuemntacao
/// do trabalho.
/// @param vertice Um vertice.
void imprimeVertice(Vertice vertice) {

    printf("%s: ", vertice.origem);

    for (int i = 0; i < vertice.num_arestas; i++) {
            printf("%s", vertice.arestas[i].destino);
            if(i != vertice.num_arestas-1)
                printf(", ");
    }

    printf("\n\n");
}