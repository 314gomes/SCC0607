#include "arquivos/tipos.h"

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
