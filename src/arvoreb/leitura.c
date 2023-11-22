#include <stdio.h>
#include "arvoreb/tipos.h"
#include "arvoreb/defines.h"

void ArBLeChave(FILE *f, char* c){
	char in; // caractere de leitura atual
	for(int i = 0; i < ARB_TAM_CHAVE; i++){
		fread(&in, sizeof(char), 1, f);
		
		if(in == '$'){
			c[i] = '\0';
		}
		else{
			c[i] = in;
		}
	}
}

void ArBLeNo(FILE* f, ArBNo* no){
	fread(&no->nroChavesNo, sizeof(int), 1, f);
	fread(&no->alturaNo, sizeof(int), 1, f);
	fread(&no->RRNdoNo, sizeof(int), 1, f);
	
	fread(&no->RRNFilho[0], sizeof(int), 1, f);
	for(int i = 0; i < ARB_ORDEM - 1; i++){
		// ler corretamente ate lixo
		ArBLeChave(f, no->chaveValor[i].chave);
		
		fread(&no->chaveValor[i].RRNArquivoDados, sizeof(int), 1, f);
		
		fread(&no->RRNFilho[i + 1], sizeof(int), 1, f);
	}
}