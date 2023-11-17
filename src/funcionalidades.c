#include <stdlib.h>
#include <string.h>
#include "funcionalidades.h"
#include "funcoesFornecidas.h"

StatusDeRetorno funcionalidade1 (char* caminhoCSV, char* caminhoBin){

    // initializes a new c_buffer
    Cabecalho *c_buffer = novo_cabecalho();
    
    // opens binary files for reading and writing
    // checking if they were opened correctly
    FILE *CSV_in = fopen(caminhoCSV, "r");
    if (CSV_in == NULL) return falha_processamento;
    FILE *BIN_out = fopen(caminhoBin, "wb");
    if (BIN_out == NULL) return falha_processamento;
    

    escreverCabecalho(BIN_out, c_buffer);

    // reads the CSV file and writes to the binary file
    parseCSV(CSV_in, BIN_out, c_buffer);

    // Writes updated binary file header
    c_buffer->status = CONSISTENTE;
    escreverCabecalho(BIN_out, c_buffer);

    // Closes files
    fclose(CSV_in);
    fclose(BIN_out);

    binarioNaTela(caminhoBin);

    // frees memory
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

    // reading and printing of the binary file
    while(fread(&(r->removido), sizeof(char), 1, BIN_out) != 0) {
       
       if (r->removido == NAO_REMOVIDO) {
            status = sucesso;
            leConteudoRegistro(BIN_out, r);
            imprimeRegistro(*r);

            int tam_lixo = TAM_REGISTRO - calcularTamanhoRegistro(r);
            fseek(BIN_out, tam_lixo, SEEK_CUR);
       }
       else{
            fseek(BIN_out, TAM_REGISTRO - 1, SEEK_CUR);
       }
       
    }
   
    free_registro(r);
    fclose(BIN_out);
    return status;
}

StatusDeRetorno funcionalidade3 (char* caminhoBin, int n) {

    // stores the field that will be searched
    char busca_campo[n][TAM_MAXIMO_STRING]; 

    // stores the sought-after integer
    int busca_int[n];
    // stores the sought-after variable-length string
    char busca_char[n][TAM_MAXIMO_STRING];

    // sets the position of the field in the record for integers
    // assigns the value -1 when dealing with variable-length strings
    int campo[n];

    // stores the number of characters in the sought-after string
    int tamanho[n];

    // 0 when dealing with the first variable string
    // 1 when dealing with the second variable string
    int flag[n];

    for (int i = 0; i < n; i++) {
        scanf("%s", busca_campo[i]);

        if (strcmp(busca_campo[i], "grupo") == 0) {
            scanf("%d", &busca_int[i]);
            campo[i] = 0;           
        }
        else if (strcmp(busca_campo[i], "popularidade") == 0) {
            scanf("%d", &busca_int[i]);
            campo[i] = 4;
        }
        else if (strcmp(busca_campo[i], "peso") == 0) {
            scanf("%d", &busca_int[i]);
            campo[i] = 8;
        } 
        else if (strcmp(busca_campo[i], "nomeTecnologiaOrigem") == 0) {
			scan_quote_string(busca_char[i]);
            tamanho[i] = strlen(busca_char[i]);
            campo[i] = -1;
            flag[i] = 0;
        } 
        else if (strcmp(busca_campo[i], "nomeTecnologiaDestino") == 0) {
            scan_quote_string(busca_char[i]);
            tamanho[i] = strlen(busca_char[i]);
            campo[i] = -1;
            flag[i] = 1;
        }     
    }

    // flag to check if no sought-after records exist
    int st = 0;
    StatusDeRetorno status = sucesso;
    

    for (int i = 0; i < n; i++) {
        
        if(campo[i] > -1) {
            status = buscaCampoInt(caminhoBin, campo[i], busca_int[i]);
        }

        if (campo[i] == -1) {
            status = buscaCampoStringVariavel(caminhoBin, busca_char[i], tamanho[i], flag[i]);
        }

        // in case the search functions return falha_processamento
        if (status == falha_processamento) return falha_processamento;

        // in case the search functions do not find the desired field
        if (status == registro_inexistente) st++;
    }

    // if all return "registro_inexistente", st will be equal to the number of searches n
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
    free_registro(r_buffer);
    fclose(bin);
    return sucesso;
}