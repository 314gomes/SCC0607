#include <string.h>
#include <stdio.h>
#include "arvoreb/busca.h"
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
	// indice de inicio da busca binaria
	int ini = 0;
	// indice de fim da busca binaria
	int fim = no->nroChavesNo - 1;
	// variaveis para indice de meio e comparacao das strings
	int meio, cmp;

	// enquanto houver busca consistente
	while (ini <= fim){
		// atualizar meio
		meio = ini + (fim - ini)/2;
		// atualizar comparacao
		cmp = strcmp(c, no->chaveValor[meio].chave);

		// encontrada a chave, encerrar
		if(cmp == 0){
			break;
		}
		// se e menor, buscar na metade inferior
		else if(cmp < 0){
			fim = meio - 1;
		}
		// se e maior, buscar na metade superior
		else if(cmp > 0){
			ini = meio + 1;
		}
	}

	// no fim do laco, escrever o indice de retorno enocntrado
	*indice_retorno = meio;
	return cmp;

}

/// @brief Funcao recursiva da busca em arvore B.
/// @param i FILE* da arvore B
/// @param c chave de busca
/// @param r RRN de retorno 
/// @param RRN_busca RRN no qual realizar a busca
/// @param no No para o qual escrever os dados lidos
/// @return `sucesso` caso tenha sido encontrado, `registro_inexistente` caso
/// contrario.
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

	// Caso contrario, determinar em qual RRN deve ser feita a proxima busca
	int RRN_prox_busca;
	// proxima busca no filho esquerdo
	if(cmp < 0){
		RRN_prox_busca = no->RRNFilho[indice_encontrado];
	}
	// proxima busca no filho direito
	else{
		RRN_prox_busca = no->RRNFilho[indice_encontrado + 1];
	}

	// caso RRN seja nulo, chegou ao fim da arvore sem encontrar. Retornar
	// registro_inexistente
	if(RRN_prox_busca == ARB_RRN_NULL){
		return registro_inexistente;
	}
	// Se for RRN valido, busca recursiva no proximo RRN
	else{
		return arBBuscaRecursiva(i, c, r, RRN_prox_busca, no);
	}

}

/// @brief Executa busca em arquivo de arvore b.
/// @param indices FILE* para o arquio de indices arvore B.
/// @param chave chave de busca.
/// @param resultado_RRN Resultado da busca.
/// @return `sucesso` caso tenha sido encontrado, `registro_inexistente` caso
/// contrario.
StatusDeRetorno arBBusca(FILE* indices, char *chave, int *resultado_RRN){
	// deixar apenas um no em memoria primaria e passa-lo por referencia para
	// a funcao recursiva
	ArBNo noDeBusca;

	// variavel para guardar o RRN do no raiz, pelo qual inciar a busca
	int RRN_no_raiz;
	// variavel local para guardar o status de retorno da busca
	StatusDeRetorno s;	

	// Posicionar stream para leitura do RRN do no raiz
	fseek(indices, ARB_POS_NO_RAIZ, SEEK_SET);
	// Ler RRN do no raiz
	fread(&RRN_no_raiz, sizeof(int), 1, indices);
	
	// Iniciar busca recursiva pela raiz
	s = arBBuscaRecursiva(indices, chave, resultado_RRN, RRN_no_raiz, &noDeBusca);
	// Retornar status da busca
	return s;
}