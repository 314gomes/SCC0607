#include "arvoreb/defines.h"

#ifndef AR_B_TIPOS_INCLUDED

#define AR_B_TIPOS_INCLUDED

typedef struct {
    char status;
    int noRaiz;
    int RRNProxNo;
} ArBCabecalho;

typedef struct {
	int nroChavesNo;
	int alturaNo;
	int RRNdoNo;

	int RRNFilho[ARB_ORDEM];
	int RRNArquivoDados[ARB_ORDEM - 1];
	char chave[ARB_ORDEM - 1][ARB_TAM_CHAVE + 1];
} ArBNo;

#endif