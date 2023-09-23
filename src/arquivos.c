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

