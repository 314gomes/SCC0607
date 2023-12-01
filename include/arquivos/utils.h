#include <stdio.h>
#include "arquivos/tipos.h"

Registro *novo_registro();

void free_registro(Registro *r);

Cabecalho *novo_cabecalho();

int calcularTamanhoRegistro(Registro *r);

FILE *abreBinario(char *caminhobin, char *modo);

long byteoffset_RRN(int RRN);

int campoDeBuscaEString(char* nomeCampo);