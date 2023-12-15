#include <stdio.h>
#include <stdlib.h>

#include "grafos/pilha.h"


void inicializaPilha(Pilha *pilha) {
    pilha->topo = -1;
}

int estaVazia(Pilha *pilha) {
    return pilha->topo == -1;
}

int estaCheia(Pilha *pilha) {
    return pilha->topo == MAX_SIZE - 1;
}

void empilha(Pilha *pilha, int elemento) {
    if (!estaCheia(pilha)) {
        pilha->itens[++pilha->topo] = elemento;
    } else {
        printf("Erro: Pilha cheia\n");
    }
}

int desempilha(Pilha *pilha) {
    if (!estaVazia(pilha)) {
        return pilha->itens[pilha->topo--];
    } else {
        printf("Erro: Pilha vazia\n");
        return -1; // Valor de erro
    }
}

int topo(Pilha *pilha) {
    if (!estaVazia(pilha)) {
        return pilha->itens[pilha->topo];
    } else {
        printf("Erro: Pilha vazia\n");
        return -1; // Valor de erro
    }
}