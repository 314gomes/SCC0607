#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "arquivos.h"
#include "funcoesFornecidas.h"

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
}

Cabecalho *novo_cabecalho() {

    Cabecalho* cabecalho;
    cabecalho = (Cabecalho*)malloc(sizeof(Cabecalho));

    cabecalho->status = '0';
    cabecalho->proxRRN = 0;
    cabecalho->nroTecnologias = 0;
    cabecalho->nroParesTecnologias = 0;
}


int calcularTamanhoRegistro(Registro *r){
    int tamanhoRegistro = TAM_REGISTRO_FIXO;
    tamanhoRegistro += r->tecnologiaOrigem.tamanho;
    tamanhoRegistro += r->tecnologiaDestino.tamanho;
    return tamanhoRegistro;
}

void escreverCampoChar(FILE *a, char c){
    fwrite(&c, sizeof(c), 1, a);
}
void escreverCampoInt(FILE *a, int c){
    fwrite(&c, sizeof(c), 1, a);
}
void escreverCampoStringVariavel(FILE *a, StringVariavel c){
    escreverCampoInt(a, c.tamanho);
    fwrite(c.string, sizeof(char), c.tamanho, a);
}

void escreverDadosRegistro(FILE *arquivo, Registro *r){
    escreverCampoChar(arquivo, r->removido);
    escreverCampoInt(arquivo, r->grupo);
    escreverCampoInt(arquivo, r->popularidade);
    escreverCampoInt(arquivo, r->peso);
    escreverCampoStringVariavel(arquivo, r->tecnologiaOrigem);
    escreverCampoStringVariavel(arquivo, r->tecnologiaDestino);

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

    // inicializa novo registro e novo cabecalho
    Registro *r_buffer = novo_registro();
    Cabecalho *cabecalho = novo_cabecalho();
    
    // abre arquivos de leitura e escrita em binario
    FILE *CSV_in = fopen(caminhoCSV, "r");
    FILE *BIN_out = fopen(caminhoBin, "wb");
    
    // verifica se foram abertos corretamente
    if (CSV_in == NULL) return;
    if (BIN_out == NULL) return;

    char CSV_line_buffer[100];
    int cont_registros = 0;

    // le o arquivo csv e escreve no arquivo binario
    while (fgets(CSV_line_buffer, sizeof(CSV_line_buffer), CSV_in)) {        
        cont_registros++;
        
        int field = 0;
        char *token = NULL;
        char *saveptr = CSV_line_buffer;

        while ((token = strsep(&saveptr, ",")) != NULL) {
            switch (field) {
                case 0:               
                    if (strcmp(token, "") != 0) {
                        r_buffer->tecnologiaOrigem.string = strdup(token);
                        r_buffer->tecnologiaOrigem.tamanho = strlen(token);
                    } else {
                        r_buffer->tecnologiaOrigem.string = NULL;
                    }
                    break;
                case 1:
                    if (strcmp(token, "") != 0) {
                        r_buffer->grupo = atoi(token);
                    } else {
                        r_buffer->grupo = -1;
                    }
                    break;
                case 2:
                    if (strcmp(token, "") != 0) {
                        r_buffer->popularidade = atoi(token);                    
                    } else {
                        r_buffer->popularidade = -1;
                    }
                    
                    break;
                case 3:
                    if (strcmp(token, "") != 0) {
                        r_buffer->tecnologiaDestino.string = strdup(token);
                        r_buffer->tecnologiaDestino.tamanho = strlen(token);
                    } else {
                        r_buffer->tecnologiaDestino.string = NULL;
                    }
                    break;
                case 4:
                    if (strcmp(token, ",") != 0) {
                    // Check if token is not an empty string or consists only of whitespace
                        int i;
                        int onlyWhitespace = 1;  // Assume that token consists only of whitespace
                        for (i = 0; token[i] != '\0'; i++) {
                            // Check if the current character is not a whitespace character
                            if (token[i] != ' ' && token[i] != '\t' && token[i] != '\n' && token[i] != '\r') {
                                onlyWhitespace = 0;  // Found a non-whitespace character
                                break;
                            }
                        }
                        // If token is not empty or doesn't consist only of whitespace
                        if (!onlyWhitespace) {
                            r_buffer->peso = atoi(token);  // Convert token to integer and assign to peso
                        }
                    }
                    if (strlen(token) == 0) {
                        r_buffer->peso = -1;  // If token is an empty string, assign -1 to peso
                    }
                break;
                default:
                    break;
            }
            field++;
        }
        
        /*
        printf("origem: %s,\n", r_buffer->tecnologiaOrigem.string);
        printf("grupo: %d,\n", r_buffer->grupo);
        printf("popularidade: %d,\n", r_buffer->popularidade);
        printf("destino: %s,\n", r_buffer->tecnologiaDestino.string);
        printf("peso: %d", r_buffer->peso);
        printf("\n--------------------------------------\n");
        */

        // pula a primeira linha
        if(cont_registros > 1) escreverRegistro(BIN_out, r_buffer);
        
    }
    
    cabecalho->proxRRN = cont_registros; // numero de registros
    fseek(BIN_out, 0, SEEK_SET); // volta ao inicio do arquivo
    binarioNaTela(caminhoBin);

    // libera a memoria
    free(r_buffer->tecnologiaOrigem.string);
    free(r_buffer->tecnologiaDestino.string);
    free(r_buffer);
    free(cabecalho);

    // fecha os arquivos
    fclose(CSV_in);
    fclose(BIN_out);
    //cabecalho->status = '1';
}       

void leitura_e_imprime(char* caminhoBin) {

    Registro *r_buffer = novo_registro();
    int size;
    int lixo = 0;

    FILE *BIN_out = fopen(caminhoBin, "rb");
    if(BIN_out == NULL) return;
    fseek(BIN_out, 0, SEEK_SET);
    while(fread(&(r_buffer->removido), sizeof(char), 1, BIN_out) != 0) {
        size++;
        fread(&(r_buffer->grupo), sizeof(int), 1, BIN_out);
        printf("%d, ", r_buffer->grupo);

        fread(&(r_buffer->peso), sizeof(int), 1, BIN_out);
        printf("%d, ", r_buffer->peso);

        fread(&(r_buffer->popularidade), sizeof(int), 1, BIN_out);
        printf("%d, ", r_buffer->popularidade);

        fread(&(r_buffer->tecnologiaOrigem.tamanho), sizeof(int), 1, BIN_out);
        printf("%d, ", r_buffer->tecnologiaOrigem.tamanho);

        fread(r_buffer->tecnologiaOrigem.string, sizeof(char), (r_buffer->tecnologiaOrigem.tamanho), BIN_out);
        r_buffer->tecnologiaOrigem.string[r_buffer->tecnologiaOrigem.tamanho] = '\0';
        printf("%s, ", r_buffer->tecnologiaOrigem.string);

        fread(&(r_buffer->tecnologiaDestino.tamanho), sizeof(int), 1, BIN_out);
        printf("%d, ", r_buffer->tecnologiaDestino.tamanho);

        fread(r_buffer->tecnologiaDestino.string, sizeof(char), (r_buffer->tecnologiaDestino.tamanho), BIN_out);
        r_buffer->tecnologiaDestino.string[r_buffer->tecnologiaDestino.tamanho] = '\0';
        printf("%s\n", r_buffer->tecnologiaDestino.string);


        lixo = 76 - (21 + (r_buffer->tecnologiaDestino.tamanho) + (r_buffer->tecnologiaOrigem.tamanho));
        fseek(BIN_out, lixo, SEEK_CUR);
    }
    free(r_buffer->tecnologiaOrigem.string);
    free(r_buffer->tecnologiaDestino.string);
    free(r_buffer);
    fclose(BIN_out);
}
