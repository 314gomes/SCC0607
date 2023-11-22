#include <string.h>
#include <stdio.h>
#include "arquivos/tipos.h"
#include "arvoreb/tipos.h"
#include "arvoreb/defines.h"
#include "arvoreb/utils.h"
#include "arvoreb/leitura.h"


StatusDeRetorno arBBuscaBinaria(ArBNo *no, char *c, int *RRN_busca, int *RRN_retorno){
	int ini = 0;
	int fim = no->nroChavesNo - 1;
	int meio, cmp;

	while (ini <= fim){
		meio = ini + (fim - ini)/2;
		cmp = strcmp(c, no->chaveValor[meio].chave);

		if(cmp == 0){ // encontrada a chave, retornar
			*RRN_retorno = no->chaveValor[meio].RRNArquivoDados;
			return sucesso;
		}
		else if(cmp < 0){
			fim = meio - 1;
		}
		else if(cmp > 0){
			ini = meio + 1;
		}
	}

	// se no fim do laco nao foi encontrada resposta, e necessario determinar
	// qual vai ser o proximo rrn de busca dentro da propria arvore b.
	if(cmp < 0){
		*RRN_busca = no->RRNFilho[meio];
	}
	else{
		*RRN_busca = no->RRNFilho[meio + 1];
	}

	return registro_inexistente;

}

StatusDeRetorno arBBuscaRecursiva(FILE* i, char *c, int *r, int RRN_busca, ArBNo *no){
	//carregar pagina para memoria primaria
	long byteoffset = arBByteoffsetRRN(RRN_busca);
	fseek(i, byteoffset, SEEK_SET);
	ArBLeNo(i, no);
	
	//busca binaria nos nos
	int RRN_prox_busca;
	StatusDeRetorno s;
	s = arBBuscaBinaria(no, c, &RRN_prox_busca, r);
	
	if(s == sucesso){ // indica que busca achou um match perfeito
		return s;
	}

	// caso contrario, busca recursiva com o RRN da proxima busca, determinado
	// pela busca binaria. Porem, caso RRN da proxima busca seja -1, chegou ao
	// fim da arvore sem encontrar o match.
	if(RRN_prox_busca < 0){
		return registro_inexistente;
	}
	else{
		return arBBuscaRecursiva(i, c, r, RRN_prox_busca, no);
	}

}

/// @brief ler rrn raiz, iniciar busca recursiva na raiz
/// @param indices 
/// @param chave 
/// @param resultado_RRN 
/// @return 
StatusDeRetorno arBBusca(FILE* indices, char *chave, int *resultado_RRN){
	// deixar apenas um no em memoria primaria e passa-lo por referencia para
	// a funcao recursiva
	ArBNo noDeBusca;

	int RRN_no_raiz;
	StatusDeRetorno s;	

	// Ler RRN do no raiz, que esta sempre no byte 1 do arquivo de indices
	// segundo especificacao
	fseek(indices, 1, SEEK_SET);
	fread(&RRN_no_raiz, sizeof(int), 1, indices);
	
	// Iniciar busca recursiva pela raiz
	s = arBBuscaRecursiva(indices, chave, resultado_RRN, RRN_no_raiz, &noDeBusca);
	return s;
}