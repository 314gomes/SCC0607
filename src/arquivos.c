#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "arquivos.h"
#include "funcoesFornecidas.h"
#include "listaSE.h"

/***************** INICIALIZACOES **********************/
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

    return r_buffer;
}
Cabecalho *novo_cabecalho() {

    Cabecalho* cabecalho;
    cabecalho = (Cabecalho*)malloc(sizeof(Cabecalho));

    cabecalho->status = INCONSISTENTE;
    cabecalho->proxRRN = 0;
    cabecalho->nroTecnologias = 0;
    cabecalho->nroParesTecnologias = 0;

    return cabecalho;
}

int calcularTamanhoRegistro(Registro *r){
    int tamanhoRegistro = TAM_REGISTRO_FIXO;
    tamanhoRegistro += r->tecnologiaOrigem.tamanho;
    tamanhoRegistro += r->tecnologiaDestino.tamanho;
    return tamanhoRegistro;
}

FILE *abreBinario(char *caminhoBin){
    char status;
    
    FILE *bin = abreBinario(caminhoBin);
    if(bin == NULL){
        return NULL;
    }

    fseek(bin, 0, SEEK_SET);
    fread(&status, 1, 1, bin);
    
    if(status == INCONSISTENTE){
        fclose(bin);
        return NULL;
    }
    
    return bin;
}


/***************** ESCRITA **********************/
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
    fseek(arquivo, 0, 0);
    escreverCampoChar(arquivo, h->status);
    escreverCampoInt(arquivo, h->proxRRN);
    escreverCampoInt(arquivo, h->nroTecnologias);
    escreverCampoInt(arquivo, h->nroParesTecnologias);
}


/***************** PARSE **********************/
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

        char *strOrigem = r_buffer->tecnologiaOrigem.string;
        char *strDestino = r_buffer->tecnologiaDestino.string;

        int origemIsNull = isempty(strOrigem);
        int destinoIsNull = isempty(strDestino);

        int parExists = !(origemIsNull) && !(destinoIsNull);

        if(!origemIsNull)
            insereOrdenadoSemRepeticao(strOrigem, &tec);
        if(!destinoIsNull)
            insereOrdenadoSemRepeticao(strDestino, &tec);

        if(parExists)
            c_buffer->nroParesTecnologias++;

        escreverRegistro(BIN_out, r_buffer);
    }

    c_buffer->nroTecnologias = tec.tam;

    free(r_buffer->tecnologiaOrigem.string);
    free(r_buffer->tecnologiaDestino.string);
    free(r_buffer);
}
       

/***************** IMPRESSAO **********************/
void imprimeCampoInt (int n){
    if (n == -1) {
        printf("NULO");
    }
    else {
        printf("%d", n);
    }
}

void imprimeCampoStringVariavel(StringVariavel str){
    if(str.tamanho == 0){
        printf("NULO");
    }
    else {
        printf("%s", str.string);
    }
}

void imprimeSeparador(){
    printf(", ");
}

void imprimeRegistro(Registro r){
    // nomeTecnologiaOrigem, grupo, popularidade, nomeTecnologiaDestino, peso,
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


/***************** LEITURA **********************/
long byteoffset_RRN(int RRN){
    return TAM_CABECALHO + TAM_REGISTRO*RRN;
}

void leCampoInt(FILE* bin, int *i){
    fread(i, sizeof(int), 1, bin);
}

void leCampoChar(FILE* bin, char *c){
    fread(c, sizeof(char), 1, bin);
}

void leCampoStringVariavel(FILE* bin, StringVariavel *s){
    leCampoInt(bin, &s->tamanho);
    if(s->tamanho > 0){
        fread(s->string, sizeof(char), s->tamanho, bin);
    }
    // write string ending character to end of string
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
    long byte_offset = byteoffset_RRN(RRN);
    fseek(bin, byte_offset, SEEK_SET);
    
    leStatusRegistro(bin, r);
    
    if(feof(bin)){
        return registro_inexistente;
    }
    if(r->removido == REMOVIDO){
        return registro_inexistente;
    }

    leConteudoRegistro(bin, r);

    return sucesso;
}


/***************** BUSCA **********************/
void auxiliarFuncionalidade3 (char* caminhoBin, int posicao) {
    Registro *r = novo_registro();
    FILE *BIN_out = abreBinario(caminhoBin);
    if(BIN_out == NULL) return;

    
    fseek(BIN_out, 0, SEEK_SET);
    fseek(BIN_out, posicao, SEEK_CUR);


    leStatusRegistro(BIN_out, r); 
    leConteudoRegistro(BIN_out, r); 
    imprimeRegistro(*r);

    free(r);
    fclose(BIN_out);

}

StatusDeRetorno buscaCampoInt (char* caminhoBin, int campo, int buscado) {
    Registro *r = novo_registro();
    FILE *BIN_out = abreBinario(caminhoBin);

    StatusDeRetorno status = registro_inexistente;
    if(BIN_out == NULL) return falha_processamento;
    
    int lixo = 0, linha = 0, aux_int;
    char aux_char;

    fseek(BIN_out, 0, SEEK_SET);
    fseek(BIN_out, TAM_CABECALHO, SEEK_CUR);

    while(fread(&(aux_char), sizeof(char), 1, BIN_out) != 0) {
        if (aux_char == REMOVIDO) lixo = TAM_REGISTRO - 1;

        else {   
        status = sucesso;
        fseek(BIN_out, campo, SEEK_CUR);
        fread(&(aux_int), sizeof(int), 1, BIN_out);
        
        
        if (aux_int == buscado) {
            auxiliarFuncionalidade3(caminhoBin, (linha*TAM_REGISTRO)+TAM_CABECALHO);
        }    
        lixo = TAM_REGISTRO - (1 + (campo+4));
        }

        linha++;
        fseek(BIN_out, lixo, SEEK_CUR);
    }

    free(r);
    fclose(BIN_out);

    return status;
}

StatusDeRetorno buscaCampoStringVariavel (char* caminhoBin, char* buscado, int tamanho, int flag) {
    Registro *r = novo_registro();
    FILE *BIN_out = abreBinario(caminhoBin);
    StatusDeRetorno status = registro_inexistente;
    if(BIN_out == NULL) return falha_processamento;

    int campo = 12;
    int linha = 0;
    int aux_tamanho[2], lixo = 0;
    char aux_char, aux_string[TAM_MAXIMO_STRING];
    


    fseek(BIN_out, 0, SEEK_SET);
    fseek(BIN_out, TAM_CABECALHO, SEEK_CUR);
    while(fread(&(aux_char), sizeof(char), 1, BIN_out) != 0) {
        if (aux_char == REMOVIDO) lixo = TAM_REGISTRO - 1;

        else {
            status = sucesso;
            fseek(BIN_out, campo, SEEK_CUR);
            fread(&(aux_tamanho[0]), sizeof(int), 1, BIN_out);

            switch(flag) {
                case 0:
                    if (aux_tamanho[0] == tamanho) {
                        fread(&(aux_string), sizeof(char), aux_tamanho[0], BIN_out);
                        lixo = TAM_REGISTRO - (17+tamanho);

                        aux_string[tamanho] = '\0';
                        buscado[tamanho] = '\0';

                        if (strcmp(aux_string, buscado) == 0) {
                            auxiliarFuncionalidade3(caminhoBin, (linha*TAM_REGISTRO)+TAM_CABECALHO);
                        }
                    }
                    else {
                        lixo = TAM_REGISTRO -(17);
                    }
                break;
                case 1:
                    fseek(BIN_out, aux_tamanho[0], SEEK_CUR);
                    fread(&(aux_tamanho[1]), sizeof(int), 1, BIN_out);
                    
                    if (aux_tamanho[1] == tamanho) {
                        fread(&(aux_string), sizeof(char), aux_tamanho[1], BIN_out);
                        lixo = TAM_REGISTRO - (1+20+aux_tamanho[0]+tamanho);

                        aux_string[tamanho] = '\0';
                        buscado[tamanho] = '\0';

                        if (strcmp(aux_string, buscado) == 0) {
                            auxiliarFuncionalidade3(caminhoBin, (linha*TAM_REGISTRO)+TAM_CABECALHO);
                        }
                    }
                    else {
                        lixo = TAM_REGISTRO -(1+20+aux_tamanho[0]);
                    }
                break;
            }
        }
        linha++;
        fseek(BIN_out, lixo, SEEK_CUR);
    }

    return status;

    free(r);
    fclose(BIN_out);
}


/***************** FUNCIONALIDADES **********************/
StatusDeRetorno funcionalidade1 (char* caminhoCSV, char* caminhoBin){

    // inicializa novo c_buffer
    Cabecalho *c_buffer = novo_cabecalho();
    
    // abre arquivos de leitura e escrita em binario
    // verificando se foram abertos corretamente
    FILE *CSV_in = fopen(caminhoCSV, "r");
    if (CSV_in == NULL) return falha_processamento;
    FILE *BIN_out = fopen(caminhoBin, "wb");
    if (BIN_out == NULL) return falha_processamento;
    

    escreverCabecalho(BIN_out, c_buffer);

    // le o arquivo csv e escreve no arquivo binario
    parseCSV(CSV_in, BIN_out, c_buffer);

    // Writes updated binary file header
    c_buffer->status = CONSISTENTE;
    escreverCabecalho(BIN_out, c_buffer);

    // Closes files
    fclose(CSV_in);
    fclose(BIN_out);

    binarioNaTela(caminhoBin);

    // libera a memoria
    free(c_buffer);

    return sucesso;
}

StatusDeRetorno funcionalidade2 (char* caminhoBin) {

    Registro *r = novo_registro();
    StatusDeRetorno status = registro_inexistente;

    FILE *BIN_out = abreBinario(caminhoBin);
    if(BIN_out == NULL) return falha_processamento;

    fseek(BIN_out, 0, SEEK_SET);
    fseek(BIN_out, TAM_CABECALHO, SEEK_CUR);

    while(fread(&(r->removido), sizeof(char), 1, BIN_out) != 0) {
       
       if (r->removido == NAO_REMOVIDO) {
            status = sucesso;
            leConteudoRegistro(BIN_out, r);
            imprimeRegistro(*r);
       }
       
    }
   
    free(r->tecnologiaOrigem.string);
    free(r->tecnologiaDestino.string);
    free(r);
    fclose(BIN_out);
    return status;
}

StatusDeRetorno funcionalidade3 (char* caminhoBin, int n) {

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

    //verificar se registros buscados existem
    StatusDeRetorno status = sucesso;
    int st = -1;

    for (int i = 0; i < n; i++) {
        
        if(campo[i] > -1) {
            status = buscaCampoInt(caminhoBin, campo[i], busca_i[i]);
        }

        if (campo[i] == -1) {
            status = buscaCampoStringVariavel(caminhoBin, busca_c[i], tamanho[i], flag[i]);
        }

        if (status == registro_inexistente) st++;
    }
    
    if (st == n) return registro_inexistente;
    else return sucesso;
}

StatusDeRetorno funcionalidade4 (char* caminhoBin, int RRN){
    FILE* bin = abreBinario(caminhoBin);
    
    if(bin == NULL) return falha_processamento;

    Registro *r_buffer = novo_registro();
    StatusDeRetorno s = le_RRN(bin, RRN, r_buffer);

    if(s != sucesso) return s;

    imprimeRegistro(*r_buffer);
    return sucesso;
}