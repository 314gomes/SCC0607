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
    Registro r_buffer;
    
    FILE *CSV_in = fopen(caminhoCSV, "r");
    FILE *BIN_out = fopen(caminhoBin, "wb");
    
    // Cannot open file
    if (CSV_in == NULL) {
        printf("Error opening the file_in.\n");
        return;
    }

    while (fgets(CSV_line_buffer, sizeof(CSV_line_buffer), CSV_in)) {        
        r_buffer.tecnologiaOrigem.string = NULL;
        r_buffer.tecnologiaOrigem.tamanho = 0;
        r_buffer.tecnologiaDestino.string = NULL;
        r_buffer.tecnologiaDestino.tamanho = 0;
        r_buffer.grupo = -1;
        r_buffer.popularidade = -1;
        r_buffer.peso = -1;

        r_buffer.tecnologiaOrigem.string = (char *)malloc(TAM_MAXIMO_STRING);
        r_buffer.tecnologiaDestino.string =(char *)malloc(TAM_MAXIMO_STRING);

        char *token = NULL;
        int field = 0;
        char *saveptr = CSV_line_buffer;

        while ((token = strsep(&saveptr, ",")) != NULL) {
            switch (field) {
                case 0:               
                    if (strcmp(token, "") != 0) {
                        r_buffer.tecnologiaOrigem.string = strdup(token);
                        r_buffer.tecnologiaOrigem.tamanho = strlen(token);
                    } else {
                        r_buffer.tecnologiaOrigem.string = NULL;
                    }
                    
                    break;
                case 1:
                    if (strcmp(token, "") != 0) {
                        r_buffer.grupo = atoi(token);
                    } else {
                        r_buffer.grupo = -1;
                    }
                    break;
                case 2:
                    if (strcmp(token, "") != 0) {
                        r_buffer.popularidade = atoi(token);                    
                    } else {
                        r_buffer.popularidade = -1;
                    }
                    
                    break;
                case 3:
                    if (strcmp(token, "") != 0) {
                        r_buffer.tecnologiaDestino.string = strdup(token);
                        r_buffer.tecnologiaDestino.tamanho = strlen(token);
                    } else {
                        r_buffer.tecnologiaDestino.string = NULL;
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
                            r_buffer.peso = atoi(token);  // Convert token to integer and assign to peso
                        }
                    }
                    if (strlen(token) == 0) {
                        r_buffer.peso = -1;  // If token is an empty string, assign -1 to peso
                    }
                break;
                default:
                    break;
            }
            field++;
        }
        r_buffer.removido = '0';
        //printf("origem: %s,\n", r_buffer.tecnologiaOrigem.string);
        //printf("grupo: %d,\n", r_buffer.grupo);
        //printf("popularidade: %d,\n", r_buffer.popularidade);
        //printf("destino: %s,\n", r_buffer.tecnologiaDestino.string);
        //printf("peso: %d", r_buffer.peso);
        //printf("\n--------------------------------------\n");
        escreverRegistro(BIN_out, &r_buffer);
        
    }
    fclose(CSV_in);
    fclose(BIN_out);
}       

