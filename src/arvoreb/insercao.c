#include <string.h>
#include <stdlib.h>
#include "arvoreb/insercao.h"
#include "arvoreb/busca.h"
#include "arvoreb/defines.h"
#include "arvoreb/leitura.h"
#include "arvoreb/escrita.h"

#define PROMOCAO 1
#define SEM_PROMOCAO 0

// void arBInsereEmNoOrdenado(ArBChaveValor novo_cv, int novo_RRN, ArBNo *no){
	
// }

void arBSplit(FILE* f, ArBChaveValor *novo_cv, int novo_RRN, ArBNo *no_antigo, ArBChaveValor *cv_promo, int *RRN_promo){
	// "pagina" de trabalho que comporta uma chave e um rrn filho a mais
	ArBChaveValor cv_trabalho[ARB_ORDEM];
	int rrn_filho_trabalho[ARB_ORDEM + 1];
	
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
	
	// determinar indice no qual deve ser inserido a nova chave
	int indice_nova_chave, cmp;
	cmp = arBBuscaBinaria(no_antigo, novo_cv->chave, &indice_nova_chave);
	if(cmp > 0){
		indice_nova_chave++;
	}

	// O proximo bloco preenche a pagina de trabalho

	// indice de leitura do no atual
	int indice_no_atual = 0;
	// o filho mais a esquerda permanece identico
	rrn_filho_trabalho[0] = no_antigo->RRNFilho[0];
	for(int i = 0; i < ARB_ORDEM; i ++){
		// selecionar de onde vira a chave e o rrn filho e armazenar na pagina
		// de trabalho
		if(i == indice_nova_chave){ // atingiu posicao de onde o novo no deve ir
			cv_trabalho[i] = *novo_cv;
			rrn_filho_trabalho[i + 1] = novo_RRN;
		}
		else{ // inserir do no antigo
			cv_trabalho[i] = no_antigo->chaveValor[indice_no_atual];
			rrn_filho_trabalho[i + 1] = no_antigo->RRNFilho[indice_no_atual + 1];
			indice_no_atual++;
		}
	}

	// Preencher no antigo com novos dados
	no_antigo->nroChavesNo = ARB_INDICE_SPLIT;
	no_antigo->RRNFilho[0] = rrn_filho_trabalho[0];
	for(int i = 0; i < ARB_INDICE_SPLIT; i++){
		no_antigo->chaveValor[i] = cv_trabalho[i];
		no_antigo->RRNFilho[i + 1] = rrn_filho_trabalho[i + 1];
		}

	// Promover chave correta
	*cv_promo = cv_trabalho[ARB_INDICE_SPLIT];
	RRN_promo = novo_no.RRNdoNo;

	// Preencher listas do no novo
	novo_no.nroChavesNo = (ARB_ORDEM - ARB_INDICE_SPLIT - 1);
	novo_no.RRNFilho[0] = rrn_filho_trabalho[ARB_INDICE_SPLIT + 1];
	for(int i = 0; i < novo_no.nroChavesNo; i++){
		novo_no.chaveValor[i] = cv_trabalho[i + ARB_INDICE_SPLIT + 1];
		novo_no.RRNFilho[i + 1] = rrn_filho_trabalho[i + ARB_INDICE_SPLIT + 2];
		}

	// Sobrescrever o proximo RRN disponivel
	int prox_rrn = novo_no.RRNdoNo;
	prox_rrn++;
	fseek(f, ARB_POS_PROX_RRN, SEEK_SET);
	fwrite(&prox_rrn, sizeof(int), 1, f);

	// Sobrescrever paginas atualizadas
	arBEscreveNo(f, no_antigo);
	arBEscreveNo(f, &novo_no);
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