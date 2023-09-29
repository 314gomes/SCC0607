#include <stdio.h>

#include <defines.h>

typedef struct {
    char status;
    int proxRRN;
    int nroTecnologias;
    int nroParesTecnologias;
} Cabecalho;

typedef struct {
    int tamanho;
    char *string;
} StringVariavel;

typedef struct {
    char removido;
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

/// @brief Write a field of one char to file `a`
/// @param a file to write to
/// @param c field
void escreverCampoChar(FILE *a, char c);

/// @brief Write a field of one int to file `a`
/// @param a file to write to
/// @param c field
void escreverCampoInt(FILE *a, int c);


/// @brief Write a field of StringVariavel type to file `a`
/// @param a file to write to
/// @param c field
void escreverCampoStringVariavel(FILE *a, StringVariavel c);

/// @brief Writes `r` data to `arquivo` according to specifications
/// @param arquivo file to write `registro`
/// @param r register to be written
void escreverDadosRegistro(FILE *arquivo, Registro *r);

/// @brief Writes `registro` to `arquivo`
/// @param arquivo file to write `registro`
/// @param r register to be written
void escreverRegistro(FILE *arquivo, Registro *r);

/// @brief Writes `cabecalho` to `arquivo`
/// @param arquivo file to write `registro`
/// @param h register to be written
void escreverCabecalho(FILE *arquivo, Cabecalho *h);

/// @brief Parses  `caminhoCSV`
/// substituting or creating new binary file `caminhoBin`
/// @param caminhoCSV path to CSV file
/// @param caminhoBin path to new binary file according to specification
void csvParaBinario(char* caminhoCSV, char* caminhoBin);

void leitura_e_imprime(char* caminhoBin);