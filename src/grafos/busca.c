#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "grafos/utils.h"
#include "grafos/busca.h"
#include "grafos/insercao.h"
#include "grafos/impressao.h"

/// @brief 
/// @param vertices 
/// @param string 
/// @param n 
/// @return 
int indiceVertice (Vertice *vertices, char *string, int n) {

    for (int i = 0; i < n; i++) {
        if (vertices[i].origem != NULL && strcmp(string, vertices[i].origem) == 0) {
            return i;
        }
    }
    
    return -1;
}

/// @brief Procura vertice com menor distancia que não tenha sido visitado
/// @param dist 
/// @param visitado 
/// @param n 
/// @return 
int menorDistancia(int *valorDistancia, int *verificado, int n) {
    int menor = -1, primeiro = 1;

    for(int i = 0; i < n; i++) {

        if(valorDistancia[i] >= 0 && verificado[i] == 0) {
            if (primeiro) {
                menor = i;
                primeiro = 0;
            } else {
                if(valorDistancia[menor] > valorDistancia[i])
                    menor = i;
            }
        }
    }
    // indice do vertice que satisfaz a condicao: menor distancia e nao visitado
    return menor;
}

/// @brief 
/// @param vertices 
/// @param inicio 
/// @param verticeAnterior 
/// @param valorDistancia 
/// @param n 
void menorCaminho(Vertice *vertices, char *inicio, int* verticeAnterior, int* valorDistancia, int n){
    
    int cont;
    int *verificado;
    int arestaIndex, verticeIndex, origemIndex;

    cont = n;
    verificado = (int*) malloc(n * sizeof(int));
    
    for(int i = 0; i < n; i++){
        verticeAnterior[i] = -1; // anterior
        valorDistancia[i] = -1; // distancia
        verificado[i] = 0; 
    }
    
    // salva o indice da tecnologia inicial no vetor de vertices em 'index'
    origemIndex = indiceVertice(vertices, inicio, n);
    valorDistancia[origemIndex] = 0;

    // enquanto tem vertice para visitar
    while(cont > 0){
        // indice do vertice que nao foi verificado ainda e que tem a menor distanca
        verticeIndex = menorDistancia(valorDistancia, verificado, n);
        //printf("u = %d\n",u);
        if(verticeIndex == -1)
            break;

        verificado[verticeIndex] = 1;
        cont--;
        
        // visitar todos os vizinhos de vert
        for(int i = 0; i < vertices[verticeIndex].grau_saida; i++){
                
            // indice da tecnologia da aresta
            arestaIndex = indiceVertice(vertices, vertices[verticeIndex].arestas[i].destino, n);
            
            if (valorDistancia[arestaIndex] < 0) { // ninguem chegou nele aarestaIndexa, distancia invalida

               valorDistancia[arestaIndex] = valorDistancia[verticeIndex] + vertices[verticeIndex].arestas[i].peso;
               verticeAnterior[arestaIndex] = verticeIndex; 

            } else {

                if (valorDistancia[arestaIndex] > valorDistancia[verticeIndex] + vertices[verticeIndex].arestas[i].peso) {
                    valorDistancia[arestaIndex] = valorDistancia[verticeIndex] + vertices[verticeIndex].arestas[i].peso;
                    verticeAnterior[arestaIndex] = verticeIndex;
                }

            }
        }
    }

    free(verificado);
}

/*
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
*/


