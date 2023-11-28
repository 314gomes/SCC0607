/**
 * Grupo 8
 * João Pedro Gomes - 13839069
 * Luana Hartmann Franco da Cruz - 13676350
*/

#include "arquivos/defines.h"

#ifndef ARB_DEFINES_INCLUDED

#define ARB_DEFINES_INCLUDED

#define ARB_TAM_PAGINA 205
#define ARB_TAM_CABECALHO (sizeof(char) + 2 * sizeof(int))
#define ARB_TAM_LIXO_CABECALHO (ARB_TAM_PAGINA - ARB_TAM_CABECALHO)
#define ARB_ORDEM 4
#define ARB_TAM_CHAVE 55

#define ARB_RRN_NULL -1

// posicao do inteiro que guarda proximo RRN disponivel no arquivo da arvore b
#define ARB_POS_PROX_RRN 5
// posicao do inteiro que guarda RRN do no raiz no arquivo da arvore b
#define ARB_POS_NO_RAIZ 1

// Indice da chave que sera promovida na subrotina de split
#define ARB_INDICE_SPLIT (ARB_ORDEM / 2)

#endif