/**
 * Grupo 8
 * João Pedro Gomes - 13839069
 * Luana Hartmann Franco da Cruz - 13676350
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "arquivos/leitura.h"
#include "grafos/utils.h"
#include "grafos/insercao.h"
#include "grafos/busca.h"
#include "arquivos/defines.h"
#include "grafos/defines.h"

// funcoes elaboras para comparar e ordenas as arestas de um vertice.
int compararArestas(const void *a, const void *b) {
    const Aresta *aresta1 = (const Aresta *)a;
    const Aresta *aresta2 = (const Aresta *)b;
    return strcmp(aresta1->destino, aresta2->destino);
}

void ordenarArestas(Vertice *vertice) {
    if (vertice->num_arestas > 0) {
        qsort(vertice->arestas, vertice->num_arestas, sizeof(Aresta), compararArestas);
    }
}

// funcoes elaboras para comparar e ordenas os vertices de um vetor.
int compararDestinos(const void *a, const void *b) {
    const Vertice *vertice1 = (const Vertice *)a;
    const Vertice *vertice2 = (const Vertice *)b;
    return strcmp(vertice1->origem, vertice2->origem);
}

void ordenarDestinos(Vertice *vertices, int n) {
    qsort(vertices, n, sizeof(Vertice), compararDestinos);
}


/// @brief Funcao que atualiza os vertices e as arestas do grafo ponderado e direcionado
/// @param index Indice do vertice no vetor de vertices.
/// @param vertices Vetor de vertices.
/// @param r_buffer Registro temporario que contem os parametros para a insercao no grafo.
/// @param origem Tecnologia que sera tratada como origem na insercao.
/// @param destino Tecnologia que sera tratada como destino na insercao.
/// @param n Numero de vertices
/// @param flag A flag sera 0 quando uma origem estiver sendo inserida e 1 quando um 
/// destino estiver sendo inserido
/// @param bool Boleana que dira se o grafo e transposto.
void atualizaVertice (int index, Vertice *vertices, Registro* r_buffer, char* origem, char* destino, int n, int flag, int bool) {

    // caso o vertice ja exista no vetor de vertices
    if (index != -1) {

        // caso uma origem esteja sendo atualizada
        if (flag == 0) {
            vertices[index].grau_saida++;
            int num_arestas = vertices[index].num_arestas;
            vertices[index].arestas[num_arestas].destino = strdup(destino);
            vertices[index].arestas[num_arestas].peso = r_buffer->peso;
            vertices[index].num_arestas++;

            // caso o grafo nao seja transposto
            if (bool == TRUE)
                vertices[index].grupo = r_buffer->grupo;
 
        }   
        // caso um destino esteja sendo atualizado   
        else {
            vertices[index].grau_entrada++;

            // caso o grafo seja transposto
            if (bool == FALSE)
                vertices[index].grupo = r_buffer->grupo;
        }
            
        vertices[index].grau_total++;

        ordenarArestas(&vertices[index]);
    }

    // caso o vertice nao exista no vetor de vertices
    else {

        // verifica qual o proximo indice livre no
        // vetor de vertices
        int free_index = -1;
        for (int i = 0; i < n; ++i) {
            if (vertices[i].origem == NULL) {
                free_index = i;
                break;
            }
        }

        // caso exista um indice livre
        if (free_index != -1) {
            // caso uma origem esteja sendo criada
            if (flag == 0) {
                vertices[free_index].origem = strdup(origem);                 
                vertices[free_index].grau_saida = 1;
                vertices[free_index].num_arestas = 1;
                vertices[free_index].arestas[0].destino = strdup(destino);
                vertices[free_index].arestas[0].peso = r_buffer->peso;

                // caso o grafo nao seja transposto
                if (bool == TRUE)
                    vertices[free_index].grupo = r_buffer->grupo;
            }

            // caso um destino esteja sendo criado
            else {     
                vertices[free_index].origem = strdup(destino);
                vertices[free_index].grau_entrada = 1;

                // caso o grafo seja transposto
                if (bool == FALSE)
                    vertices[free_index].grupo = r_buffer->grupo;
            }
            
            vertices[free_index].grau_total = 1;
            
        }

        ordenarArestas(&vertices[n-1]);
    }
}

/// @brief Funcao que encaminha os parametros do registro para serem inseridos no grafo
/// direcionado e ponderado
/// @param vertices Vetor que armazena os vertices do grafo.
/// @param r_buffer Registro temporario que sera encaminhado ao grafo.
/// @param n Numero de tecnologias/vertices do grafo.
void insereGrafo (Vertice *vertices, Registro *r_buffer, int n) {

    char *charOrigem = r_buffer->tecnologiaOrigem.string;
    char *charDestino = r_buffer->tecnologiaDestino.string;

    int iDestino = indiceVertice (vertices, charDestino, n);
    int iOrigem = indiceVertice (vertices, charOrigem, n);

    atualizaVertice (iDestino, vertices, r_buffer, charOrigem, charDestino, n, 1, TRUE);
    atualizaVertice (iOrigem, vertices, r_buffer, charOrigem, charDestino, n, 0, TRUE);
}

/// @brief Funcao que encaminha os parametrs do registro para serem inseridos no grafo
/// direcionado, ponderado e transposto.
/// @param vertices Vetor que armazena os vertices do grafo.
/// @param r_buffer Registro temporario que sera encaminhado ao grafo.
/// @param n Numero de tecnologias/vertices do grafo.
void insereGrafoTransposto (Vertice *vertices, Registro *r_buffer, int n) {
    char *charOrigem = r_buffer->tecnologiaOrigem.string;
    char *charDestino = r_buffer->tecnologiaDestino.string;

    int iDestino = indiceVertice (vertices, charDestino, n);
    int iOrigem = indiceVertice (vertices, charOrigem, n);

    // envia 'charOrigem' e 'charDestino' invertidos em relacao ao grafo original para
    // realizar a transposicao
    atualizaVertice (iOrigem, vertices, r_buffer, charDestino, charOrigem, n, 1, FALSE);
    atualizaVertice (iDestino, vertices, r_buffer, charDestino, charOrigem, n, 0, FALSE);
}

/// @brief Funcao principal para a criacao de um grafo. Le o arquivo binario, armazena temporariamente o 
/// registro lido e o encaminha para sua insercao no grafo caso ambas as strings nao sejam nulas.
/// @param bin Caminho para o arquivo binario.
/// @param vertices Vetor que armazena os vertices do grafo.
/// @param n Numero de tecnologias/vertices do grafo.
/// @param  bool Boleana que sera 'TRUE' caso seja um grafo direcionado e ponderado e
/// 'FALSE' caso seja o transposto desse mesmo grafo.
/// @return Retorna o vetor de vertices do grafo.
Vertice *criaGrafo (FILE* bin, Vertice *vertices, int n, int bool) {

    // 'auxiliarChar' armazenando temporariamente o campo 'removido' do registro
    char auxiliarChar;
    Registro *r_buffer = novo_registro();

    while(fread(&(auxiliarChar), sizeof(char), 1, bin) != 0) {
        
        // verifica se foi removido
        if (auxiliarChar == NAO_REMOVIDO) {
            leConteudoRegistro(bin, r_buffer);
            // verifica se 'grupo', 'origem' e 'destino' sao NULO
            if (r_buffer->tecnologiaOrigem.tamanho != 0) {
                if (r_buffer->tecnologiaDestino.tamanho != 0) 
                    if (r_buffer->grupo != -1){
                        // 'TRUE' insere no grafo direcionado e ponderado
                        if (bool == TRUE)
                            insereGrafo(vertices, r_buffer, n);
                        // 'FALSE' insere no grafo transposto
                        else
                            insereGrafoTransposto(vertices, r_buffer, n);
                    }
                    
            }

        }
            
    }

    // verificar se o vetor de vertices foi inteiramente ocupado
    // caso existam espaços vazios, a ordenacao tem segmentation fault
    int vazios = 0;

    for (int i = 0; i < n; i++) {
        if (vertices[i].origem == NULL)
            vazios++;
    }

    ordenarDestinos(vertices, n-vazios);
    free_registro(r_buffer);

    return vertices;
}