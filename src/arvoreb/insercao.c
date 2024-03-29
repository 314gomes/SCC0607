#include <string.h>
#include <stdlib.h>
#include "arvoreb/insercao.h"
#include "arvoreb/busca.h"
#include "arvoreb/defines.h"
#include "arvoreb/leitura.h"
#include "arvoreb/escrita.h"
#include "arvoreb/utils.h"

#define PROMOCAO 1
#define SEM_PROMOCAO 0

/// @brief Inserir nova chave de forma ordenada em um no de arvore b
/// @param novo_cv Nova chave a ser inserida
/// @param novo_RRN Filho direito da nova chave
/// @param no No no qual realizar a insercao
void arBInsereEmNoOrdenado(ArBChaveValor *novo_cv, int novo_RRN, ArBNo *no){
	// variaveis locais de comparacao da busca binaria e indice no qual inserir
	int cmp, indice_novo_cv;
	
	// Determinar posicao de insercao.
	cmp = arBBuscaBinaria(no, novo_cv->chave, &indice_novo_cv);
	// caso filho direito
	if(cmp > 0){
		indice_novo_cv++;
	}
	
	// shiftar elementos a direita da posicao desejada 1 a direita
	for(int i = no->nroChavesNo; i > indice_novo_cv;  i--){
		no->chaveValor[i] = no->chaveValor[i - 1];
		no->RRNFilho[i + 1] = no->RRNFilho[i];
	}

	// inserir nova chave e novo filho na posicao correta
	no->chaveValor[indice_novo_cv] = *novo_cv;
	no->RRNFilho[indice_novo_cv + 1] = novo_RRN;
	
	// atualizar nro chaves
	no->nroChavesNo++;
}

/// @brief Subrotina de split 1 para 2. Divide o mais igualmente possivel os
/// valores do no antigo e nova chave inserida entre dois nos.
/// @param f Arquivo de arvore B sobre o qual realizar o split.
/// @param novo_cv Nova chave a ser inserida.
/// @param novo_RRN RRN do filho direito dessa chave.
/// @param no_antigo No ja existente na arvoreb.
/// @param cv_promo ponteiro de chave que sera promovida na rotina de split.
/// @param RRN_promo ponteiro para inteiro do filho direito da chave promovida. 
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
	
	// determinar indice no qual deve ser inserida a nova chave
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
	*RRN_promo = novo_no.RRNdoNo;

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

/// @brief Funcao recursiva de insercao na arvore B.
/// @param index FILE* do arquivo arvore B.
/// @param RRN_atual RRN atual no qual tentar a insercao.
/// @param cv_novo Nova chave a ser inserida.
/// @param cv_promo ponteiro para a chave que pode ser promovida pela insercao.
/// @param RRN_promo ponteiro de int para o filho direito da chave promovida.
/// @param altura_promo ponteiro para int da altura que pode ser promovida.
/// @return retorna PROMOCAO se precisou promover chave, SEM_PROMOCAO caso
/// contrario.
int arBInsereRecursiva(FILE *index, int RRN_atual, ArBChaveValor *cv_novo, ArBChaveValor *cv_promo, int *RRN_promo, int *altura_promo){
	// No atual guardado em memoria primaria
	ArBNo no_atual;
	// proximo indice de insercao a ser determinado usando busca binaria
	int rrn_prox_insercao, indice_rrn_prox_insercao, cmp;
	// chave recebida como promocao na insercao inferior
	ArBChaveValor cv_promo_recebido;
	// rrn do filho direito da chave recebida como promocao
	int rrn_promo_recebido;
	// valor de retorno da chamada recursiva
	int valor_retorno;

	// Atingiu fim da arvore, retornar promocao.
	if(RRN_atual == ARB_RRN_NULL){
		*cv_promo = *cv_novo;
		*RRN_promo = ARB_RRN_NULL;
		if(altura_promo != NULL){
			*altura_promo = 1;
		}
		return PROMOCAO;
	}
	// senao...

	// posiciona stream no rrn da pagina atual
	long off_atual = arBByteoffsetRRN(RRN_atual);
	fseek(index, off_atual, SEEK_SET);
	// le chave atual para a variavel local
	ArBLeNo(index, &no_atual);

	// buscar onde a proxima insercao recursiva deve ser
	cmp = arBBuscaBinaria(&no_atual, cv_novo->chave, &indice_rrn_prox_insercao);
	// se ultima comparacao indicou que deve buscar a direita
	if(cmp > 0){ 
		indice_rrn_prox_insercao++;
	}
	rrn_prox_insercao = no_atual.RRNFilho[indice_rrn_prox_insercao];

	// chamada recursiva da insercao
	valor_retorno = arBInsereRecursiva(
		index,
		rrn_prox_insercao,
		cv_novo,
		&cv_promo_recebido,
		&rrn_promo_recebido,
		NULL
	);

	// encerrar em caso de nao ter promocao
	if(valor_retorno == SEM_PROMOCAO){
		return valor_retorno;
	}

	// se tiver espaco no no atual para a nova chave
	if(no_atual.nroChavesNo < ARB_ORDEM - 1){
		// inserir no no armazenado em memoria primaria
		arBInsereEmNoOrdenado(&cv_promo_recebido, rrn_promo_recebido, &no_atual);
		// atualizar no disco
		arBEscreveNo(index, &no_atual);
		// retornar valor correto
		return SEM_PROMOCAO;
	}
	else{
		// utilizar subrotina de split e promover nova chave.
		arBSplit(
			index,
			&cv_promo_recebido,
			rrn_promo_recebido,
			&no_atual,
			cv_promo,
			RRN_promo
		);
		if(altura_promo != NULL){
			*altura_promo = no_atual.alturaNo + 1;
		}
		return PROMOCAO;
	}

}

/// @brief Inserir chave e valor na arvore B com cabecalho ja existente.
/// @param index arquivo de indices arvore B de acordo com especificacao
/// @param cv tipo ArBChaveValor a ser inserido
void arBInsere(FILE *index, ArBChaveValor *cv){
	// Variavel para guardar proximo RRN disponivel para insercao de novo no
	int prox_RRN;
	// Variavel para guardar o RRN do no raiz
	int raiz_RRN;
	// Variavel do status de retorno da insercao, caso haja overflow ate a raiz
	int resultado_insercao;
	// Variavel para o RRN que pode ser promovido pela insercao
	int RRN_promo;
	// Variavel para a chave que pode ser promovida pela insercao
	ArBChaveValor cv_promo;
	// Altura da nova raiz, determinada pela funcao de isnercao recursiva
	int altura_promo;
	// Variavel para possivel novo no a ser criado em caso de overflow na raiz
	ArBNo nova_raiz;

	// certificar que a posicao esta correta para leitura dos parametros
	fseek(index, ARB_POS_NO_RAIZ, SEEK_SET);
	// Ler rrn da raiz e proximo rrn disponivel para escrita
	fread(&raiz_RRN, sizeof(int), 1, index);
	fread(&prox_RRN, sizeof(int), 1, index);

	// toda insercao comeca pelo no raiz
	resultado_insercao = arBInsereRecursiva(index, raiz_RRN, cv, &cv_promo, &RRN_promo, &altura_promo);
	
	// em caso de promocao, preencher nova raiz e escreve-la
	if(resultado_insercao == PROMOCAO){
		// Ler proximo rrn disponivel para escrita novamente
		fseek(index, ARB_POS_PROX_RRN, SEEK_SET);
		fread(&prox_RRN, sizeof(int), 1, index);
		// nova raiz, incialmente com apenas 1 no
		nova_raiz.nroChavesNo = 1;
		// rrn da nova raiz e o proximo rrn disponivel
		nova_raiz.RRNdoNo = prox_RRN;
		// altura conseguida na funcao de insercao recursiva
		nova_raiz.alturaNo = altura_promo;
		// inserir chave promovida para nova raiz
		nova_raiz.chaveValor[0] = cv_promo;
		// raiz antiga como filho esquerdo da nova chave 
		nova_raiz.RRNFilho[0] = raiz_RRN;
		// rrn promovido como filho direito da nova chave
		nova_raiz.RRNFilho[1] = RRN_promo;
		
		// escrever nova raiz
		arBEscreveNo(index, &nova_raiz);
		
		// atualizar valor rrn da raiz
		raiz_RRN = prox_RRN;
		// atualizar valor proximo rrn
		prox_RRN++;
	
		// certificar que a posicao esta correta para escrita dos valores
		fseek(index, ARB_POS_NO_RAIZ, SEEK_SET);
		// sobrescrever RRN da raiz e do proximo disponivel.
		fwrite(&raiz_RRN, sizeof(int), 1, index);
		fwrite(&prox_RRN, sizeof(int), 1, index);
	}
}