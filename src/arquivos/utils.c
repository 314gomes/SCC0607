#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "arquivos/utils.h"
#include "arquivos/tipos.h"
#include "arquivos/defines.h"

Registro *novo_registro() {

    Registro* r_buffer;
    r_buffer = (Registro*)malloc(sizeof(Registro));

    r_buffer->removido = '0';
    r_buffer->tecnologiaOrigem.string = NULL;
    r_buffer->tecnologiaOrigem.tamanho = 0;
    r_buffer->tecnologiaDestino.string = NULL;
    r_buffer->tecnologiaDestino.tamanho = 0;
    r_buffer->grupo = -1;
    r_buffer->popularidade = -1;
    r_buffer->peso = -1;

    r_buffer->tecnologiaOrigem.string = (char *)malloc(TAM_MAXIMO_STRING);
    r_buffer->tecnologiaDestino.string =(char *)malloc(TAM_MAXIMO_STRING);

    return r_buffer;
}

void free_registro(Registro *r){
    free(r->tecnologiaOrigem.string);
    free(r->tecnologiaDestino.string);
    free(r);
}

Cabecalho *novo_cabecalho() {

    Cabecalho* cabecalho;
    cabecalho = (Cabecalho*)malloc(sizeof(Cabecalho));

    cabecalho->status = INCONSISTENTE;
    cabecalho->proxRRN = 0;
    cabecalho->nroTecnologias = 0;
    cabecalho->nroParesTecnologias = 0;

    return cabecalho;
}

int calcularTamanhoRegistro(Registro *r){
    int tamanhoRegistro = TAM_REGISTRO_FIXO;
    tamanhoRegistro += r->tecnologiaOrigem.tamanho;
    tamanhoRegistro += r->tecnologiaDestino.tamanho;
    return tamanhoRegistro;
}

FILE *abreBinario(char *caminhoBin, char *modo){
    char status;
    
    FILE *bin = fopen(caminhoBin, modo);
    if(bin == NULL){
        return NULL;
    }

    fread(&status, 1, 1, bin);
    
    if(status == INCONSISTENTE){
        fclose(bin);
        return NULL;
    }
    
    return bin;
}

long byteoffset_RRN(int RRN){
    // quite straightforward formula
    return TAM_CABECALHO + TAM_REGISTRO*RRN;
}

int campoDeBuscaEString(char* nomeCampo){
    if (strcmp(nomeCampo, "grupo") == 0) {
        return 0;
    }
    else if (strcmp(nomeCampo, "popularidade") == 0) {
        return 0;
    }
    else if (strcmp(nomeCampo, "peso") == 0) {
        return 0;
    } 
    else if (strcmp(nomeCampo, "nomeTecnologiaOrigem") == 0) {
        return 1;
    } 
    else if (strcmp(nomeCampo, "nomeTecnologiaDestino") == 0) {
        return 1;
    }
    return -1;
}