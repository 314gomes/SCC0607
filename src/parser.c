#include "parser.h"

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
			status = csvParaBinario(args[0], args[1]);
			break;
		case SELECT_FROM:
			scanf("%s", args[0]);
			leitura_e_imprime(args[0]);
			break;
		case SELECT_WHERE:
			scanf("%s", args[0]);
			scanf("%d", &num_busca);
			funcionalidade3(args[0], num_busca);
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
