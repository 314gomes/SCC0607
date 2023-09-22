#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "arquivos.h"

int main() {
    FILE *file;
    Registro *registro = NULL;
    int size = 0;
    int cont = 0;
    char line[100]; // suppose each line of the file has a maximum of 100 characters

    // open csv
    FILE *file_in = fopen("tecnologia.csv", "r");
    FILE *file_out = fopen("tecnologia.bin", "wb");

    if (file_in == NULL) {
        printf("Error opening the file_in.\n");
        return 1;
    }

    // parse csv
    while (fgets(line, sizeof(line), file_in)) {

        // allocates space for the new struct
        registro = (Registro *)realloc(registro, (size + 1) * sizeof(Registro));
        if (registro == NULL) {
            printf("Error while allocating memory.\n");
            return 1;
        }

        // initializes the struct fields
        registro[size].tecnologiaOrigem.string = NULL;
        registro[size].tecnologiaDestino.string = NULL;

        // allocates space for variable size fields
        registro[size].tecnologiaOrigem.string = (char *)malloc(50); // 50 é um size arbitrário
        registro[size].tecnologiaDestino.string = (char *)malloc(50);


        // reads values and allocates memory for variable-sized fields
        sscanf(line, "%49[^,],%d,%d,%49[^,],%d",
               registro[size].tecnologiaOrigem.string,
               &registro[size].grupo,
               &registro[size].popularidade,
               registro[size].tecnologiaDestino.string,
               &registro[size].peso);

        registro[size].removido = '0';
        registro[size].tecnologiaOrigem.tamanho = strlen(registro[size].tecnologiaOrigem.string);
        registro[size].tecnologiaDestino.tamanho = strlen(registro[size].tecnologiaDestino.string);

        escreverRegistro(file_out, registro);

        // updates the size and counter
        size++;
        cont++;

        // verifies that we do not exceed the maximum registration limit
        if (cont >= TAMANHO_MAXIMO) {
            printf("maximum registration limit\n");
            break;
        }
    }

    fclose(file_in);
    fclose(file_out);

    // print
    for (int i = 1; i < 5; i++) {
        //printf("%d:\n", i + 1);
        printf("%s,", registro[i].tecnologiaOrigem.string);
        //printf("%d,", registro[i].tecnologiaOrigem.tamanho);
        printf("%d,", registro[i].grupo);
        printf("%d,", registro[i].popularidade);
        printf("%d,", registro[i].peso);
        printf("%s", registro[i].tecnologiaDestino.string);
        printf("\n");
    }

    // calculating the size of line 1
    int sizeRegistro = sizeof(char) +  // removed
                         sizeof(int) * 3 + // grupo, popularidade, peso
                         strlen(registro[1].tecnologiaOrigem.string) + 1 + // tecnologiaOrigem.string + '\0'
                         strlen(registro[1].tecnologiaDestino.string) + 1; // tecnologiaDestino.string + '\0'

    //printf("\n\nThe size of line 1 is: %d bytes\n", sizeRegistro);

    //char *a = "tecnologia.bin";
    //binarioNaTela(a);

    // frees allocated memory for variable size fields
    for (int i = 0; i < size; i++) {
        free(registro[i].tecnologiaOrigem.string);
        free(registro[i].tecnologiaDestino.string);
    }

    // frees allocated memory for struct Registro
    free(registro);

    return 0;
}
