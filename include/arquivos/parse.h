#include <stdio.h>
#include "listaSE.h"
#include "arquivos/tipos.h"

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
void adicionaLista (listaSE* tec, Cabecalho *c_buffer, Registro *r_buffer);

/// @brief Parse entire CSV file and write its data to BIN_out file, while
/// updating a buffer for Cabecalho `c_buffer` according to specification.
/// @param CSV_in CSV input file.
/// @param BIN_out Binary output file.
/// @param c_buffer Buffer for updated `Cabecalho`.
void parseCSV(FILE *CSV_in, FILE *BIN_out, Cabecalho *c_buffer);