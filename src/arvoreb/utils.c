#include <stdio.h>
#include "arvoreb/utils.h"
#include "arvoreb/defines.h"

/// @brief Calcular byteoffset de um RRN do arquivo de indices arvore B
/// @param RRN numero relativo de registro
/// @return byteoffset correspondente
long arBByteoffsetRRN(int RRN){
    return ARB_TAM_PAGINA * (RRN + 1);
}

/// @brief Abrir arquivo de indice binario da Arvore B em modo de leitura
/// @param caminhobin caminho para o arquivo de indice da Arvore
/// @return NULL se status for `INCONSISTENTE` ou se arquivo não existir, FILE*
/// do arquivo binário se tudo correto.
FILE *arBAbre(char *caminhoBin, char *modo){
    char status;
    
    FILE *bin = fopen(caminhoBin, modo);
    if(bin == NULL){
        return NULL;
    }

    fseek(bin, 0, SEEK_SET);
    fread(&status, 1, 1, bin);
    
    if(status == INCONSISTENTE){
        fclose(bin);
        return NULL;
    }
    
    return bin;
}