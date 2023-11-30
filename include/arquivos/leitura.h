#include <stdio.h>
#include "arquivos/tipos.h"

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

void leCabecalho (FILE *bin, Cabecalho *c);

/// @brief Reads registry at relative registry number RRN from binary FILE* bin
/// according to specification, writing it to r
/// @param bin Open binary FILE* to write to
/// @param RRN Relative registry number to acess
/// @param r Registry buffer to write to
/// @return Returns `sucesso` on sucess or `registro_inexistente` if registry has
/// been marked as deleted or is not on file
StatusDeRetorno le_RRN(FILE *bin, int RRN, Registro *r);