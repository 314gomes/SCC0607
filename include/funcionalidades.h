/**
 * Grupo 8
 * João Pedro Gomes - 13839069
 * Luana Hartmann Franco da Cruz - 13676350
*/
#include "arquivos/tipos.h"
#include "arquivos/defines.h"

void imprimeMensagemErro(StatusDeRetorno s);

/// @brief Parses  `caminhoCSV`
/// substituting or creating new binary file `caminhoBin`
/// @param caminhoCSV path to CSV file
/// @param caminhoBin path to new binary file according to specification
/// @return `sucesso` if no errors encountered, `falha_processamento` otherwise
StatusDeRetorno funcionalidade1(char* caminhoCSV, char* caminhoBin);

/// @brief Function for feature 2. Opens binary file at 'caminhoBin'
/// and print it
/// @param caminhoBin Path to binary file to open
/// @return `sucess` upon sucessful execution, `registro_inexistente` if
/// all the registries are deleted
/// `erro_processamento` if other errors have been found.
StatusDeRetorno funcionalidade2(char* caminhoBin);

/// @brief  Main function for feature 3. Opens binary file at 'caminhoBin',
/// receives 'n' searches and prints all the registries with the searched field
/// @param caminhoBin Path to binary file to open
/// @param n Number of field to be searched
/// @return `sucess` upon sucessful execution, `registro_inexistente` if
/// no registry with the field especified is encoutered
/// `erro_processamento` if other errors have been found.
StatusDeRetorno funcionalidade3(char* bin_path, int n, char** campo, char** valor);

/// @brief Main function for feature 4. Opens binary file at `caminhoBin` and
/// prints registry at relative registry number `RRN`
/// @param caminhoBin Path to binary file to open
/// @param RRN Relative registry number to read
/// @return `sucess` upon sucessful execution, `registro_inexistente` if
/// registry at RRN has been marked as deleted or is not present on file or
/// `erro_processamento` if other errors have been found.
StatusDeRetorno funcionalidade4(char* caminhoBin, int RRN);
StatusDeRetorno funcionalidade5(char *bin_path, char *index_path);
StatusDeRetorno funcionalidade6(char *bin_path, char *index_path, int n, char** campo, char** valor);