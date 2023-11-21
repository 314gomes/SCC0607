#include <stdio.h>
#include "arvoreb/tipos.h"
#include "arvoreb/defines.h"

void ArBLeNo(FILE* f, ArBNo* no){
	fread(&no->nroChavesNo, sizeof(int), 1, f);
	fread(&no->alturaNo, sizeof(int), 1, f);
	fread(&no->RRNdoNo, sizeof(int), 1, f);
	
	fread(&no->RRNFilho[0], sizeof(int), 1, f);
	for(int i = 1; i < ARB_ORDEM; i++){
		fread(&no->chave[i], sizeof(char), ARB_TAM_CHAVE, f);
		// finalizar a string com terminador por seguranca
		no->chave[i][ARB_TAM_CHAVE + 1] = '\0';

		fread(&no->RRNArquivoDados[i], sizeof(int), 1, f);
		
		fread(&no->RRNFilho[i + 1], sizeof(int), 1, f);
	}
}