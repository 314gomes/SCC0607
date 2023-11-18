/**
 * Grupo 8
 * João Pedro Gomes - 13839069
 * Luana Hartmann Franco da Cruz - 13676350
*/
#include <stdio.h>
#include "funcionalidades.h"

#define TAMANHO_VALOR_ARGUMENTO 100
#define CONTAGEM_ARGUMENTOS 3

typedef enum {
	CREATE_TABLE = 1,
	SELECT_FROM = 2,
	SELECT_WHERE = 3,
	SELECT_RRN = 4,
} Comandos;

void tratar_comando_entrada(){
	Comandos cmd_in = 0;
	char args[CONTAGEM_ARGUMENTOS][TAMANHO_VALOR_ARGUMENTO];
	StatusDeRetorno status;
	int num_busca;
	int num_RRN;
	int buffer;
	scanf("%d", &buffer);
	cmd_in = buffer;

	switch (cmd_in){
		case CREATE_TABLE:
			scanf("%s", args[0]);
			scanf("%s", args[1]);
			status = funcionalidade1(args[0], args[1]);
			break;
		case SELECT_FROM:
			scanf("%s", args[0]);
			status = funcionalidade2(args[0]);
			break;
		case SELECT_WHERE:
			scanf("%s", args[0]);
			scanf("%d", &num_busca);
			status = funcionalidade3(args[0], num_busca);
			break;
		case SELECT_RRN:
			scanf("%s", args[0]);
			scanf("%d", &num_RRN);
			status = funcionalidade4(args[0], num_RRN);
			break;
		default:
			break;
	}

	switch (status)
	{
	case sucesso:
		break;
	
	case falha_processamento:
		printf("Falha no processamento do arquivo.");
		break;

	case registro_inexistente:
		printf("Registro inexistente.");
		break;

	default:
		printf("Erro desconhecido!");
		break;
	}

}

int main() {
    tratar_comando_entrada();
    return 0;
}
