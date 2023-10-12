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
	
	return n;
}

void insereNoPosicao(listaSE *l, char *str, No *p, No *n){
	No *new = novoNo(str);
	new->prox = n;
	
	if(p == NULL){
		l->inicio = new;
	}
	else{
		p->prox = new;
	}

	l->tam++;
}

void insereOrdenadoSemRepeticao(char *str, listaSE *l){
	No *atual = l->inicio;
	No *prev = NULL;

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
			
			insereNoPosicao(l, str, prev, atual);
			return;
		}

		prev = atual;
		atual = atual->prox;
	}

	// Could not find previous matches: place at end
	insereNoPosicao(l, str, prev, atual);
	
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