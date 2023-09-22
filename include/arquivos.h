#define TAM_REGISTRO 76
#define TAM_REGISTRO_FIXO 21
#define LIXO = '$'
#define NAO_REMOVIDO = '0'
#define REMOVIDO = '1'
#define TAMANHO_MAXIMO 500

typedef struct {
    char status;
    int proxRRn;
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

// @brief Writes `registro` to `arquivo`
// @param arquivo file to write `registro`
// @param registro register to be written
void escreverRegistro(FILE *arquivo, Registro *registro);
void binarioNaTela(char *nomeArquivoBinario);