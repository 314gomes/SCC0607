#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "grafos/utils.h"
#include "grafos/impressao.h"

void imprimeGrafo (Vertice* vertices, int n) {

    for (int i = 0; i < n; i ++) {
        for (int j = 0; j < vertices[i].num_arestas; j++) {
            printf("%s ", vertices[i].destino);
            printf("%d ", vertices[i].grupo);
            printf("%d ", vertices[i].grau_entrada);
            printf("%d ", vertices[i].grau_saida);
            printf("%d ", vertices[i].grau_total);
            printf("%s ", vertices[i].arestas[j].origem);
            printf("%d ", vertices[i].arestas[j].peso);
            printf("\n");
        }
        
    }
}

void imprimeVertice(Vertice vertice) {

    printf("%s: ", vertice.destino);

    for (int i = 0; i < vertice.num_arestas; i++) {
            printf("%s", vertice.arestas[i].origem);
            if(i != vertice.num_arestas-1)
                printf(", ");
    }

    printf("\n\n");
}