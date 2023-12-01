#include "arquivos/tipos.h"

StatusDeRetorno buscaCampoInt (FILE* BIN_out, int campo, int buscado);

StatusDeRetorno buscaCampoStringVariavel (FILE* BIN_out, char* buscado, int tamanho, int flag);

StatusDeRetorno buscaCampo(FILE* bin, char* campo, char* valor);