#include "listaSE.h"
#include "stdlib.h"
#include "string.h"

listaSE novaLista(){
	listaSE n;
	n.inicio = NULL;
	n.tam = 0;
	return n;
}

No *novoNo(char *str){
	No *n = NULL;
	n = malloc(sizeof(No));
	strcpy(n->str, str);
}

void insereNoPosicao(listaSE *l, char *str, No *n){
	No *p = novoNo(str);
	p->prox = n;
	
	if(n == l->inicio){
		l->inicio = p;
	}

	l->tam++;
}

void insereOrdenadoSemRepeticao(char *str, listaSE *l){
	No *atual = l->inicio;

	while(atual != NULL){
		int cmp = strcmp (str, atual->str);

		// equal strings
		if(cmp == 0){
			// no insertion
			return;
		}
		// str < l in alphabetical order
		else if(cmp > 0){
			// insert in reverse alphabetical order
			
			insereNoPosicao(l, str, atual);
			return;
		}

		atual = atual->prox;
	}

	// Could not find previous matches: place at end
	insereNoPosicao(l, str, atual);
}

void destroiNo(No *n){
	free(n);
}

void destroiLista(listaSE *l){
	No* tmp;
	// start from l->inicio
	No *atual = l->inicio;

	while(atual != NULL){
		// save needed content to temporary variable
		tmp = atual->prox;

		// free node memory
		destroiNo(atual);

		// jump to next node
		atual = tmp;

	}
}