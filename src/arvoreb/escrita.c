#include "arvoreb/escrita.h"
#include "arvoreb/defines.h"

/// @brief Escrever cabecalho c na arvore B f de acordo com especificacao
/// @param f arquivo arvore B
/// @param c Cabecalho a ser escrito
void arBEscreveCabecalho(FILE *f, ArBCabecalho *c){
	fwrite(&c->status, sizeof(char), 1, f);
	fwrite(&c->noRaiz, sizeof(char), 1, f);
	fwrite(&c->RRNProxNo, sizeof(char), 1, f);

	char lixo = LIXO;
	for(int i = 0; i < ARB_TAM_LIXO_CABECALHO; i++){
		fwrite(&lixo, sizeof(char), 1, f);
	}
}