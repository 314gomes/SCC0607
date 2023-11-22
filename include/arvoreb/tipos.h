#include "arvoreb/defines.h"

#ifndef AR_B_TIPOS_INCLUDED

#define AR_B_TIPOS_INCLUDED

typedef struct {
    char status;
    int noRaiz;
    int RRNProxNo;
} ArBCabecalho;

typedef struct {
	int RRNArquivoDados;
	char chave[ARB_TAM_CHAVE];
} ArBChaveValor;

typedef struct {
	int nroChavesNo;
	int alturaNo;
	int RRNdoNo;

	int RRNFilho[ARB_ORDEM];
	ArBChaveValor chaveValor[ARB_ORDEM - 1];
} ArBNo;

#endif