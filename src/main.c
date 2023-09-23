#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "arquivos.h"

int main() {
    FILE *file;

    csvParaBinario("tecnologia.csv", "tecnologia.bin");

    // // print
    // for (int i = 1; i < 5; i++) {
    //     //printf("%d:\n", i + 1);
    //     printf("%s,", registro[i].tecnologiaOrigem.string);
    //     //printf("%d,", registro[i].tecnologiaOrigem.tamanho);
    //     printf("%d,", registro[i].grupo);
    //     printf("%d,", registro[i].popularidade);
    //     printf("%d,", registro[i].peso);
    //     printf("%s", registro[i].tecnologiaDestino.string);
    //     printf("\n");
    // }

    // // calculating the size of line 1
    // int sizeRegistro = sizeof(char) +  // removed
    //                      sizeof(int) * 3 + // grupo, popularidade, peso
    //                      strlen(registro[1].tecnologiaOrigem.string) + 1 + // tecnologiaOrigem.string + '\0'
    //                      strlen(registro[1].tecnologiaDestino.string) + 1; // tecnologiaDestino.string + '\0'

    // //printf("\n\nThe size of line 1 is: %d bytes\n", sizeRegistro);

    // //char *a = "tecnologia.bin";
    // //binarioNaTela(a);

    // // frees allocated memory for variable size fields
    // for (int i = 0; i < size; i++) {
    //     free(registro[i].tecnologiaOrigem.string);
    //     free(registro[i].tecnologiaDestino.string);
    // }

    // // frees allocated memory for struct Registro
    // free(registro);

    return 0;
}
