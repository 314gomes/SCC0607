#include "arquivos/tipos.h"

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