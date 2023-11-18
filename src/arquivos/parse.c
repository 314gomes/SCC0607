#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include "arquivos/parse.h"
#include "arquivos/tipos.h"
#include "arquivos/utils.h"
#include "arquivos/escrita.h"
#include "listaSE.h"

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

    // free memory from linked list
    destroiLista(&tec);

    free_registro(r_buffer);
}
