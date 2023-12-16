#ifndef PILHA_H
#define PILHA_H

#define MAX_SIZE 120

typedef struct {
    int itens[MAX_SIZE];
    int topo;
} Pilha;

void inicializaPilha(Pilha *pilha);

int estaVazia(Pilha *pilha);

int estaCheia(Pilha *pilha);

void empilha(Pilha *pilha, int elemento);

int desempilha(Pilha *pilha);

int topo(Pilha *pilha);

#endif  // PILHA_H
