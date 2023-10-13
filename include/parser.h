#include "arquivos.h"

#define TAMANHO_VALOR_ARGUMENTO 100
#define CONTAGEM_ARGUMENTOS 3

typedef enum {
	CREATE_TABLE = 1,
	SELECT_FROM = 2,
	SELECT_WHERE = 3,
	SELECT_RRN = 4,
} Comandos;

/// @brief function to parse input from stdin as commands to be executed
void tratar_comando_entrada();