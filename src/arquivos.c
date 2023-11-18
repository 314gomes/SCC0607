/**
 * Grupo 8
 * João Pedro Gomes - 13839069
 * Luana Hartmann Franco da Cruz - 13676350
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "arquivos.h"
#include "funcoesFornecidas.h"
#include "listaSE.h"
#include "arquivos/impressao.h"
#include "arquivos/escrita.h"

/***************** READING **********************/

long byteoffset_RRN(int RRN){
    // quite straightforward formula
    return TAM_CABECALHO + TAM_REGISTRO*RRN;
}

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


/***************** SEARCH **********************/

void auxiliarFuncionalidade3 (char* caminhoBin, int posicao) {

    // Função criada com o intuito de evitar múltiplas chamadas 
    // de fseek no arquivo

    Registro *r = novo_registro();
    FILE *BIN_out = abreBinario(caminhoBin);
    if(BIN_out == NULL) return;

    
    fseek(BIN_out, 0, SEEK_SET);
    fseek(BIN_out, posicao, SEEK_CUR);


    leStatusRegistro(BIN_out, r); 
    leConteudoRegistro(BIN_out, r); 
    imprimeRegistro(*r);

    free_registro(r);
    fclose(BIN_out);

}

StatusDeRetorno buscaCampoInt (char* caminhoBin, int campo, int buscado) {

    Registro *r = novo_registro();
    FILE *BIN_out = abreBinario(caminhoBin);

    StatusDeRetorno status = registro_inexistente;
    if(BIN_out == NULL) return falha_processamento;
    
    // lixo calcula bytes para o proximo registro
    // RRN encaminhado para a funcao auxiliar
    // aux_int armazena temporariamente o valor do campo lido
    int lixo = 0, RRN = 0, aux_int;

    // aux_char armazena temporariamente o valor do campo removido
    char aux_char;

    fseek(BIN_out, 0, SEEK_SET);
    fseek(BIN_out, TAM_CABECALHO, SEEK_CUR);

    while(fread(&(aux_char), sizeof(char), 1, BIN_out) != 0) {

        // TAM_REGISTRO - removido
        if (aux_char == REMOVIDO)
            lixo = TAM_REGISTRO - 1;

        else {  
            fseek(BIN_out, campo, SEEK_CUR);
            fread(&(aux_int), sizeof(int), 1, BIN_out);        

            if (aux_int == buscado) {
                // nao retorna registro inexistente quando encontra o buscado
                status = sucesso;

                auxiliarFuncionalidade3(caminhoBin, (RRN*TAM_REGISTRO)+TAM_CABECALHO);
        }    

        // TAM_REGISTRO - (removido + (campo + inteiro))
        lixo = TAM_REGISTRO - (1 + (campo+4));

        }

        RRN++;
        fseek(BIN_out, lixo, SEEK_CUR);
    }

    free_registro(r);
    fclose(BIN_out);

    return status;
}

StatusDeRetorno buscaCampoStringVariavel (char* caminhoBin, char* buscado, int tamanho, int flag) {
    Registro *r = novo_registro();
    FILE *BIN_out = abreBinario(caminhoBin);
    StatusDeRetorno status = registro_inexistente;
    if(BIN_out == NULL) return falha_processamento;

    // constant field = 3*int
    // grupo + popularidade + peso
    int campo = 12;
    int RRN = 0;

    // aux_size temporarily stores the size of the 
    // two strings that will be compared.
    int aux_tamanho[2], lixo = 0;

    // aux_char temporarily stores the value of the removed field
    // aux_string temporarily stores the read field
    char aux_char, aux_string[TAM_MAXIMO_STRING];
    
    fseek(BIN_out, 0, SEEK_SET);
    fseek(BIN_out, TAM_CABECALHO, SEEK_CUR);

    while(fread(&(aux_char), sizeof(char), 1, BIN_out) != 0) {
        if (aux_char == REMOVIDO) lixo = TAM_REGISTRO - 1;

        else {
            
            fseek(BIN_out, campo, SEEK_CUR);

            // size of the first string stored
            // case 0: string read
            // case 1: fseek with the string's size
            fread(&(aux_tamanho[0]), sizeof(int), 1, BIN_out);

            switch(flag) {
                // first variable-length string
                case 0:
                    if (aux_tamanho[0] == tamanho) {
                        fread(&(aux_string), sizeof(char), aux_tamanho[0], BIN_out);

                        // (TAM_REGISTRO - 4): tamanhoDestino was not read
                        lixo = TAM_REGISTRO - ((TAM_REGISTRO_FIXO - 4)+tamanho);

                        // assigns \0 at the end of strings for comparison purposes
                        aux_string[tamanho] = '\0';
                        buscado[tamanho] = '\0';

                        if (strcmp(aux_string, buscado) == 0) {
                            status = sucesso;
                            auxiliarFuncionalidade3(caminhoBin, (RRN*TAM_REGISTRO)+TAM_CABECALHO);
                        }
                    }
                    else {
                        lixo = TAM_REGISTRO - (TAM_REGISTRO_FIXO - 4);
                    }
                break;

                // second variable-length string
                case 1:
                    fseek(BIN_out, aux_tamanho[0], SEEK_CUR);
                    fread(&(aux_tamanho[1]), sizeof(int), 1, BIN_out);
                    
                    if (aux_tamanho[1] == tamanho) {
                        fread(&(aux_string), sizeof(char), aux_tamanho[1], BIN_out);

                        // only the record's GARBAGE remains
                        lixo = TAM_REGISTRO - (TAM_REGISTRO_FIXO + aux_tamanho[0] + tamanho);

                        aux_string[tamanho] = '\0';
                        buscado[tamanho] = '\0';

                        if (strcmp(aux_string, buscado) == 0) {
                            status = sucesso;
                            auxiliarFuncionalidade3(caminhoBin, (RRN*TAM_REGISTRO)+TAM_CABECALHO);
                        }
                    }
                    else {
                        // remains the last string and the garbage of the record
                        lixo = TAM_REGISTRO - (TAM_REGISTRO_FIXO + aux_tamanho[0]);
                    }
                break;
            }
        }
        RRN++;
        fseek(BIN_out, lixo, SEEK_CUR);
    }

    return status;

    free_registro(r);
    fclose(BIN_out);
}