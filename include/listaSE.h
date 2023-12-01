/**
 * Grupo 8
 * João Pedro Gomes - 13839069
 * Luana Hartmann Franco da Cruz - 13676350
*/
#include "arquivos/defines.h"

typedef struct no{
	char str[TAM_MAXIMO_STRING];
	struct no *prox;
} No;

typedef struct {
	No *inicio;
	int tam;
} listaSE;

listaSE novaLista();

No *novoNo(char *str);

void insereNoPosicao(listaSE *l, char *str, No *p, No *n);

void insereOrdenadoSemRepeticao(char *str, listaSE *l);

void destroiNo(No *n);

void destroiLista(listaSE *l);