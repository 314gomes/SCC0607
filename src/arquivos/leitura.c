#include <stdio.h>
#include <string.h>
#include "arquivos/leitura.h"
#include "arquivos/defines.h"
#include "arquivos/tipos.h"
#include "arquivos/utils.h"

void leCampoInt(FILE* bin, int *i){
    fread(i, sizeof(int), 1, bin);
}

void leCampoChar(FILE* bin, char *c){
    fread(c, sizeof(char), 1, bin);
}

void leCampoStringVariavel(FILE* bin, StringVariavel *s){
    // specification says StringVariavel field starts with string size
    leCampoInt(bin, &s->tamanho);
    // read s-> tamanho characters if string is not zero-lenght
    if(s->tamanho > 0){
        fread(s->string, sizeof(char), s->tamanho, bin);
    }
    // write string ending character to end of string
    // in case of zero-lenght, '\0' will be the first character
    s->string[s->tamanho] = '\0';
}

void leStatusRegistro(FILE *bin, Registro *r){
    leCampoChar(bin, &r->removido);
}

void leConteudoRegistro(FILE *bin, Registro *r){  
    leCampoInt(bin, &r->grupo);  
    leCampoInt(bin, &r->popularidade);
    leCampoInt(bin, &r->peso);
    leCampoStringVariavel(bin, &r->tecnologiaOrigem);
    leCampoStringVariavel(bin, &r->tecnologiaDestino);
}

StatusDeRetorno le_RRN(FILE *bin, int RRN, Registro *r){
    // fseek field start's byte offset 
    long byte_offset = byteoffset_RRN(RRN);
    fseek(bin, byte_offset, SEEK_SET);
    
    // reads status
    leStatusRegistro(bin, r);
    
    // reading might set EOF indicator for stream, in which case it is desired
    // to indicate that the registry does not exists, as we may have supassed
    // end of file.
    if(feof(bin)){
        return registro_inexistente;
    }
    // exit with `registro inexistente` code when attempting to read registry
    // marked as removed. 
    if(r->removido == REMOVIDO){
        return registro_inexistente;
    }

    // if no erros previously encountered, read remaining data and return
    // sucess code
    leConteudoRegistro(bin, r);

    return sucesso;
}