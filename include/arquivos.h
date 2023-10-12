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


/// @brief Enum for return features' return types
typedef enum{
    /// @brief Sucess!
    sucesso = 0,
    /// @brief Falha no processamento do arquivo.
    falha_processamento = 1,
    /// @brief Registro inexistente.
    registro_inexistente = 2,
} StatusDeRetorno;

/// @brief Allocates memory and assings default values for new Registro struct.
/// @return pointer to new Registro.
Registro *novo_registro();

/// @brief Allocates memory and assings default values for new Cabecalho struct.
/// @return pointer to new Cabecalho.
Cabecalho *novo_cabecalho();

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

/// @brief Determine whether `s` is whitespace (or empty) only.
/// @param s String to test.
/// @return `1` if `s` is whitespace only, `0` if `s` is nonempty
int isempty(const char *s);

/// @brief Parse integer type token `tkn` and write it to `n`
/// @param tkn token string to parse
/// @param n pointer to integer to be written
void parseTokenInt(char* tkn, int *n);

/// @brief Parse StringVariavel type token `tkn` and write it to `str`.
/// @param tkn Token string to parse.
/// @param n Pointer to StringVariavel to be written.
void parseTokenStringVariavel(char* tkn, StringVariavel *str);

/// @brief Parse line from CSV file and write it to buffer according to
/// specifications.
/// @param CSV_line String containing csv line according to specification.
/// May contain NULL field.
/// @param r_buffer Registry buffer.
void parseLinhaCSV(char *CSV_line, Registro *r_buffer);

/// @brief Parse entire CSV file and write its data to BIN_out file, while
/// updating a buffer for Cabecalho `c_buffer` according to specification.
/// @param CSV_in CSV input file.
/// @param BIN_out Binary output file.
/// @param c_buffer Buffer for updated `Cabecalho`.
void parseCSV(FILE *CSV_in, FILE *BIN_out, Cabecalho *c_buffer);

/// @brief Parses  `caminhoCSV`
/// substituting or creating new binary file `caminhoBin`
/// @param caminhoCSV path to CSV file
/// @param caminhoBin path to new binary file according to specification
StatusDeRetorno csvParaBinario(char* caminhoCSV, char* caminhoBin);

void leitura_e_imprime(char* caminhoBin);
void funcionalidade3 (char* caminhoBin, int n);