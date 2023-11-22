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
	SELECT_WHERE_ARB = 6,
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

void tratar_comando_entrada(){
	Comandos cmd_in = 0;
	StatusDeRetorno status;
	char bin_path[TAM_MAXIMO_STRING];
	scanf("%d", (int *) &cmd_in);


	int num_busca;
	char **campo;
	char **valor;
	switch (cmd_in){
		case CREATE_TABLE:
			char csv_path[TAM_MAXIMO_STRING];
			
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
			int num_RRN;
			scanf("%s", bin_path);
			scanf("%d", &num_RRN);
			
			status = funcionalidade4(bin_path, num_RRN);
			
			break;
		case SELECT_WHERE_ARB:
			char index_path[TAM_MAXIMO_STRING];

			scanf("%s", bin_path);
			scanf("%s", index_path);
			scanf("%d", &num_busca);

			criar_termos_busca(num_busca, &campo, &valor);

			ler_termos_busca(num_busca, campo, valor);

			status = funcionalidade6(bin_path, index_path, num_busca, campo, valor);

			destruir_termos_busca(num_busca, campo, valor);
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
