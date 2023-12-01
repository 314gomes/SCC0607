#include <stdio.h>
#include "arquivos/impressao.h"
#include "arquivos/tipos.h"

#define VALOR_NULO "NULO"

/// @brief Imprime inteiro de acordo com a especificacao, lidando com valores
/// nulos
/// @param n Inteiro a ser impresso
void imprimeCampoInt (int n){
    // tratar de forma diferente quando inteiro esta marcado como nulo
    if (n == -1) {
        printf(VALOR_NULO);
    }
    else {
        printf("%d", n);
    }
}

/// @brief Imprime StringVariavel de acordo com a especificacao, lidando com
/// valores nulos
/// @param str StringVariavel a ser impressa 
void imprimeCampoStringVariavel(StringVariavel str){
    // tratar de forma diferente strings de tamanho 0
    if(str.tamanho == 0){
        printf(VALOR_NULO);
    }
    else {
        printf("%s", str.string);
    }
}

/// @brief Imprime separador de valor de acordo com a especificacao
void imprimeSeparador(){
    printf(", "); // separador de tokens
}

/// @brief Imprime conteudo do registro de acordo com a especificacao, lidando
/// automaticamente com valores nulos
/// @param r registro a ser impresso
void imprimeRegistro(Registro r){
    // imprimir nesta ordem:
    // nomeTecnologiaOrigem, grupo, popularidade, nomeTecnologiaDestino, peso
    imprimeCampoStringVariavel(r.tecnologiaOrigem);
    imprimeSeparador();
    
    imprimeCampoInt(r.grupo);
    imprimeSeparador();
    
    imprimeCampoInt(r.popularidade);
    imprimeSeparador();
    
    imprimeCampoStringVariavel(r.tecnologiaDestino);
    imprimeSeparador();

    imprimeCampoInt(r.peso);
    printf("\n");
}