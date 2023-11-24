#include "arvoreb/insercao.h"

#define PROMOCAO 1
#define SEM_PROMOCAO 0

int arBSplit(ArBChaveValor novo_cv, int novo_RRN, ArBNo *no_atual, ArBChaveValor *cv_promo, int *RRN_promo, ArBNo *novo_no){

}

int arBInsereRecursiva(FILE *index, int RRN_atual, ArBChaveValor *cv, ArBChaveValor *cv_promo, int *RRN_promo){

}

/// @brief Inserir chave e valor na arvore B com cabecalho ja existente.
/// @param index arquivo de indices arvore B de acordo com especificacao
/// @param cv tipo ArBChaveValor a ser inserido
void arBInsere(FILE *index, ArBChaveValor *cv){
	int RRNProxNo;
	int noRaiz;
	// certificar que a posicao esta correta para leitura dos parametros
	fseek(index, 1, SEEK_SET);
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