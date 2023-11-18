/**
 * Grupo 8
 * João Pedro Gomes - 13839069
 * Luana Hartmann Franco da Cruz - 13676350
*/
#include <stdio.h>

#include "arquivos/defines.h"
#include "arquivos/utils.h"

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

/// @brief Calculates byteoffset of a given RRN according to specification.
/// @param RRN Relative registry number.
/// @return It's byte offset.
long byteoffset_RRN(int RRN);

/// @brief Reads an integer field and writes it to i
/// @param bin Open binary FILE* to read from
/// @param i int buffer to write to
void leCampoInt(FILE* bin, int *i);

/// @brief Reads a single char from binary file and writes it to c
/// @param bin Open binary FILE* to read from
/// @param c Char buffer to write to
void leCampoChar(FILE* bin, char *c);

/// @brief Reads a StringVariavel field from binary file and writes it to s
/// @param bin Open binary FILE* to read from
/// @param s StringVariavel buffer to write to
void leCampoStringVariavel(FILE* bin, StringVariavel *s);

/// @brief Reads status from registry and writes it to r
/// @param bin Open binary FILE* to read from
/// @param r Registry buffer to write to
void leStatusRegistro(FILE *bin, Registro *r);

/// @brief Reads content (not including status) from registry and writes it to
/// r
/// @param bin Binary FILE* to read from
/// @param r registry buffer to write to
void leConteudoRegistro(FILE *bin, Registro *r);

/// @brief Reads registry at relative registry number RRN from binary FILE* bin
/// according to specification, writing it to r
/// @param bin Open binary FILE* to write to
/// @param RRN Relative registry number to acess
/// @param r Registry buffer to write to
/// @return Returns `sucesso` on sucess or `registro_inexistente` if registry has
/// been marked as deleted or is not on file
StatusDeRetorno le_RRN(FILE *bin, int RRN, Registro *r);

/// @brief Funcao auxiliar utilizada na busca de campos fixos de inteiros
/// @param caminhoBin Caminho para arquivo binario
/// @param campo Campo que deve ser buscado
/// @param buscado Valor inteiro a ser buscado
/// @return Retorna `sucesso` quando a busca e bem sucedida, `registro_inexistente` se
/// o registro esta marcado como deletado ou caso nao esteja no arquivo
StatusDeRetorno buscaCampoInt (char* caminhoBin, int campo, int buscado);

/// @brief Funcao auxiliar utilizada na busca de campos variaveis de char
/// @param caminhoBin Caminho para arquivo binario
/// @param buscado String que sera buscada
/// @param tamanho Tamanho da string a ser buscada
/// @param flag Flag para informar qual string sera buscada. Seu valor e `0` para a primeira
/// string e `1` para a segunda
/// @return Retorna `sucesso` quando a busca e bem sucedida, `registro_inexistente` se
/// o registro esta marcado como deletado ou caso nao esteja no arquivo
StatusDeRetorno buscaCampoStringVariavel (char* caminhoBin, char* buscado, int tamanho, int flag);
