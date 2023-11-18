#include <stdio.h>
#include "arquivos/tipos.h"

void imprimeCampoInt (int n){
    if (n == -1) {
        printf("NULO");
    }
    else {
        printf("%d", n);
    }
}

void imprimeCampoStringVariavel(StringVariavel str){
    // specification says zero-lenght strings must be printed as "NULO"
    if(str.tamanho == 0){
        printf("NULO");
    }
    else {
        printf("%s", str.string);
    }
}

void imprimeSeparador(){
    printf(", "); // token separator according to specification
}

void imprimeRegistro(Registro r){
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