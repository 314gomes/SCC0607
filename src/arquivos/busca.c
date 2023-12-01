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

void buscaAuxiliar (FILE* BIN_out, Registro* r_buffer) {
    leConteudoRegistro(BIN_out, r_buffer); 
    imprimeRegistro(*r_buffer);
}

/// @brief Funcao auxiliar utilizada na busca de campos fixos de inteiros
/// @param caminhoBin Caminho para arquivo binario
/// @param campo Campo que deve ser buscado
/// @param buscado Valor inteiro a ser buscado
/// @return Retorna `sucesso` quando a busca e bem sucedida, `registro_inexistente` se
/// o registro esta marcado como deletado ou caso nao esteja no arquivo
StatusDeRetorno buscaCampoInt (FILE* BIN_out, int campo, int buscado) {

    // r_buffer para auxiliar na leitura e impressao dos registros
    // status pre definido como 'registro_inexistente', sera alterado caso
    // a busca seja bem sucedida
    Registro *r_buffer = novo_registro();
    StatusDeRetorno status = registro_inexistente;
    
    // lixo calcula o número de bytes necessários para o próximo registro.
    // auxiliarInt armazena temporariamente o valor do campo lido.
    int lixo = 0, auxiliarInt;

    // auxiliarChar armazena temporariamente o valor do campo "removido".
    char auxiliarChar;

    fseek(BIN_out, 0, SEEK_SET);
    fseek(BIN_out, TAM_CABECALHO, SEEK_CUR);

    while(fread(&(auxiliarChar), sizeof(char), 1, BIN_out) != 0) {

       if (auxiliarChar == NAO_REMOVIDO) {

            // Caso o registro nao esteja removido, o campo procurado e lido 
            // e armazenado em auxiliarInt. Em seguida,
            // o ponteiro retorna ao comeco do registro
            fseek(BIN_out, campo, SEEK_CUR);
            fread(&(auxiliarInt), sizeof(int), 1, BIN_out); 
            fseek(BIN_out, -(campo+sizeof(int)), SEEK_CUR);

            // Caso o auxiliarInt seja igual ao buscado, status recebe 'sucesso'
            // e chama-se buscaAuxiliar para ler e imprimir o registro por completo.
            // lixo calcula a quantidade de bytes para o proximo registro
            // considerando que todo o registro atual foi lido.
            if (auxiliarInt == buscado) {              
                status = sucesso;

                buscaAuxiliar (BIN_out, r_buffer);
                lixo = TAM_REGISTRO - (TAM_REGISTRO_FIXO + r_buffer->tecnologiaOrigem.tamanho + r_buffer->tecnologiaDestino.tamanho);
                
            }
            // lixo calcula a quantidade de bytes para o proximo registro
            // desconsiderando apenas o campo "removido"
            else 
                lixo = TAM_REGISTRO - 1;
       }
       else 
            lixo = TAM_REGISTRO - 1;

        // proximo registro
        fseek(BIN_out, lixo, SEEK_CUR);
    }

    // libera r_buffer
    free_registro(r_buffer);

    return status;
}

/// @brief Funcao auxiliar utilizada na busca de campos variaveis de char.
/// @param caminhoBin Caminho para arquivo binario.
/// @param buscado String que sera buscada.
/// @param tamanho Tamanho da string a ser buscada.
/// @param flag Flag para informar qual string sera buscada. Seu valor e `0` para a primeira
/// string e `1` para a segunda.
/// @return Retorna `sucesso` quando a busca e bem sucedida, `registro_inexistente` se
/// o registro esta marcado como deletado ou caso nao esteja no arquivo.
StatusDeRetorno buscaCampoStringVariavel (FILE *BIN_out, char* buscado, int tamanho, int flag) {
    
    // r_buffer para auxiliar na leitura e impressao dos registros
    // status pre definido como 'registro_inexistente', sera alterado caso
    // a busca seja bem sucedida
    Registro *r_buffer = novo_registro();
    StatusDeRetorno status = registro_inexistente;

    // campo constante pois necessariamente devera
    // ler o tamanho da primeira string todas as vezes
    // lixo calcula o número de bytes necessários para o próximo registro.
    int campo = 12, lixo = 0;;

    // auxiliarTamanho armazena temporariamente o tamanho de 
    // duas strings que serao comparadas.
    int auxiliarTamanho[2];

    // auxiliarChar armazena temporariamente o valor do campo "removido".
    // auxiliarString armazena temporariamente a string lida.
    char auxiliarChar, auxiliarString[TAM_MAXIMO_STRING];
    
    fseek(BIN_out, 0, SEEK_SET);
    fseek(BIN_out, TAM_CABECALHO, SEEK_CUR);

    while(fread(&(auxiliarChar), sizeof(char), 1, BIN_out) != 0) {
        
        if (auxiliarChar == NAO_REMOVIDO) {
            
            // Caso o registro nao esteja removido, o tamanho da primeira
            // string e lido e armazenado em auxiliarTamanho[0]. Em seguida,
            // o ponteiro retorna ao comeco do registro
            fseek(BIN_out, campo, SEEK_CUR);
            fread(&(auxiliarTamanho[0]), sizeof(int), 1, BIN_out);
   
            switch(flag) {
                // caso 0: primeira string e a buscada
                case 0:
                    // Caso a string do registro atual tenha o mesmo tamanho da string
                    // buscada, ela e lida. Caso contrario, e passado para o proximo registro.
                    // Apos a leitura o ponteiro retorna ao inicio do registro.
                    if (auxiliarTamanho[0] == tamanho) {

                        fread(&(auxiliarString), sizeof(char), auxiliarTamanho[0], BIN_out);
                        fseek(BIN_out, -(campo+sizeof(int)+auxiliarTamanho[0]), SEEK_CUR);

                        // adiciona '\0' no final das strings que serao comparadas
                        // visando o comportamento correto de strcmp
                        auxiliarString[tamanho] = '\0';
                        buscado[tamanho] = '\0';

                        // Caso o auxiliarString seja igual ao buscado, status recebe 'sucesso'
                        // e chama-se buscaAuxiliar para ler e imprimir o registro por completo.
                        // lixo calcula a quantidade de bytes para o proximo registro
                        // considerando que todo o registro atual foi lido.
                        if (strcmp(auxiliarString, buscado) == 0) {
                            status = sucesso; 
                            buscaAuxiliar (BIN_out, r_buffer);
                            lixo = TAM_REGISTRO - (TAM_REGISTRO_FIXO + r_buffer->tecnologiaOrigem.tamanho + r_buffer->tecnologiaDestino.tamanho);
                        }
                        else {
                            lixo = TAM_REGISTRO-1;
                        }
                    }
                    // lixo calcula a quantidade de bytes para o proximo registro
                    // desconsiderando todos os campos ate o tamanho da primeira string
                    else {
                        fseek(BIN_out, -(campo+sizeof(int)), SEEK_CUR);
                        lixo = TAM_REGISTRO-1;
                    }
                break;

                // caso 1: segunda string e a buscada
                case 1:

                    // Utiliza-se o auxiiarTamanho[0] para pular os bytes necessarios
                    // e ler o tamanho da segunda string.
                    fseek(BIN_out, auxiliarTamanho[0], SEEK_CUR);
                    fread(&(auxiliarTamanho[1]), sizeof(int), 1, BIN_out);
                    
                    // Caso a string do registro atual tenha o mesmo tamanho da string
                    // buscada, ela e lida. Caso contrario, e passado para o proximo registro.
                    // Apos a leitura o ponteiro retorna ao inicio do registro.
                    if (auxiliarTamanho[1] == tamanho) {

                        fread(&(auxiliarString), sizeof(char), auxiliarTamanho[1], BIN_out);
                        fseek(BIN_out, -(campo+(2*sizeof(int))+auxiliarTamanho[0]+auxiliarTamanho[1]), SEEK_CUR);

                        // adiciona '\0' no final das strings que serao comparadas
                        // visando o comportamento correto de strcmp
                        auxiliarString[tamanho] = '\0';
                        buscado[tamanho] = '\0';

                        // Caso o auxiliarString seja igual ao buscado, status recebe 'sucesso'
                        // e chama-se buscaAuxiliar para ler e imprimir o registro por completo.
                        // lixo calcula a quantidade de bytes para o proximo registro
                        // considerando que todo o registro atual foi lido.
                        if (strcmp(auxiliarString, buscado) == 0) {
                            status = sucesso;   
                            buscaAuxiliar (BIN_out, r_buffer);
                            lixo = TAM_REGISTRO - (TAM_REGISTRO_FIXO + r_buffer->tecnologiaOrigem.tamanho + r_buffer->tecnologiaDestino.tamanho);
                        }
                        else {
                            lixo = TAM_REGISTRO-1;
                        }
                    }
                    else {
                        fseek(BIN_out, -(campo+(2*sizeof(int))+auxiliarTamanho[0]), SEEK_CUR);
                        lixo = TAM_REGISTRO-1;
                    }
                break;
            }
        }
        else
            lixo = TAM_REGISTRO - 1;

        // proximo registro
        fseek(BIN_out, lixo, SEEK_CUR);
    }

    // libera r_buffer
    free_registro(r_buffer);

    return status;
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