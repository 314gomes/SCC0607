#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "grafos/utils.h"
#include "grafos/busca.h"
#include "grafos/insercao.h"
#include "grafos/impressao.h"


int verificaVertices (Vertice *vertices, char *string, int n) {

    for (int i = 0; i < n; i++) {
        if (vertices[i].destino != NULL && strcmp(string, vertices[i].destino) == 0) {
            return i;
        }
    }
    
    return -1;
}

void verificaOrigem (Vertice vertice, char *string) {
    //printf("==> %s\n", vertice.destino);
    for (int i = 0; i < vertice.num_arestas; i++) {
        printf("\t%s,\n", vertice.arestas[i].origem);
        //if (strcmp(string, vertice.arestas[i].origem) == 0)
            //printf ("\t(procuro: %s destino: %s origem: %s)\n",string, vertice.arestas[i].origem, vertice.destino);

    }
}

void DFS(Vertice* vertices, int vertice, int* visitados, Pilha* pilha, int n) {
    if (vertice < 0 || vertice >= n) {
        printf("Erro: Índice inválido em DFS\n");
        return;
    }

    visitados[vertice] = 1;

    for (int i = 0; i < vertices[vertice].num_arestas; i++) {
        int adjacente = vertices[vertice].arestas[i].peso;
        if (adjacente < 0 || adjacente >= n) {
            printf("Erro: Índice adjacente inválido em DFS\n");
            continue;
        }
        if (!visitados[adjacente]) {
            DFS(vertices, adjacente, visitados, pilha, n);
        }
    }

    if (pilha != NULL) {
        empilha(pilha, vertice);  // Empilhar na ordem de finalização
    }
}


void kosaraju(Vertice *vertices, int n, FILE *bin) {
    int *visitados = (int *)malloc(n * sizeof(int));
    memset(visitados, 0, n * sizeof(int));  // Inicializar com 0 (não visitado)

    Pilha pilha;
    inicializaPilha(&pilha);

    for (int i = 0; i < n; ++i) {
        if (i < 0 || i >= n) {
            printf("Erro: Índice inicial inválido em kosaraju\n");
            free(visitados);
            //return 0;  // Ou outro valor apropriado
        }
        if (!visitados[i]) {
            DFS(vertices, i, visitados, &pilha, n);
        }
    }

    // Transpor o grafo usando a função criaGrafo
    Vertice *transposto = criaGrafo(bin, vertices, n, FALSE);

    memset(visitados, 0, n * sizeof(int));  // Reinicializar visitados

    int componentesConexos = 0;

    while (!estaVazia(&pilha)) {
        int i = desempilha(&pilha);

        if (!visitados[i]) {
            DFS(transposto, i, visitados, NULL, n);
            componentesConexos++;
        }
    }

    free(visitados);
    free(transposto);

    if (componentesConexos == 1) {
        printf("Sim, o grafo é fortemente conexo e possui 1 componente.\n");
    } else {
        printf("Nao, o grafo nao e fortemente conexo e possui %d componentes.\n", componentesConexos);
    }

}



