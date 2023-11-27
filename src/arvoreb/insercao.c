#include <string.h>
#include <stdlib.h>
#include "arvoreb/insercao.h"
#include "arvoreb/busca.h"
#include "arvoreb/defines.h"

#define PROMOCAO 1
#define SEM_PROMOCAO 0

void arBAppendNo(ArBChaveValor *novo_cv, int novo_RRN, ArBNo *no){
	// determinar indice de insercao no final do no
	int indice_insercao = no->nroChavesNo;

	// inserir nova chave
	char *c = no->chaveValor[indice_insercao].chave;
	strcpy(c, novo_cv->chave);

	// inserir novo rrn filho
	no->chaveValor->chave[indice_insercao] = novo_cv->RRNArquivoDados;

	// inserir novo RRN a direita do novo no
	no->RRNFilho[indice_insercao + 1] = novo_RRN;
}

// void arBInsereEmNoOrdenado(ArBChaveValor novo_cv, int novo_RRN, ArBNo *no){
	
// }

void arBSplit(FILE* f, ArBChaveValor *novo_cv, int novo_RRN, ArBNo *no_antigo, ArBChaveValor *cv_promo, int *RRN_promo){
	// variavel local com as informacoes do novo no
	ArBNo novo_no;
	
	// no comeca vazio
	novo_no.nroChavesNo = 0;
	
	// novo no tem mesma altura do no atual
	novo_no.alturaNo = no_antigo->alturaNo;
	
	// posicionar stream para ler proximo rrn disponivel
	fseek(f, ARB_POS_PROX_RRN, SEEK_SET);
	// RRN do no e o proximo disponivel na arvore, recuperado pelo cabecalho
	fread(&novo_no.RRNdoNo, sizeof(int), 1, f);
	
	// determinar indice no qual deve ser inserido 
	int indice_novo_no, cmp;
	cmp = arBBuscaBinaria(no_antigo, novo_cv->chave, &indice_novo_no);
	if(cmp > 0){
		indice_novo_no++;
	}

	// COMENTAR MELHOR QUANDO ACABAR	
	int indice_no_atual = 0;
	ArBChaveValor *cv_atual;
	int RRN_atual;

	// Inserir no mais a esquerda na posicao correta

	// Executar numero de chaves por no + 1 vezes
	for(int i = 0; i < ARB_ORDEM; i ++){
		// selecionar de onde vira a chave e o rrn filho
		if(i == indice_novo_no){ // atingiu posicao de onde o novo no deve ir
			cv_atual = novo_cv;
			RRN_atual = novo_RRN;
		}
		else{ // inserir do no antigo
			cv_atual = &no_antigo->chaveValor[indice_no_atual];
			RRN_atual = no_antigo->RRNFilho[indice_no_atual + 1];
			indice_no_atual++;
		}

		// escolher entre inserir no no antigo, promover ou inserir no novo no
		if(i < ARB_INDICE_SPLIT + 1){
			no_antigo->chaveValor[i] = *cv_atual;
			no_antigo->RRNFilho[i + 1] = RRN_atual;
		}
		else if(i == ARB_INDICE_SPLIT + 1){
			*cv_promo = *cv_atual;
			*RRN_promo = RRN_atual; 
		}
		else{
			arBAppendNo(cv_atual, RRN_atual, &novo_no);
		}
	}

}

// int arBInsereRecursiva(FILE *index, int RRN_atual, ArBChaveValor *cv, ArBChaveValor *cv_promo, int *RRN_promo){

// }

/// @brief Inserir chave e valor na arvore B com cabecalho ja existente.
/// @param index arquivo de indices arvore B de acordo com especificacao
/// @param cv tipo ArBChaveValor a ser inserido
void arBInsere(FILE *index, ArBChaveValor *cv){
	int RRNProxNo;
	int noRaiz;
	// certificar que a posicao esta correta para leitura dos parametros
	fseek(index, ARB_POS_NO_RAIZ, SEEK_SET);
	// toda insercao comeca lendo o cabecalho
	fread(&noRaiz, sizeof(int), 1, index);
	fread(&RRNProxNo, sizeof(int), 1, index);

	// toda insercao comeca pelo no raiz
	int resultado_insercao;
	ArBChaveValor *cv_promo;
	int *RRN_promo;
	resultado_insercao = arBInsereRecursiva(index, noRaiz, cv, cv_promo, RRN_promo);
	
	if(resultado_insercao == PROMOCAO){
		// create a new root page with key := PROMO_KEY, left child := ROOT and
		// right child := PROMO_R_CHILD
		// set ROOT to RRN of new root page
	}
}