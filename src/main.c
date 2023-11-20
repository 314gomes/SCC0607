/**
 * Grupo 8
 * João Pedro Gomes - 13839069
 * Luana Hartmann Franco da Cruz - 13676350
*/
#include <stdio.h>
#include "funcionalidades.h"
#include "arquivos/defines.h"

typedef enum {
	CREATE_TABLE = 1,
	SELECT_FROM = 2,
	SELECT_WHERE = 3,
	SELECT_RRN = 4,
} Comandos;

void tratar_comando_entrada(){
	Comandos cmd_in = 0;
	StatusDeRetorno status;
	char bin_path[TAM_MAXIMO_STRING];
	scanf("%d", (int *) &cmd_in);

	switch (cmd_in){
		case CREATE_TABLE:
			char csv_path[TAM_MAXIMO_STRING];
			
			scanf("%s", csv_path);
			scanf("%s", bin_path);
			
			status = funcionalidade1(csv_path, bin_path);
			break;
		case SELECT_FROM:
			
			status = funcionalidade2(bin_path);
			
			break;
		case SELECT_WHERE:
			int num_busca;
			
			scanf("%s", bin_path);
			scanf("%d", &num_busca);
			
			status = funcionalidade3(bin_path, num_busca);
			break;
		case SELECT_RRN:
			int num_RRN;
			scanf("%s", bin_path);
			scanf("%d", &num_RRN);
			
			status = funcionalidade4(bin_path, num_RRN);
			
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
