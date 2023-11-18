/**
 * Grupo 8
 * João Pedro Gomes - 13839069
 * Luana Hartmann Franco da Cruz - 13676350
*/
#include <stdio.h>

#include "arquivos/defines.h"
#include "arquivos/utils.h"

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
