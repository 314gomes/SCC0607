/**
 * Grupo 8
 * João Pedro Gomes - 13839069
 * Luana Hartmann Franco da Cruz - 13676350
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "grafos/utils.h"
#include "grafos/busca.h"
#include "grafos/insercao.h"
#include "grafos/impressao.h"
#include "grafos/pilha.h"

/// @brief Funcao utilizada para identificar qual o indice de determinada string no
/// vetor de vertices.
/// @param vertices Vetor de vertices.
/// @param string String que sera procurada no vetor de vertices.
/// @param n Numero de vetores no vertice.
/// @return Retorna qual o indicie da string no vetor. Caso nao exista retorna -1.
int indiceVertice (Vertice *vertices, char *string, int n) {

    for (int i = 0; i < n; i++) {
        if (vertices[i].origem != NULL && strcmp(string, vertices[i].origem) == 0) {
            return i;
        }
    }
    
    return -1;
}

/// @brief Procura vertice com menor distancia que não tenha sido visitado
/// @param valorDistancia Vetor de distancias entre a origem recebida e 
/// os demais vertices do grafo.
/// @param verificado Vetor que armazena se o vertice ja foi verificado na
/// busca pelo menor caminho.
/// @param n Numero de vertices.
/// @return Retorna o indicie do menor caminho partindo da origem definida.
int menorDistancia(int *valorDistancia, int *verificado, int n) {
    int menor = -1, primeiro = 1;

    for(int i = 0; i < n; i++) {

        if(valorDistancia[i] >= 0 && verificado[i] == 0) {
            // se for a primeira aresta do vertice sendo verificada
            // ela e pre-definida como menor.
            if (primeiro) {
                menor = i;
                primeiro = 0;
            // caso nao seja a primeira aresta, realizar comparacao com o menor
            // e atualizar 'menor' caso necessario.
            } else {
                if(valorDistancia[menor] > valorDistancia[i])
                    menor = i;
            }
        }
    }
    // indice do vertice que satisfaz a condicao: menor distancia e nao visitado
    return menor;
}

/// @brief Funcao que ira completar os vetores 'verticeAnterior' e 'valorDistancia' ao calcular a 
/// menor distancia entre uma tecnologia pre-definda 'inicio' e os demais vertices do grafo. Utiliza
/// o algoritmo de Dijkstra.
/// @param vertices Vetor de vertices.
/// @param inicio Tecnologia dada como origem dos caminhos que serao calculados.
/// @param verticeAnterior Vetor que armazena o antecessor de um vertice no menor caminho ate ele.
/// @param valorDistancia Vetor que armazena a menor distancia entre a origem e um vertice do vetor.
/// @param n Numero de vertices.
void menorCaminho(Vertice *vertices, char *inicio, int* verticeAnterior, int* valorDistancia, int n) {
    
    // vetor que armazenara se um vertice ja foi visitado ou nao
    int *verificado; 
    // variaveis que armazenarao temporariamente o indice de uma aresta, 
    // de um vertice e da origem
    int arestaIndex, verticeIndex, origemIndex;

    // inicializa os vetores
    verificado = (int*) malloc(n * sizeof(int));
    for(int i = 0; i < n; i++){
        verticeAnterior[i] = -1;
        valorDistancia[i] = -1;
        verificado[i] = 0; 
    }
    
    // salva o indice da tecnologia de origem em 'origemIndex' e
    // atribui 0 a sua distancia no vetor 'valorDistancia'
    origemIndex = indiceVertice(vertices, inicio, n);
    valorDistancia[origemIndex] = 0;

    // enquanto tem vertice para verificar
    for (int i = 0; i < n; i++) {

        // indice do vertice que nao foi verificado ainda e que tem a menor distanca
        verticeIndex = menorDistancia(valorDistancia, verificado, n);
  
        if(verticeIndex == -1)
            break;

        verificado[verticeIndex] = 1;
        
        // verificar todas as arestas de 'verticeIndex'
        for(int j = 0; j < vertices[verticeIndex].grau_saida; j++) {
                
            // indice da tecnologia da aresta que esta sendo verificada
            arestaIndex = indiceVertice(vertices, vertices[verticeIndex].arestas[j].destino, n);
            
            // ninguem verificou em 'arestaIndex' ainda, distancia invalida
            if (valorDistancia[arestaIndex] < 0) {

                // a distancia se torna a distancia do seu vertice anterior + o proprio peso
                valorDistancia[arestaIndex] = valorDistancia[verticeIndex] + vertices[verticeIndex].arestas[j].peso;
                verticeAnterior[arestaIndex] = verticeIndex; 

            // 'arestaIndex' ja foi verificada
            } else {
                
                // caso a nova distancia seja menor, a distancia e o antecessor sao atualizados
                if (valorDistancia[arestaIndex] > valorDistancia[verticeIndex] + vertices[verticeIndex].arestas[j].peso) {
                    valorDistancia[arestaIndex] = valorDistancia[verticeIndex] + vertices[verticeIndex].arestas[j].peso;
                    verticeAnterior[arestaIndex] = verticeIndex;
                }

            }
        }
    }
    free(verificado);
}

/// @brief "Deep-Fist Search", a busca em profundidade se inicia em um determinado 'indice' tenta 
/// verificar o maior numero possivel de arestas partindo dele ate que chegue em um vertice sem mais
/// arestas para verificar, entao retorna e realiza o mesmo com o proximo nao verificado.
/// @param pilha Pilha na qual serao empilhados os indices dos vertices buscados pela dfs.
/// @param vertices Vetor de vertices.
/// @param indiceInicial Indice inicial no qual iniciara a busca em profundidade.
/// @param visitado Vetor que indica se determinado vertice ja foi verificado.
/// @param n Numero de vertices.
/// @param  bool Boleana que indica se e a primeira ou a segunda interacao com a dfs. Caso seja a 
/// primeira, teremos 'TRUE' e ocorrera o empilhamento. Caso seja 'FALSE' nao ocorrera o empilhamento.
void dfsPilha(Pilha *pilha, Vertice *vertices, int indiceInicial, int *visitado, int n, int bool) {

    // marca o vertice como verificado e empilha
    visitado[indiceInicial] = 1;

    // 'indiceAresta' armazenara temporariamente o indice das arestas 
    // do vertice de 'indiceInicial'
    int indiceAresta = 0;

    // realiza a verificacao
    for (int i = 0; i < vertices[indiceInicial].grau_saida; i++) {
        indiceAresta = indiceVertice(vertices, vertices[indiceInicial].arestas[i].destino, n);
        // caso a aresta nao tenha sido visitada, e chamada a recursvidade da dfs
        if (visitado[indiceAresta] == 0)
            dfsPilha(pilha, vertices, indiceAresta, visitado, n, bool);
    }

    // caso seja a primeira interacao com a dfs, ocorre o empilhamento
    if (bool == TRUE) {
        empilha(pilha, indiceInicial);
    }
        
}

/// @brief Funcao feita para calcular o numero de componentes conexos de um grafo. Utiliza o 
/// algoritmo de kosaraju, o qual realiza duas interacoes em uma busca profunda. Uma para empilhar
/// os indices do grafo e outra para desempilhar esses mesmos indices e encaminha-los para a 
/// busca em profundidade com o transposto desse grafo.
/// @param vertices Vetor de vertices de um grafo.
/// @param verticesT Vetor de vertices de um grafo transposto.
/// @param n Numero de vertices.
/// @return Retorna o numero de componentes conexos do grafo.
int kosaraju (Vertice *vertices, Vertice *verticesT, int n) {
 
    Pilha *pilha = (Pilha*) malloc (sizeof(Pilha));
    inicializaPilha(pilha);
    
    // vetor que armazenara se um vertice ja foi visitado ou nao.
    int *verificados;
    // inicializa 'verificados'
    verificados = (int*) malloc(n * sizeof(int));
    for(int i = 0; i < n; i++){
        verificados[i] = 0; 
    }
    
    // primeira parte: criar uma pilha de indices
    // com a busca em profundidade com o grafo.
    for (int i = 0; i < n; i++) {
        if (verificados[i] == 0) {
            dfsPilha(pilha, vertices, i, verificados, n, TRUE);
        }      
    }

    // segunda parte: desempilhar a pilha de indicies
    // com a busca em profundidade com o grafo transposto
    // para contabilizar o numero de componentes conexos.
    int nroComponentes = 0;
    // reinicializa 'verificados'
    for(int i = 0; i < n; i++) {
        verificados[i] = 0;
    }

    // enquanto a pilha nao for complemente esvaziada,
    // ocorrera o desempilhamento
    while (!estaVazia(pilha)) {
        int indice = desempilha(pilha);
        // caso o indice nao tenha sido verificado durante a dfs,
        // ele e encaminhado para a dfs e o numero de componentes conexos
        // e incrementado.
        if (verificados[indice] == 0) {
            dfsPilha(pilha, verticesT, indice, verificados, n, FALSE);
            nroComponentes++;
        }
    }

    free(pilha);
    free(verificados);

    return nroComponentes;
}
