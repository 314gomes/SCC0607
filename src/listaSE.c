/**
 * Grupo 8
 * João Pedro Gomes - 13839069
 * Luana Hartmann Franco da Cruz - 13676350
*/
#include "listaSE.h"
#include "stdlib.h"
#include "string.h"

/// @brief Inicializa nova lista
/// @return lista incializada
listaSE novaLista(){
	listaSE n;
	n.inicio = NULL;
	n.tam = 0;
	return n;
}

/// @brief Aloca memoria para novo no contendo `str`
/// @param str conteudo do novo no
/// @return ponteiro para novo no
No *novoNo(char *str){
	No *n = NULL;
	n = malloc(sizeof(No));
	strcpy(n->str, str);
	
	return n;
}

/// @brief Cria novo no entre os nos `p` (previous) e `n` (next) contendo `str`,
/// atualizando automaticamente l->incio e l->tam
/// @param l lista na qual inserir
/// @param str conteudo do novo no
/// @param p ponteiro para no anterior
/// @param n ponteiro para proximo no
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

/// @brief Cria novo no contendo `str` e insere em `l` mantendo ordenacao e
/// evitando repeticoes.
/// @param str string do novo no
/// @param l lista na qual inserir
void insereOrdenadoSemRepeticao(char *str, listaSE *l){
	No *atual = l->inicio;
	No *prev = NULL;

	// percorrer lista
	while(atual != NULL){
		// comparar string de insercao com o no atual
		int cmp = strcmp (str, atual->str);

		// caso strings iguais
		if(cmp == 0){
			// retornar sem insercao
			return;
		}
		// str < n em ordem alfabetica
		else if(cmp > 0){
			// insere em ordem alfabetica			
			insereNoPosicao(l, str, prev, atual);
			return;
		}

		// avancar nos nos
		prev = atual;
		atual = atual->prox;
	}

	// nao achou outra posicao de insercao. inserir no final.
	insereNoPosicao(l, str, prev, atual);
	
}

/// @brief Libera memoria do no `n`
/// @param n no a ser liberado
void destroiNo(No *n){
	free(n);
}

/// @brief Libera memoria da lista `l` (e de todos seus nos)
/// @param l lista a ser liberada
void destroiLista(listaSE *l){
	No* prox_no;
	// inciar de l->inicio
	No *atual = l->inicio;

	while(atual != NULL){
		// salvar proximo no temporariamente
		prox_no = atual->prox;

		// liberar memoria do no
		destroiNo(atual);

		// avancar no no
		atual = prox_no;

	}
}