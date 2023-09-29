#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
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

void escreverCabecalho(FILE *arquivo, Cabecalho *h){
    escreverCampoChar(arquivo, h->status);
    escreverCampoInt(arquivo, h->proxRRN);
    escreverCampoInt(arquivo, h->nroTecnologias);
    escreverCampoInt(arquivo, h->nroParesTecnologias);
}

int isempty(const char *s)
{
	if ((*s) == '\0') return 1;

	while (*s) {
	
	if (!isspace(*s))
		return 0;
	s++;
	}
	return 1;
}

void parseTokenInt(char* tkn, int *n){
    if (isempty(tkn)){
        *n = -1;
    } else {
        *n = atoi(tkn);
    }
}

void parseTokenStringVariavel(char* tkn, StringVariavel *str){
    if (isempty(tkn)){
        *(str->string) = '\0';
        str->tamanho = 0;
    } else {
        strcpy(str->string, tkn);
        str->tamanho = strlen(tkn);
    }
}

void parseLinhaCSV(char *CSV_line, Registro *r_buffer){
    int field = 0;
    char *tkn = NULL;

    while ((tkn = strsep(&CSV_line, ",")) != NULL) {
        switch (field) {
            case 0:               
                parseTokenStringVariavel(tkn, &(r_buffer->tecnologiaOrigem));
                break;
            case 1:
                parseTokenInt(tkn, &(r_buffer->grupo));
                break;
            case 2:
                parseTokenInt(tkn, &(r_buffer->popularidade));
                break;
            case 3:
                parseTokenStringVariavel(tkn, &(r_buffer->tecnologiaDestino));
                break;
            case 4:
                parseTokenInt(tkn, &(r_buffer->peso));
                break;
            default:
                break;
        }
        field++;
    }
}

void parseCSV(FILE *CSV_in, FILE *BIN_out, Cabecalho *c_buffer){
    Registro *r_buffer = novo_registro();
    char CSV_line_buffer[100];
    
    // le primeira linha mas nao escreve
    fgets(CSV_line_buffer, sizeof(CSV_line_buffer), CSV_in);

    while (fgets(CSV_line_buffer, sizeof(CSV_line_buffer), CSV_in)) {
        c_buffer->proxRRN++;

        parseLinhaCSV(CSV_line_buffer, r_buffer);

        escreverRegistro(BIN_out, r_buffer);
    }

    free(r_buffer->tecnologiaOrigem.string);
    free(r_buffer->tecnologiaDestino.string);
    free(r_buffer);
}

void csvParaBinario(char* caminhoCSV, char* caminhoBin){

    // inicializa novo c_buffer
    Cabecalho *c_buffer = novo_cabecalho();
    
    // abre arquivos de leitura e escrita em binario
    FILE *CSV_in = fopen(caminhoCSV, "r");
    FILE *BIN_out = fopen(caminhoBin, "wb");
    
    // verifica se foram abertos corretamente
    if (CSV_in == NULL) return;
    if (BIN_out == NULL) return;

    escreverCabecalho(BIN_out, c_buffer);

    // le o arquivo csv e escreve no arquivo binario
    parseCSV(CSV_in, BIN_out, c_buffer);

    fseek(BIN_out, 0, SEEK_SET); // volta ao inicio do arquivo
    binarioNaTela(caminhoBin);

    // libera a memoria
    free(c_buffer);

    // fecha os arquivos
    fclose(CSV_in);
    fclose(BIN_out);
    //c_buffer->status = '1';
}       

int imprime_int (int parametro){
    if (parametro == -1) {
        printf("NULO, ");
        return 0;
    }
    else {
        printf("%d, ", parametro);
        if (parametro != 0) return 1;
        else return 0;
    }
}
void leitura_e_imprime(char* caminhoBin) {

    Registro *r_buffer = novo_registro();
    int size;
    int lixo = 0;

    FILE *BIN_out = fopen(caminhoBin, "rb");
    if(BIN_out == NULL) return;
    fseek(BIN_out, 0, SEEK_SET);
    fseek(BIN_out, 13, SEEK_CUR);
    while(fread(&(r_buffer->removido), sizeof(char), 1, BIN_out) != 0) {
        size++;
        fread(&(r_buffer->grupo), sizeof(int), 1, BIN_out);
        imprime_int (r_buffer->grupo);

        fread(&(r_buffer->peso), sizeof(int), 1, BIN_out);
        imprime_int (r_buffer->peso);

        fread(&(r_buffer->popularidade), sizeof(int), 1, BIN_out);
        imprime_int (r_buffer->popularidade);

        fread(&(r_buffer->tecnologiaOrigem.tamanho), sizeof(int), 1, BIN_out);
        if(imprime_int (r_buffer->tecnologiaOrigem.tamanho)){
            fread(r_buffer->tecnologiaOrigem.string, sizeof(char), (r_buffer->tecnologiaOrigem.tamanho), BIN_out);
            r_buffer->tecnologiaOrigem.string[r_buffer->tecnologiaOrigem.tamanho] = '\0';
            printf("%s, ", r_buffer->tecnologiaOrigem.string);
        }
        else {
            printf("NULO, ");
        }

        fread(&(r_buffer->tecnologiaDestino.tamanho), sizeof(int), 1, BIN_out);
        if(imprime_int (r_buffer->tecnologiaDestino.tamanho)){
            fread(r_buffer->tecnologiaDestino.string, sizeof(char), (r_buffer->tecnologiaDestino.tamanho), BIN_out);
            r_buffer->tecnologiaDestino.string[r_buffer->tecnologiaDestino.tamanho] = '\0';
            printf("%s\n", r_buffer->tecnologiaDestino.string);
        }
        else {
            printf("NULO\n");
        }

        lixo = 76 - (21 + (r_buffer->tecnologiaDestino.tamanho) + (r_buffer->tecnologiaOrigem.tamanho));
        fseek(BIN_out, lixo, SEEK_CUR);
    }
    free(r_buffer->tecnologiaOrigem.string);
    free(r_buffer->tecnologiaDestino.string);
    free(r_buffer);
    fclose(BIN_out);
}