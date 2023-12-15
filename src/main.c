/**
 * Grupo 8
 * João Pedro Gomes - 13839069
 * Luana Hartmann Franco da Cruz - 13676350
*/
#include <stdio.h>
#include <stdlib.h>
#include "funcionalidades.h"
#include "arquivos/defines.h"
#include "funcoesFornecidas.h"
#include "arquivos/utils.h"

typedef enum {
	CREATE_TABLE = 1,
	SELECT_FROM = 2,
	SELECT_WHERE = 3,
	SELECT_RRN = 4,
	CREATE_INDEX = 5,
	SELECT_WHERE_ARB = 6,
	INSERT_ARB = 7,
	GRAFO_DIRPON = 8,
	GRAFO_TRANS = 9,
	GRAFO_WHERE = 10,
	GRAFO_CONEX = 11,
	GRAFO_CAMIN = 12,
} Comandos;

void criar_termos_busca(int n, char ***campo, char ***valor){
	*campo = calloc(n, sizeof(char*));
	*valor = calloc(n, sizeof(char*));
	for (int i = 0; i < n; i++)
	{
		(*campo)[i] = calloc(TAM_MAXIMO_STRING, sizeof(char));
		(*valor)[i] = calloc(TAM_MAXIMO_STRING, sizeof(char));
	}
	
}

void ler_termos_busca(int n, char** campo, char** valor){
	for (int i = 0; i < n; i++){
		scanf("%s", campo[i]);
		if(campoDeBuscaEString(campo[i])){
			scan_quote_string(valor[i]);
		}
		else{
			scanf("%s", valor[i]);
		}
		
	}
}

void destruir_termos_busca(int n, char **campo, char **valor){
	for (int i = 0; i < n; i++)
	{
		free(campo[i]);
		free(valor[i]);
	}
	free(campo);
	free(valor);
}


void criar_linhas_insercao (int n,  char*** linhas) {
	*linhas = calloc(n, sizeof(char*));
	for (int i = 0; i < n; i++)
	{
		(*linhas)[i] = calloc(TAM_MAXIMO_STRING, sizeof(char));
	}
}

void ler_linhas_insercao (int n, char** linhas) {
	 for (int i = 0; i < n; i++) {
		getchar();
        scanf("%[^\n]s", linhas[i]);
    }
}

void destruir_linhas_insercao (int n, char **linhas){
	for (int i = 0; i < n; i++)
	{
		free(linhas[i]);
	}
	free(linhas);
}


void criar_busca_grafo(int n, char ***campo){
	*campo = calloc(n, sizeof(char*));
	for (int i = 0; i < n; i++)
	{
		(*campo)[i] = calloc(TAM_MAXIMO_STRING, sizeof(char));
	}
	
}

void ler_busca_grafo(int n, char** campo){
	for (int i = 0; i < n; i++){
		scan_quote_string(campo[i]);
	}
}

void destruir_busca_grafo(int n, char **campo){
	for (int i = 0; i < n; i++)
	{
		free(campo[i]);
	}
	free(campo);

}
 
void tratar_comando_entrada(){
	// variaveis utilizadas na funcao
	Comandos cmd_in = 0;
	StatusDeRetorno status;
	char bin_path[TAM_MAXIMO_STRING];
	char index_path[TAM_MAXIMO_STRING];
	int num_busca;
	int num_insert_arb;
	char **campo;
	char **valor;
	char **linhas;

	char csv_path[TAM_MAXIMO_STRING];
	int num_RRN;

	scanf("%d", (int *) &cmd_in);

	switch (cmd_in){
		case CREATE_TABLE:
				
			scanf("%s", csv_path);
			scanf("%s", bin_path);
			
			status = funcionalidade1(csv_path, bin_path);
			break;
		case SELECT_FROM:
			scanf("%s", bin_path);
			
			status = funcionalidade2(bin_path);
			
			break;
		case SELECT_WHERE:
			scanf("%s", bin_path);
			scanf("%d", &num_busca);

			criar_termos_busca(num_busca, &campo, &valor);

			ler_termos_busca(num_busca, campo, valor);

			status = funcionalidade3(bin_path, num_busca, campo, valor);

			destruir_termos_busca(num_busca, campo, valor);
			break;
		case SELECT_RRN:
			
			scanf("%s", bin_path);
			scanf("%d", &num_RRN);
			
			status = funcionalidade4(bin_path, num_RRN);
			
			break;
		case CREATE_INDEX:
			scanf("%s", bin_path);
			scanf("%s", index_path);

			status = funcionalidade5(bin_path, index_path);
			break;
		case SELECT_WHERE_ARB:
			scanf("%s", bin_path);
			scanf("%s", index_path);
			scanf("%d", &num_busca);

			criar_termos_busca(num_busca, &campo, &valor);

			ler_termos_busca(num_busca, campo, valor);

			status = funcionalidade6(bin_path, index_path, num_busca, campo, valor);

			destruir_termos_busca(num_busca, campo, valor);
			break;
		case INSERT_ARB:
			scanf("%s", bin_path);
			scanf("%s", index_path);
			scanf("%d", &num_insert_arb);

			criar_linhas_insercao(num_insert_arb, &linhas);

			ler_linhas_insercao(num_insert_arb, linhas);

			status = funcionalidade7(bin_path, index_path, num_insert_arb, linhas);
			
			destruir_linhas_insercao(num_insert_arb, linhas);

			break;
		case GRAFO_DIRPON: // func8
			scanf("%s", bin_path);

			status = funcionalidade8(bin_path);

			break;
		case GRAFO_TRANS: // func9
			scanf("%s", bin_path);

			status = funcionalidade9(bin_path);

			break;
		case GRAFO_WHERE:
			scanf("%s", bin_path);
			scanf("%d", &num_busca);

			criar_busca_grafo(num_busca, &campo);

			ler_busca_grafo (num_busca, campo);

			status = funcionalidade10(bin_path, num_busca, campo);

			destruir_busca_grafo (num_busca, campo);

			break;
		case GRAFO_CONEX: // func11
			scanf("%s", bin_path);

			status = funcionalidade11(bin_path);

			break;
		default:
			break;
	}

	imprimeMensagemErro(status);

}

int main() {
    tratar_comando_entrada();
    return 0;
}
