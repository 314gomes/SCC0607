#include "parser.h"

void tratar_comando_entrada(){
	Comandos cmd_in = 0;
	char args[CONTAGEM_ARGUMENTOS][TAMANHO_VALOR_ARGUMENTO];
	int num_busca;
	int buffer;
	scanf("%d", &buffer);
	cmd_in = buffer;

	switch (cmd_in){
		case CREATE_TABLE:
			scanf("%s", args[0]);
			scanf("%s", args[1]);
			csvParaBinario(args[0], args[1]);
			break;
		case SELECT_FROM:
			scanf("%s", args[0]);
			leitura_e_imprime(args[0]);
			break;
		case SELECT_WHERE:
			scanf("%s", args[0]);
			scanf("%d", &num_busca);
			busca_campo(args[0], num_busca);
		default:
			break;
	}
}
