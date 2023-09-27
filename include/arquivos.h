#include <stdio.h>

#define TAM_REGISTRO 76
#define TAM_REGISTRO_FIXO 21
#define TAM_MAXIMO_STRING (TAM_REGISTRO - TAM_REGISTRO_FIXO)
#define LIXO '$'
#define NAO_REMOVIDO '0'
#define REMOVIDO '1'

typedef struct {
    char status[2];
    int proxRRN;
    int nroTecnologias;
    int nroParesTecnologias;
} Cabecalho;

typedef struct {
    int tamanho;
    char *string;
} StringVariavel;

typedef struct {
    char removido[1];
    int grupo;
    int popularidade;
    int peso;
    StringVariavel tecnologiaOrigem;
    StringVariavel tecnologiaDestino;
} Registro;

/// @brief Calculates size of a register
/// @param r pointer to register
/// @return size of register data on file
int calcularTamanhoRegistro(Registro *r);

/// @brief Writes trash values to complete fixed register size.
/// @param arquivo file to write `registro`
/// @param r register to be written
void escreverLixoRegistro(FILE *arquivo, Registro *r);

/// @brief Writes `r` data to `arquivo` according to specifications
/// @param arquivo file to write `registro`
/// @param r register to be written
void escreverDadosRegistro(FILE *arquivo, Registro *r);

/// @brief Writes `registro` to `arquivo`
/// @param arquivo file to write `registro`
/// @param r register to be written
void escreverRegistro(FILE *arquivo, Registro *r);

/// @brief Parses  `caminhoCSV`
/// substituting or creating new binary file `caminhoBin`
/// @param caminhoCSV path to CSV file
/// @param caminhoBin path to new binary file according to specification
void csvParaBinario(char* caminhoCSV, char* caminhoBin);

void leitura_e_imprime(char* caminhoBin);