#include "stdio.h"
#include "arquivos/escrita.h"
#include "arquivos/tipos.h"
#include "arquivos/utils.h"
#include "arquivos/defines.h"

/// @brief Escrever campo de um char no arquivo `a`
/// @param a arquivo a escrever
/// @param c campo onde ser escrito
void escreverCampoChar(FILE *a, char c){
    fwrite(&c, sizeof(c), 1, a);
}

/// @brief Escrever campo de um char no arquivo `a`
/// @param a arquivo onde escrever
/// @param c campo a ser escrito
void escreverCampoInt(FILE *a, int c){
    fwrite(&c, sizeof(c), 1, a);
}

/// @brief Escrever campo de tipo StringVariavel no arquivo `a`
/// @param a arquivo onde escrever
/// @param c campo a ser escrito
void escreverCampoStringVariavel(FILE *a, StringVariavel c){
    escreverCampoInt(a, c.tamanho);
    fwrite(c.string, sizeof(char), c.tamanho, a);
}

/// @brief Escreve dados de `r` no `arquivo` de acordo com a especificacao
/// @param arquivo arquivo onde escrever `registro`
/// @param r registro a ser escrito
void escreverDadosRegistro(FILE *arquivo, Registro *r){
    escreverCampoChar(arquivo, r->removido);
    escreverCampoInt(arquivo, r->grupo);
    escreverCampoInt(arquivo, r->popularidade);
    escreverCampoInt(arquivo, r->peso);
    escreverCampoStringVariavel(arquivo, r->tecnologiaOrigem);
    escreverCampoStringVariavel(arquivo, r->tecnologiaDestino);

}

/// @brief Escrever lixo para completar tamanho do registro.
/// @param arquivo arquivo onde escrever o lixo de `registro`
/// @param r registro a partir do qual calcular tamanho do lixo
void escreverLixoRegistro(FILE *arquivo, Registro *r){
    int t = calcularTamanhoRegistro(r);
    int numeroLixos = TAM_REGISTRO - t;
    char lixo = LIXO;
    for(int i = 0; i < numeroLixos; i++){
        fwrite(&lixo, sizeof(lixo), 1, arquivo);
    }
}

/// @brief Writes `registro` to `arquivo`
/// @param arquivo file to write `registro`
/// @param r register to be written
void escreverRegistro(FILE *arquivo, Registro *r) {
    escreverDadosRegistro(arquivo, r);
    escreverLixoRegistro(arquivo, r);
}

/// @brief Writes `cabecalho` to `arquivo`
/// @param arquivo file to write `registro`
/// @param h register to be written
void escreverCabecalho(FILE *arquivo, Cabecalho *h){
    fseek(arquivo, 0, SEEK_SET);
    escreverCampoChar(arquivo, h->status);
    escreverCampoInt(arquivo, h->proxRRN);
    escreverCampoInt(arquivo, h->nroTecnologias);
    escreverCampoInt(arquivo, h->nroParesTecnologias);
}