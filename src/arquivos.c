#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "arquivos.h"

void escreverRegistro(FILE *arquivo, Registro *registro) {
    fwrite(&registro->removido, sizeof(char), 1, arquivo);
    fwrite(&registro->grupo, sizeof(int), 1, arquivo);
    fwrite(&registro->popularidade, sizeof(int), 1, arquivo);
    fwrite(&registro->peso, sizeof(int), 1, arquivo);
    fwrite(registro->tecnologiaOrigem.string, sizeof(char), registro->tecnologiaOrigem.tamanho, arquivo);
    fwrite(&registro->tecnologiaOrigem.tamanho, sizeof(int), 1, arquivo);
    fwrite(registro->tecnologiaDestino.string, sizeof(char), registro->tecnologiaDestino.tamanho, arquivo);
    fwrite(&registro->tecnologiaDestino.tamanho, sizeof(int), 1, arquivo);
}

