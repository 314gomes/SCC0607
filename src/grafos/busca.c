#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "grafos/utils.h"
#include "grafos/busca.h"
#include "grafos/insercao.h"
#include "grafos/impressao.h"
#include "grafos/pilha.h"

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

void dfsPilha(Pilha *pilha, Vertice *vertices, int ini, int *visitado, int n) {
    if (visitado[ini] == 1)
        return;

    // marca o vértice como verificado e empilha
    visitado[ini] = 1;
    int index;

    // verifica os não verificados
    for (int i = 0; i < vertices[ini].grau_saida; i++) {
        index = indiceVertice(vertices, vertices[ini].arestas[i].destino, n);
        if (visitado[index] == 0)
            dfsPilha(pilha, vertices, index, visitado, n);
    }

    empilha(pilha, index);
}

void dfsComponentes(Vertice *vT, int indice, int *verificados, int n, int* num) {
    if (verificados[indice] == 1)
        return;

    // marca o vértice como verificado e atribui o componente atual
    verificados[indice] = 1;

    // verifica os não verificados
    for (int i = 0; i < vT[indice].grau_saida; i++) {
        //printf("for\n");
        int index = indiceVertice(vT, vT[indice].arestas[i].destino, n);
        printf("tecnologia %s\n", vT[indice].arestas[i].destino);
        if (verificados[index] == 0) {
            printf("index nao visitado %d\n", index);
            (*num)++;
            printf("num %d\n", (*num));
            dfsComponentes(vT, index, verificados, n, num);
            
            
        }
            
    }
}


int kosaraju (Vertice *v, Vertice *vT, int n) {
 
    Pilha *pilha = (Pilha*) malloc (sizeof(Pilha));
    inicializaPilha(pilha);
    
    int verificados[n];
    // inicializa vetor de vertices verificados
    // 0 nao verificado e 1 verificado
    for(int i = 0; i < n; i++) {
        verificados[i] = 0;
    }
    

    // primeira parte: criar uma pilha de indices
    // com a busca em profundidade com o grafo direcionado e ponderado
    for (int i = 0; i < n; i++) {
        if (verificados[i] == 0)
            dfsPilha(pilha, v, i, verificados, n);
    }


    // segunda parte: desempilhar a pilha de indicies
    // com a busca em profundidade com o grafo transposto
    int nroComponentes = 0;

    for(int i = 0; i < n; i++) {
        verificados[i] = 0;
    }

    while (!estaVazia(pilha)) {
        int indice = desempilha(pilha);
        if (verificados[indice] == 0) {
            printf("entrando dfs %d\n", indice);
            dfsPilha(pilha, vT, indice, verificados, n);
            nroComponentes++;
        }
    }

    return nroComponentes;
}
/*
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


