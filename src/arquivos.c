#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "arquivos.h"

int calcularTamanhoRegistro(Registro *r){
    int tamanhoRegistro = TAM_REGISTRO_FIXO;
    tamanhoRegistro += r->tecnologiaOrigem.tamanho;
    tamanhoRegistro += r->tecnologiaDestino.tamanho;
    return tamanhoRegistro;
}

void escreverDadosRegistro(FILE *arquivo, Registro *r){
    fwrite(&r->removido, sizeof(char), 1, arquivo);
    fwrite(&r->grupo, sizeof(int), 1, arquivo);
    fwrite(&r->popularidade, sizeof(int), 1, arquivo);
    fwrite(&r->peso, sizeof(int), 1, arquivo);
    fwrite(&r->tecnologiaOrigem.tamanho, sizeof(int), 1, arquivo);
    fwrite(r->tecnologiaOrigem.string, sizeof(char), r->tecnologiaOrigem.tamanho, arquivo);
    fwrite(&r->tecnologiaDestino.tamanho, sizeof(int), 1, arquivo);
    fwrite(r->tecnologiaDestino.string, sizeof(char), r->tecnologiaDestino.tamanho, arquivo);
}

void escreverLixoRegistro(FILE *arquivo, Registro *r){
    int t = calcularTamanhoRegistro(r);
    int numeroLixos = TAM_REGISTRO - t;
    char lixo = LIXO;
    for(int i = 0; i < numeroLixos; i++){
        fwrite(&lixo, sizeof(lixo), 1, arquivo);
    }
}

void escreverRegistro(FILE *arquivo, Registro *r) {
    escreverDadosRegistro(arquivo, r);
    escreverLixoRegistro(arquivo, r);
}

void csvParaBinario(char* caminhoCSV, char* caminhoBin){
    char CSV_line_buffer[100];
    char tec_origem_buffer[TAM_MAXIMO_STRING];
    char tec_destino_buffer[TAM_MAXIMO_STRING];
    Registro r_buffer;

    r_buffer.tecnologiaOrigem.string = tec_origem_buffer;
    r_buffer.tecnologiaDestino.string = tec_destino_buffer;
    
    FILE *CSV_in = fopen(caminhoCSV, "r");
    FILE *BIN_out = fopen(caminhoBin, "wb");

    // Cannot open file
    if (CSV_in == NULL) {
        return;
    }

    while (fgets(CSV_line_buffer, sizeof(CSV_line_buffer), CSV_in)) {
        sscanf(CSV_line_buffer, "%49[^,],%d,%d,%49[^,],%d",
               r_buffer.tecnologiaOrigem.string,
               &r_buffer.grupo,
               &r_buffer.popularidade,
               r_buffer.tecnologiaDestino.string,
               &r_buffer.peso);

        r_buffer.removido = '0';

        r_buffer.tecnologiaOrigem.tamanho = strlen(r_buffer.tecnologiaOrigem.string);
        r_buffer.tecnologiaDestino.tamanho = strlen(r_buffer.tecnologiaDestino.string);

        escreverRegistro(BIN_out, &r_buffer);
    }

    fclose(CSV_in);
    fclose(BIN_out);
}
