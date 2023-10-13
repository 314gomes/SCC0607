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

int isempty(const char *s)
{
    // string only contains \0
	if ((*s) == '\0') return 1;

    // iterate over string
	while (*s) {
        // if a single nonempty character was found, return false
        if (!isspace(*s))
            return 0;
        s++;
	}

    // otherwiser return true
	return 1;
}

void parseTokenInt(char* tkn, int *n){
    // specification says empty integers must receive value -1
    if (isempty(tkn)){
        *n = -1;
    } else {
        *n = atoi(tkn);
    }
}

void parseTokenStringVariavel(char* tkn, StringVariavel *str){
    // empty strings remain empty and have size 0
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

    // iterate over each token from csv line, each separated by ','
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
    // initialize buffer variables and list of technologies
    Registro *r_buffer = novo_registro();
    char CSV_line_buffer[100];
    listaSE tec = novaLista();

    // skips first line
    fgets(CSV_line_buffer, sizeof(CSV_line_buffer), CSV_in);

    // iterate over each line of the CSV file    
    while (fgets(CSV_line_buffer, sizeof(CSV_line_buffer), CSV_in)) {
        c_buffer->proxRRN++;

        // parse line to buffer
        parseLinhaCSV(CSV_line_buffer, r_buffer);

        // some temporary variables for readability
        char *strOrigem = r_buffer->tecnologiaOrigem.string;
        char *strDestino = r_buffer->tecnologiaDestino.string;

        int origemIsNull = isempty(strOrigem);
        int destinoIsNull = isempty(strDestino);

        // pair exists if both tec origem and tec destino are not null
        int parExists = !(origemIsNull) && !(destinoIsNull);


        // add non-null technologies to list of technologies without repetitions
        if(!origemIsNull)
            insereOrdenadoSemRepeticao(strOrigem, &tec);
        if(!destinoIsNull)
            insereOrdenadoSemRepeticao(strDestino, &tec);

        // increase technology pairs accordingly
        if(parExists)
            c_buffer->nroParesTecnologias++;

        // write buffer to file
        escreverRegistro(BIN_out, r_buffer);
    }

    // number of different technologies is then just the size of our list
    c_buffer->nroTecnologias = tec.tam;

    free(r_buffer->tecnologiaOrigem.string);
    free(r_buffer->tecnologiaDestino.string);
    free(r_buffer);
}

StatusDeRetorno csvParaBinario(char* caminhoCSV, char* caminhoBin){

    // inicializa novo c_buffer
    Cabecalho *c_buffer = novo_cabecalho();
    
    // abre arquivos de leitura e escrita em binario
    // verificando se foram abertos corretamente
    FILE *CSV_in = fopen(caminhoCSV, "r");
    if (CSV_in == NULL) return falha_processamento;
    FILE *BIN_out = fopen(caminhoBin, "wb");
    if (BIN_out == NULL) return falha_processamento;
    
    // Writes header initialy empty and marked as 
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

void imprime_int (int n){
    // Specification says -1 should be printed as "NULO"
    if (n == -1) {
        printf("NULO");
    }
    else {
        printf("%d", n);
    }
}

void imprime_StringVariavel(StringVariavel str){
    // specification says zero-lenght strings must be printed as "NULO"
    if(str.tamanho == 0){
        printf("NULO");
    }
    else {
        printf("%s", str.string);
    }
}

void imprime_separador(){
    printf(", "); // token separator according to specification
}

void imprime_registro(Registro r){
    // nomeTecnologiaOrigem, grupo, popularidade, nomeTecnologiaDestino, peso
    imprime_StringVariavel(r.tecnologiaOrigem);
    imprime_separador();
    
    imprime_int(r.grupo);
    imprime_separador();
    
    imprime_int(r.popularidade);
    imprime_separador();
    
    imprime_StringVariavel(r.tecnologiaDestino);
    imprime_separador();

    imprime_int(r.popularidade);
    printf("\n");
}

FILE *abreBinario(char *caminhoBin){
    char status;
    
    // Open in read mode
    FILE *bin = fopen(caminhoBin, "rb");
    
    // fopen errors result in NULL
    if(bin == NULL){
        return NULL;
    }

    // read file status
    fseek(bin, 0, SEEK_SET);
    fread(&status, 1, 1, bin);
    
    // Inconsistent status results in NULL (error)
    if(status == INCONSISTENTE){
        // close file!
        fclose(bin);
        return NULL;
    }
    
    // If no errors encountered, return pointer to file
    return bin;
}

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
        printf("%s", r_buffer->tecnologiaOrigem.string);
    }
    else {
        printf("NULO");
    }
    imprime_int (r_buffer->grupo);
    imprime_int (r_buffer->popularidade);
    if((r_buffer->tecnologiaOrigem.tamanho) != 0){
        r_buffer->tecnologiaDestino.string[r_buffer->tecnologiaDestino.tamanho] = '\0';
        printf(", %s", r_buffer->tecnologiaDestino.string);
    }
    else {
        printf(", NULO");
    }
    imprime_int (r_buffer->peso);
    printf("\n");

    free(r_buffer);
    fclose(BIN_out);

}

void leitura_e_imprime(char* caminhoBin) {

    Registro *r_buffer = novo_registro();
    int posicao = 13;

    FILE *BIN_out = fopen(caminhoBin, "rb");
    if(BIN_out == NULL) return;
    fseek(BIN_out, 0, SEEK_SET);
    fseek(BIN_out, 13, SEEK_CUR);
    /*
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
    */

    while(fread(&(r_buffer->removido), sizeof(char), 1, BIN_out) != 0) {
        func3_aux(caminhoBin, posicao);

        posicao += 76;
        fseek(BIN_out, 75, SEEK_CUR);
    }
    

    
    free(r_buffer->tecnologiaOrigem.string);
    free(r_buffer->tecnologiaDestino.string);
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
        

        if(campo[i] > -1) {
            busca_int(caminhoBin, campo[i], busca_i[i]);
        }

        if (campo[i] == -1) {
            busca_string(caminhoBin, busca_c[i], tamanho[i], flag[i]);
        }

    }
}

StatusDeRetorno funcionalidade4(char* caminhoBin, int RRN){
    FILE* bin = abreBinario(caminhoBin);
    
    if(bin == NULL){
        return falha_processamento;
    }

    Registro *r_buffer = novo_registro();
    StatusDeRetorno s = le_RRN(bin, RRN, r_buffer);

    if(s != sucesso) return s;

    imprime_registro(*r_buffer);
    return sucesso;
}