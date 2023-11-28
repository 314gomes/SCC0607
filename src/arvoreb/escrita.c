#include <string.h>
#include "arvoreb/escrita.h"
#include "arvoreb/defines.h"
#include "arvoreb/utils.h"

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

/// @brief Escreve chave c na posicao atual do arquivo f.
/// @param f arquivo de arvore b
/// @param c chave a ser escrita
void arBEscreveChave(FILE *f, char *c){
	// determinar tamanho da chave
	int tamChave = strlen(c);
	// escrever a chave
	fwrite(c, sizeof(char), tamChave, f);
	
	char lixo = LIXO;
	for(int i; i < ARB_TAM_CHAVE - tamChave; i++){
		fwrite(&lixo, sizeof(char), 1, f);
	}
}

/// @brief Sobrescrever no n no arquivo de arvore B apontado por f de acordo com
/// a especificacao. A funcao automaticamente determina a posicao de escrita
/// usando o membro RRNDoNo.
/// @param f Arquivo de arvore B no qual inserir
/// @param n No a ser inserido
void arBEscreveNo(FILE *f, ArBNo *n){
	// determinar o byteoffset da escrita
	long off = arBByteoffsetRRN(n->RRNdoNo);
	// posicionar stream na escrita correta
	fseek(f, off, SEEK_SET);

	fwrite(&n->nroChavesNo, sizeof(int), 1, f);
	fwrite(&n->alturaNo, sizeof(int), 1, f);
	fwrite(&n->RRNdoNo, sizeof(int), 1, f);
	
	// Escrever campos nao nulos
	fwrite(&n->RRNFilho[0], sizeof(int), 1, f);
	for(int i = 0; i < n->nroChavesNo; i++){
		// ler corretamente ate lixo
		arBEscreveChave(f, n->chaveValor[i].chave);
		
		fwrite(&n->chaveValor[i].RRNArquivoDados, sizeof(int), 1, f);
		
		fwrite(&n->RRNFilho[i + 1], sizeof(int), 1, f);
	}
	// Escrever campos nulos
	for(int i = 0; i < ARB_ORDEM - 1 - n->nroChavesNo; i++){
		// chave null
		arBEscreveChave(f, "");
		// rrn do arquivo de dados null
		fwrite(&ARB_RRN_NULL, sizeof(int), 1, f);
		// rrn filho null
		fwrite(&ARB_RRN_NULL, sizeof(int), 1, f);
	}

}