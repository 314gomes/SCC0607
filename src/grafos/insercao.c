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

int compararArestas(const void *a, const void *b) {
    const Aresta *aresta1 = (const Aresta *)a;
    const Aresta *aresta2 = (const Aresta *)b;
    return strcmp(aresta1->origem, aresta2->origem);
}

void ordenarArestas(Vertice *vertice) {
    if (vertice->num_arestas > 0) {
        qsort(vertice->arestas, vertice->num_arestas, sizeof(Aresta), compararArestas);
    }
}

int compararDestinos(const void *a, const void *b) {
    const Vertice *vertice1 = (const Vertice *)a;
    const Vertice *vertice2 = (const Vertice *)b;
    return strcmp(vertice1->destino, vertice2->destino);
}

void ordenarDestinos(Vertice *vertices, int n) {
    qsort(vertices, n, sizeof(Vertice), compararDestinos);
}

void atualizaVertice (int index, Vertice *vertices, Registro* r_buffer, char* origem, char* destino, int n, int flag, int bool) {

    if (index != -1) {
        if (flag == 0) {
            vertices[index].grau_entrada++;
            int num_arestas = vertices[index].num_arestas;
            vertices[index].arestas[num_arestas].origem = strdup(origem);
            vertices[index].arestas[num_arestas].peso = r_buffer->peso;
            vertices[index].num_arestas++;

            if (bool == FALSE)
                vertices[index].grupo = r_buffer->grupo;
 
        }      
        else {
            vertices[index].grau_saida++;
            if (bool == TRUE)
                vertices[index].grupo = r_buffer->grupo;
        }
            
        vertices[index].grau_total++;

        ordenarArestas(&vertices[index]);
    }
    else {
        int free_index = -1;
        for (int i = 0; i < n; ++i) {
            if (vertices[i].destino == NULL) {
                free_index = i;
                break;
            }
        }

        if (free_index != -1) {
            // Utilize o vértice livre
            if (flag == 0) {
                vertices[free_index].destino = strdup(destino);                 
                vertices[free_index].grau_entrada = 1;
                vertices[free_index].num_arestas = 1;
                vertices[free_index].arestas[0].origem = strdup(origem);
                vertices[free_index].arestas[0].peso = r_buffer->peso;

                if (bool == FALSE)
                    vertices[free_index].grupo = r_buffer->grupo;
            }
            else {     
                vertices[free_index].destino = strdup(origem);
                vertices[free_index].grau_saida = 1;
                if (bool == TRUE)
                    vertices[free_index].grupo = r_buffer->grupo;
            }
            
            vertices[free_index].grau_total = 1;
            
        }

        ordenarArestas(&vertices[n-1]);
    }
}

void insereGrafo (Vertice *vertices, Registro *r_buffer, int n) {

    char *charOrigem = r_buffer->tecnologiaOrigem.string;
    char *charDestino = r_buffer->tecnologiaDestino.string;

    int iDestino = verificaVertices(vertices, charDestino, n);
    int iOrigem = verificaVertices(vertices, charOrigem, n);

    atualizaVertice (iDestino, vertices, r_buffer, charOrigem, charDestino, n, 0, TRUE);
    atualizaVertice (iOrigem, vertices, r_buffer, charOrigem, charDestino, n, 1, TRUE);
}

void insereGrafoTransposto (Vertice *vertices, Registro *r_buffer, int n) {
    char *charOrigem = r_buffer->tecnologiaOrigem.string;
    char *charDestino = r_buffer->tecnologiaDestino.string;

    int iDestino = verificaVertices(vertices, charDestino, n);
    int iOrigem = verificaVertices(vertices, charOrigem, n);

    atualizaVertice (iOrigem, vertices, r_buffer, charDestino, charOrigem, n, 0, FALSE);
    atualizaVertice (iDestino, vertices, r_buffer, charDestino, charOrigem, n, 1, FALSE);
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

    char auxiliarChar;
    Registro *r_buffer = novo_registro();

    while(fread(&(auxiliarChar), sizeof(char), 1, bin) != 0) {
        
        if (auxiliarChar == NAO_REMOVIDO) {
            leConteudoRegistro(bin, r_buffer);
            if (r_buffer->tecnologiaOrigem.tamanho != 0) {
                if (r_buffer->tecnologiaDestino.tamanho != 0) 
                    if (r_buffer->grupo != -1){
                        if (bool == TRUE)
                            insereGrafo(vertices, r_buffer, n);
                        else
                            insereGrafoTransposto(vertices, r_buffer, n);
                    }
                    
            }

        }
            
    }
    //printf("n %d\n\n", n);
    ordenarDestinos(vertices, n);
    free_registro(r_buffer);

    return vertices;
}