#include <string.h>
#include <stdio.h>
#include "arquivos/tipos.h"
#include "arvoreb/tipos.h"
#include "arvoreb/defines.h"


int arBBuscaBinaria(ArBNo *no, char *c, int *indice_retorno);
StatusDeRetorno arBBuscaRecursiva(FILE* i, char *c, int *r, int RRN_busca, ArBNo *no);
StatusDeRetorno arBBusca(FILE* indices, char *chave, int *resultado_RRN);