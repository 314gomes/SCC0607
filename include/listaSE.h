#include "defines.h"

typedef struct no{
	char str[TAM_MAXIMO_STRING];
	struct no *prox;
} No;

typedef struct {
	No *inicio;
	int tam;
} listaSE;

/// @brief Initialize new list
/// @return initialized list
listaSE novaLista();

/// @brief Allocates memory for new node with `str` content
/// @param str content for new node
/// @return 
No *novoNo(char *str);

/// @brief Crates new node with string `str` before `n` on list `l`,
/// automatically updating l->incio and l->tam
/// @param l list to write
/// @param p node to be inserted before next
/// @param n next node already on list
void insereNoPosicao(listaSE *l, char *str, No *n);

/// @brief Inserts node with `str` content in `l` in reverse alphabetical order
/// (assumes `l` is already in reverse alphabetical order)
/// @param str node data to be inserted to `l`
/// @param l list to insert into
void insereOrdenadoSemRepeticao(char *str, listaSE *l);

/// @brief Free `n` memory
/// @param n node pointer to be freed
void destroiNo(No *n);

/// @brief free `l` memory
/// @param l list to free memory
void destroiLista(listaSE *l);