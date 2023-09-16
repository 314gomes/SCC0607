#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char removido;
    int grupo;
    int popularidade;
    int peso;
    char *nomeTecnologiaOrigem;
    char *nomeTecnologiaDestino;
} RegistroDados;

int main() {
    FILE *arquivo;
    RegistroDados *dados = NULL;
    int size = 0;
    int cont = 0;
    char line[100]; // suppose each line of the file has a maximum of 100 characters

    arquivo = fopen("tecnologia.csv", "r");
    if (arquivo == NULL) {
        printf("Error opening the file.\n");
        return 1;
    }

    while (fgets(line, sizeof(line), arquivo)) {

        // allocates space for the new struct
        dados = (RegistroDados *)realloc(dados, (size + 1) * sizeof(RegistroDados));
        if (dados == NULL) {
            printf("Error while allocating memory.\n");
            return 1;
        }

        // initializes the struct fields
        dados[size].nomeTecnologiaOrigem = NULL;
        dados[size].nomeTecnologiaDestino = NULL;


        // allocates space for variable size fields
        dados[size].nomeTecnologiaOrigem = (char *)malloc(50); // 50 é um size arbitrário
        dados[size].nomeTecnologiaDestino = (char *)malloc(50);

        // reads values and allocates memory for variable-sized fields
        sscanf(line, "%49[^,],%d,%d,%49[^,],%d",
               dados[size].nomeTecnologiaOrigem,
               &dados[size].grupo,
               &dados[size].popularidade,
               dados[size].nomeTecnologiaDestino,
               &dados[size].peso);
        // updates the size and counter
        size++;
        cont++;

        // verifies that we do not exceed the maximum registration limit
        if (cont >= 10) {
            printf("maximum registration limit\n");
            break;
        }
    }

    fclose(arquivo);

    // print
    for (int i = 1; i < size; i++) {
        //printf("%d:\n", i + 1);
        printf("%s,", dados[i].nomeTecnologiaOrigem);
        printf("%d,", dados[i].grupo);
        printf("%d,", dados[i].popularidade);
        printf("%d,", dados[i].peso);
        printf("%s", dados[i].nomeTecnologiaDestino);
        printf("\n");
    }

    // calculating the size of line 1
    int sizeRegistro = sizeof(char) +  // removed
                         sizeof(int) * 3 + // grupo, popularidade, peso
                         strlen(dados[1].nomeTecnologiaOrigem) + 1 + // nomeTecnologiaOrigem + '\0'
                         strlen(dados[1].nomeTecnologiaDestino) + 1; // nomeTecnologiaDestino + '\0'

    printf("\n\nThe size of line 1 is: %d bytes\n", sizeRegistro);

    // frees allocated memory for variable size fields
    for (int i = 0; i < size; i++) {
        free(dados[i].nomeTecnologiaOrigem);
        free(dados[i].nomeTecnologiaDestino);
    }

    // frees allocated memory for struct RegistroDados
    free(dados);

    return 0;
}
