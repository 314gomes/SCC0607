#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "arquivos.h"
#include "funcoesFornecidas.h"
#include "listaSE.h"

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
    listaSE tec = novaLista();

    // le primeira linha mas nao escreve
    fgets(CSV_line_buffer, sizeof(CSV_line_buffer), CSV_in);

    while (fgets(CSV_line_buffer, sizeof(CSV_line_buffer), CSV_in)) {
        c_buffer->proxRRN++;

        parseLinhaCSV(CSV_line_buffer, r_buffer);

        insereOrdenadoSemRepeticao(r_buffer->tecnologiaOrigem.string, &tec);
        insereOrdenadoSemRepeticao(r_buffer->tecnologiaDestino.string, &tec);

        escreverRegistro(BIN_out, r_buffer);
    }

    c_buffer->nroParesTecnologias = c_buffer->proxRRN;
    c_buffer->nroTecnologias = tec.tam;

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

    // Writes updated binary file header
    c_buffer->status = '1';
    fseek(BIN_out, 0, 0);
    escreverCabecalho(BIN_out, c_buffer);

    // Closes files
    fclose(CSV_in);
    fclose(BIN_out);

    binarioNaTela(caminhoBin);

    // libera a memoria
    free(c_buffer);
}       

int imprime_int (int parametro){
    if (parametro == -1) {
        printf("NULO");
        return 0;
    }
    else {
        printf("%d", parametro);
        if (parametro != 0) return 1;
        else return 0;
    }
}

void virgula () {
    printf(", ");
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
        virgula();

        fread(&(r_buffer->peso), sizeof(int), 1, BIN_out);
        imprime_int (r_buffer->peso);
        virgula();

        fread(&(r_buffer->popularidade), sizeof(int), 1, BIN_out);
        imprime_int (r_buffer->popularidade);
        virgula();

        fread(&(r_buffer->tecnologiaOrigem.tamanho), sizeof(int), 1, BIN_out);
        if(imprime_int (r_buffer->tecnologiaOrigem.tamanho)){
            fread(r_buffer->tecnologiaOrigem.string, sizeof(char), (r_buffer->tecnologiaOrigem.tamanho), BIN_out);
            r_buffer->tecnologiaOrigem.string[r_buffer->tecnologiaOrigem.tamanho] = '\0';
            printf(", %s, ", r_buffer->tecnologiaOrigem.string);
        }
        else {
            printf("NULO, ");
        }

        fread(&(r_buffer->tecnologiaDestino.tamanho), sizeof(int), 1, BIN_out);
        if(imprime_int (r_buffer->tecnologiaDestino.tamanho)){
            fread(r_buffer->tecnologiaDestino.string, sizeof(char), (r_buffer->tecnologiaDestino.tamanho), BIN_out);
            r_buffer->tecnologiaDestino.string[r_buffer->tecnologiaDestino.tamanho] = '\0';
            printf(", %s\n", r_buffer->tecnologiaDestino.string);
        }
        else {
            printf(", NULO\n");
        }

        lixo = 76 - (21 + (r_buffer->tecnologiaDestino.tamanho) + (r_buffer->tecnologiaOrigem.tamanho));
        fseek(BIN_out, lixo, SEEK_CUR);
    }
    free(r_buffer->tecnologiaOrigem.string);
    free(r_buffer->tecnologiaDestino.string);
    free(r_buffer);
    fclose(BIN_out);
}

void func3_aux (char* caminhoBin, int posicao) {
    Registro *r_buffer = novo_registro();
    FILE *BIN_out = fopen(caminhoBin, "rb");
    if(BIN_out == NULL) return;

    fseek(BIN_out, 0, SEEK_SET);
    fseek(BIN_out, posicao, SEEK_CUR);

    fread(&(r_buffer->removido), sizeof(char), 1, BIN_out);
    fread(&(r_buffer->grupo), sizeof(int), 1, BIN_out);
    fread(&(r_buffer->popularidade), sizeof(int), 1, BIN_out);
    fread(&(r_buffer->peso), sizeof(int), 1, BIN_out);
    

    fread(&(r_buffer->tecnologiaOrigem.tamanho), sizeof(int), 1, BIN_out);
    if((r_buffer->tecnologiaOrigem.tamanho) != 0){
        fread(r_buffer->tecnologiaOrigem.string, sizeof(char), (r_buffer->tecnologiaOrigem.tamanho), BIN_out);
        r_buffer->tecnologiaOrigem.string[r_buffer->tecnologiaOrigem.tamanho] = '\0';
    }

    fread(&(r_buffer->tecnologiaDestino.tamanho), sizeof(int), 1, BIN_out);
    if((r_buffer->tecnologiaDestino.tamanho) != 0){
        fread(r_buffer->tecnologiaDestino.string, sizeof(char), (r_buffer->tecnologiaDestino.tamanho), BIN_out);
        r_buffer->tecnologiaDestino.string[r_buffer->tecnologiaDestino.tamanho] = '\0';
    }

    // print
    if((r_buffer->tecnologiaOrigem.tamanho) != 0){
        r_buffer->tecnologiaOrigem.string[r_buffer->tecnologiaOrigem.tamanho] = '\0';
        printf("%s, ", r_buffer->tecnologiaOrigem.string);
    }
    else {
        printf("NULO, ");
    }
    imprime_int (r_buffer->grupo);
    virgula();
    imprime_int (r_buffer->popularidade);
    virgula();
    if((r_buffer->tecnologiaOrigem.tamanho) != 0){
        r_buffer->tecnologiaDestino.string[r_buffer->tecnologiaDestino.tamanho] = '\0';
        printf("%s, ", r_buffer->tecnologiaDestino.string);
    }
    else {
        printf("NULO, ");
    }
    imprime_int (r_buffer->peso);
    printf("\n");

    free(r_buffer);
    fclose(BIN_out);

}

void busca_int (char* caminhoBin, int campo, int buscado) {
    Registro *r_buffer = novo_registro();
    FILE *BIN_out = fopen(caminhoBin, "rb");
    if(BIN_out == NULL) return;
    
    int lixo = 0;
    int linha = 0;
    int aux_int;
    char aux_char;

    fseek(BIN_out, 0, SEEK_SET);
    fseek(BIN_out, 13, SEEK_CUR);
    while(fread(&(aux_char), sizeof(char), 1, BIN_out) != 0) {
        
        fseek(BIN_out, campo, SEEK_CUR);
        fread(&(aux_int), sizeof(int), 1, BIN_out);
        
        
        if (aux_int == buscado) {
            func3_aux(caminhoBin, (linha*76)+13);
        }
        linha++;
        lixo = 76 - (1 + (campo+1)*4);
        fseek(BIN_out, lixo, SEEK_CUR);
    }

    free(r_buffer);
    fclose(BIN_out);
}

void busca_string (char* caminhoBin, char* buscado, int tamanho, int flag) {
    Registro *r_buffer = novo_registro();
    FILE *BIN_out = fopen(caminhoBin, "rb");
    if(BIN_out == NULL) return;

    int campo = 12;
    int linha = 0;
    int aux_tamanho[2], lixo = 0;
    char aux_char, aux_string[TAM_MAXIMO_STRING];
    


    fseek(BIN_out, 0, SEEK_SET);
    fseek(BIN_out, 13, SEEK_CUR);
    while(fread(&(aux_char), sizeof(char), 1, BIN_out) != 0) {
        
        fseek(BIN_out, campo, SEEK_CUR);
        fread(&(aux_tamanho[0]), sizeof(int), 1, BIN_out);

        switch(flag) {
            case 0:
                if (aux_tamanho[0] == tamanho) {
                    fread(&(aux_string), sizeof(char), aux_tamanho[0], BIN_out);
                    lixo = 76 - (17+tamanho);

                    aux_string[tamanho] = '\0';
                    buscado[tamanho] = '\0';

                    if (strcmp(aux_string, buscado) == 0) {
                        func3_aux(caminhoBin, (linha*76)+13);
                    }
                }
                else {
                    lixo = 76 -(17);
                }
            break;
            case 1:
                fseek(BIN_out, aux_tamanho[0], SEEK_CUR);
                fread(&(aux_tamanho[1]), sizeof(int), 1, BIN_out);
                
                if (aux_tamanho[1] == tamanho) {
                    fread(&(aux_string), sizeof(char), aux_tamanho[1], BIN_out);
                    lixo = 76 - (1+20+aux_tamanho[0]+tamanho);

                    aux_string[tamanho] = '\0';
                    buscado[tamanho] = '\0';

                    if (strcmp(aux_string, buscado) == 0) {
                        func3_aux(caminhoBin, (linha*76)+13);
                    }
                }
                else {
                    lixo = 76 -(1+20+aux_tamanho[0]);
                }
            break;
        }
        linha++;
        fseek(BIN_out, lixo, SEEK_CUR);
    }

    free(r_buffer);
    fclose(BIN_out);
}

void funcionalidade3 (char* caminhoBin, int n) {

    char busca[n][TAM_MAXIMO_STRING];
    int busca_i[n];
    char busca_c[n][TAM_MAXIMO_STRING];
    int campo[n];
    int tamanho[n];
    int flag[n];

    for (int i = 0; i < n; i++) {
        scanf("%s", busca[i]);

        if (strcmp(busca[i], "grupo") == 0) {
            scanf("%d", &busca_i[i]);
            campo[i] = 0;           
        }
        else if (strcmp(busca[i], "popularidade") == 0) {
            scanf("%d", &busca_i[i]);
            campo[i] = 4;
        }
        else if (strcmp(busca[i], "peso") == 0) {
            scanf("%d", &busca_i[i]);
            campo[i] = 8;
        } 
        else if (strcmp(busca[i], "nomeTecnologiaOrigem") == 0) {
			scan_quote_string(busca_c[i]);
            tamanho[i] = strlen(busca_c[i]);
            campo[i] = -1;
            flag[i] = 0;
        } 
        else if (strcmp(busca[i], "nomeTecnologiaDestino") == 0) {
            scan_quote_string(busca_c[i]);
            tamanho[i] = strlen(busca_c[i]);
            campo[i] = -1;
            flag[i] = 1;
        }     
    }

// arrumar colocar uns if para quando for de string
    for (int i = 0; i < n; i++) {
        //printf("campo = %d\n\n", campo[i]);
        if (campo[i] = -1) {
            //puts("flag char");
            busca_string(caminhoBin, busca_c[i], tamanho[i], flag[i]);
        }
        else if (campo[i] > -1){
            puts("flag int");
            busca_int(caminhoBin, campo[i], busca_i[i]);
        }
    }
}