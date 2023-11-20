/**
 * Grupo 8
 * João Pedro Gomes - 13839069
 * Luana Hartmann Franco da Cruz - 13676350
*/

#include <string.h>
#include <stdlib.h>
#include "arquivos/busca.h"
#include "arquivos/utils.h"
#include "arquivos/leitura.h"
#include "arquivos/defines.h"
#include "arquivos/impressao.h"

/***************** SEARCH **********************/

void buscaAuxiliar (FILE* BIN_out, Registro* r) {
    leConteudoRegistro(BIN_out, r); 
    imprimeRegistro(*r);
}

StatusDeRetorno buscaCampoInt (FILE* BIN_out, int campo, int buscado) {

    Registro *r = novo_registro();
    StatusDeRetorno status = registro_inexistente;
    
    // lixo calcula bytes para o proximo registro
    // RRN encaminhado para a funcao auxiliar
    // aux_int armazena temporariamente o valor do campo lido
    int lixo = 0, RRN = 0, aux_int;

    // aux_char armazena temporariamente o valor do campo removido
    char aux_char;

    fseek(BIN_out, 0, SEEK_SET);
    fseek(BIN_out, TAM_CABECALHO, SEEK_CUR);

    while(fread(&(aux_char), sizeof(char), 1, BIN_out) != 0) {

       if (aux_char == NAO_REMOVIDO) {

            fseek(BIN_out, campo, SEEK_CUR);
            fread(&(aux_int), sizeof(int), 1, BIN_out); 
            fseek(BIN_out, -(campo+sizeof(int)), SEEK_CUR);

            if (aux_int == buscado) {
                // nao retorna registro inexistente quando encontra o buscado
                status = sucesso;

                buscaAuxiliar (BIN_out, r);
                lixo = TAM_REGISTRO - (TAM_REGISTRO_FIXO + r->tecnologiaOrigem.tamanho + r->tecnologiaDestino.tamanho);
                
            }
            else 
                lixo = TAM_REGISTRO - 1;
       }
       else 
            lixo = TAM_REGISTRO - 1;

        RRN++;
        fseek(BIN_out, lixo, SEEK_CUR);
    }

    free_registro(r);

    return status;
}


StatusDeRetorno buscaCampoStringVariavel (FILE *BIN_out, char* buscado, int tamanho, int flag) {
    Registro *r = novo_registro();
    StatusDeRetorno status = registro_inexistente;

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
        
        if (aux_char == NAO_REMOVIDO) {
            
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
                        fseek(BIN_out, -(campo+sizeof(int)+aux_tamanho[0]), SEEK_CUR);

                        // assigns \0 at the end of strings for comparison purposes
                        aux_string[tamanho] = '\0';
                        buscado[tamanho] = '\0';

                        if (strcmp(aux_string, buscado) == 0) {
                            status = sucesso; 

                            buscaAuxiliar (BIN_out, r);
                            lixo = TAM_REGISTRO - (TAM_REGISTRO_FIXO + r->tecnologiaOrigem.tamanho + r->tecnologiaDestino.tamanho);
                        }
                        else {
                            lixo = TAM_REGISTRO-1;
                        }
                    }
                    else {
                        fseek(BIN_out, -(campo+sizeof(int)), SEEK_CUR);
                        lixo = TAM_REGISTRO-1;
                    }
                break;

                // second variable-length string
                case 1:
                    fseek(BIN_out, aux_tamanho[0], SEEK_CUR);
                    fread(&(aux_tamanho[1]), sizeof(int), 1, BIN_out);
                    
                    if (aux_tamanho[1] == tamanho) {
                        fread(&(aux_string), sizeof(char), aux_tamanho[1], BIN_out);
                        fseek(BIN_out, -(campo+(2*sizeof(int))+aux_tamanho[0]+aux_tamanho[1]), SEEK_CUR);

                        aux_string[tamanho] = '\0';
                        buscado[tamanho] = '\0';

                        if (strcmp(aux_string, buscado) == 0) {
                            status = sucesso;   

                            buscaAuxiliar (BIN_out, r);
                            lixo = TAM_REGISTRO - (TAM_REGISTRO_FIXO + r->tecnologiaOrigem.tamanho + r->tecnologiaDestino.tamanho);
                        }
                        else {
                            lixo = TAM_REGISTRO-1;
                        }
                    }
                    else {
                        fseek(BIN_out, -(campo+(2*sizeof(int))+aux_tamanho[0]), SEEK_CUR);
                        lixo = TAM_REGISTRO-1;
                    }
                break;
            }
        }
        else
            lixo = TAM_REGISTRO - 1;
            
        RRN++;
        fseek(BIN_out, lixo, SEEK_CUR);
    }

    return status;

    free_registro(r);
}

/// @brief Funcao auxiliar de busca de campo qualquer
/// @param bin FILE* para o arquivo binario de busca
/// @param campo nome do campo a ser buscado
/// @param valor valor de busca
/// @return `sucesso` se encontrado, `registro_inexistente` se nenhum registro
/// atende à busca.
StatusDeRetorno buscaCampo(FILE* bin, char* campo, char* valor){
    int tamanhostring;
    int buscado;
    
    if (strcmp(campo, "grupo") == 0) {
        buscado = atoi(valor);
        return buscaCampoInt (bin, 0, buscado);
    }
    else if (strcmp(campo, "popularidade") == 0) {
        buscado = atoi(valor);
        return buscaCampoInt (bin, 4, buscado);
    }
    else if (strcmp(campo, "peso") == 0) {
        buscado = atoi(valor);
        return buscaCampoInt (bin, 8, buscado);
    } 
    else if (strcmp(campo, "nomeTecnologiaOrigem") == 0) {
        tamanhostring = strlen(valor);
        return buscaCampoStringVariavel(bin, valor, tamanhostring, 0);
    } 
    else if (strcmp(campo, "nomeTecnologiaDestino") == 0) {
        tamanhostring = strlen(valor);
        return buscaCampoStringVariavel(bin, valor, tamanhostring, 1);
    }
    return -1;  
}