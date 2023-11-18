#ifndef FILENAME_INCLUDED
#define FILENAME_INCLUDED
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
#endif