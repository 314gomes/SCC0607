#include <string.h>
#include <stdio.h>
#include "arquivos/tipos.h"
#include "arvoreb/tipos.h"
#include "arvoreb/defines.h"
#include "arvoreb/utils.h"
#include "arvoreb/leitura.h"


/// @brief Funcao para determinar onde uma chave c da arvore B deve ser inserida
/// considerando determinado no.
/// @param no No a ser considerado na busca binaria.
/// @param c Chave de busca
/// @param indice_retorno ponteiro para inteiro a ser sobrescrito com o indice
/// encontrado na busca
/// @return Resultado da ultima comparacao. Caso seja negativo,a posicao da
/// chave e no filho esquerdo de  indice_retorno. Caso seja 0, foi encontrado um
/// match perfeito. Caso positivo, a posicao da chave e no filho direito de
/// indice_retorno.
int arBBuscaBinaria(ArBNo *no, char *c, int *indice_retorno){
	int ini = 0;
	int fim = no->nroChavesNo - 1;
	int meio, cmp;

	while (ini <= fim){
		meio = ini + (fim - ini)/2;
		cmp = strcmp(c, no->chaveValor[meio].chave);

		if(cmp == 0){ // encontrada a chave, encerrar
			break;
		}
		else if(cmp < 0){
			fim = meio - 1;
		}
		else if(cmp > 0){
			ini = meio + 1;
		}
	}

	// no fim do laco, escrever o indice de retorno enocntrado
	*indice_retorno = meio;
	return cmp;

}

StatusDeRetorno arBBuscaRecursiva(FILE* i, char *c, int *r, int RRN_busca, ArBNo *no){
	//carregar pagina para memoria primaria
	long byteoffset = arBByteoffsetRRN(RRN_busca);
	fseek(i, byteoffset, SEEK_SET);
	ArBLeNo(i, no);
	
	//busca binaria nos nos
	int indice_encontrado;
	int cmp = arBBuscaBinaria(no, c, &indice_encontrado);
	
	// se houve match perfeito, pode retornar sucesso
	if(cmp == 0){ // indica que busca achou um match perfeito
		*r = no->chaveValor[indice_encontrado].RRNArquivoDados;
		return sucesso;
	}

	// caso contrario, busca recursiva com o RRN da proxima busca, determinado
	// pela busca binaria. Porem, caso RRN da proxima busca seja -1, chegou ao
	// fim da arvore sem encontrar o match.
	int RRN_prox_busca;
	// proxima busca no filho esquerdo
	if(cmp < 0){
		RRN_prox_busca = no->RRNFilho[indice_encontrado];
	}
	// proxima busca no filho direito
	else{
		RRN_prox_busca = no->RRNFilho[indice_encontrado + 1];
	}

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