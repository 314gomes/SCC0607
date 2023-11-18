#include <stdio.h>
#include "arquivos/tipos.h"

/// @brief Allocates memory and assings default values for new Registro struct.
/// @return pointer to new Registro.
Registro *novo_registro();

/// @brief Frees memory for Registro*
/// @param r registry pointer to have its memory freed
void free_registro(Registro *r);

/// @brief Allocates memory and assings default values for new Cabecalho struct.
/// @return pointer to new Cabecalho.
Cabecalho *novo_cabecalho();

/// @brief Calculates size of a register
/// @param r pointer to register
/// @return size of register data on file
int calcularTamanhoRegistro(Registro *r);

/// @brief Opens binary file and detects its status.
/// @param caminhobin path to binary file.
/// @return NULL if status is `INCONSISTENTE` or if file does not exist, FILE*
/// to binary file if sucessfull.
FILE *abreBinario(char *caminhobin);