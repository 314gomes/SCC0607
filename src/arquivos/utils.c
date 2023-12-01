#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "arquivos/utils.h"
#include "arquivos/tipos.h"
#include "arquivos/defines.h"

/// @brief Alocar memoria e atribuir valores padrao a um buffer Registro.
/// @return Ponteiro para novo Registro
Registro *novo_registro() {

    Registro* r_buffer;
    r_buffer = (Registro*) malloc(sizeof(Registro));

    r_buffer->removido = '0';
    r_buffer->tecnologiaOrigem.string = NULL;
    r_buffer->tecnologiaOrigem.tamanho = 0;
    r_buffer->tecnologiaDestino.string = NULL;
    r_buffer->tecnologiaDestino.tamanho = 0;
    r_buffer->grupo = -1;
    r_buffer->popularidade = -1;
    r_buffer->peso = -1;

    r_buffer->tecnologiaOrigem.string = (char *) malloc(TAM_MAXIMO_STRING);
    r_buffer->tecnologiaDestino.string = (char *) malloc(TAM_MAXIMO_STRING);

    return r_buffer;
}

/// @brief Libera memoria de Registro *
/// @param r Ponteiro de registro a ter sua memoria liberada
void free_registro(Registro *r){
    free(r->tecnologiaOrigem.string);
    free(r->tecnologiaDestino.string);
    free(r);
}

/// @brief Alocar memoria e atribuir valores padrao a um buffer Cabecalho.
/// @return Ponteiro para novo cabecalho Cabecalho.
Cabecalho *novo_cabecalho() {

    Cabecalho* cabecalho;
    cabecalho = (Cabecalho*)malloc(sizeof(Cabecalho));

    cabecalho->status = INCONSISTENTE;
    cabecalho->proxRRN = 0;
    cabecalho->nroTecnologias = 0;
    cabecalho->nroParesTecnologias = 0;

    return cabecalho;
}

/// @brief Calcula tamanho de Registro.
/// @param r Ponteiro para um Registro.
/// @return Tamanho dos dados do registro quando escritos no arquivo.
int calcularTamanhoRegistro(Registro *r){
    int tamanhoRegistro = TAM_REGISTRO_FIXO;
    tamanhoRegistro += r->tecnologiaOrigem.tamanho;
    tamanhoRegistro += r->tecnologiaDestino.tamanho;
    return tamanhoRegistro;
}

/// @brief Abre arquivo binario e detecta seu status.
/// @param caminhobin Caminho para arquivo binario.
/// @param modo String de modo de abertura de acordo com a especificacao fopen.
/// @return NULL se status for `INCONSISTENTE` ou se arquivo nao existe, FILE*
/// para arquivo binario se teve sucesso.
FILE *abreBinario(char *caminhoBin, char *modo){
    // variavel temporaria para guardar status do arquivo
    char status;
    
    FILE *bin = fopen(caminhoBin, modo);
    // se houve falha na abertura, retornar
    if(bin == NULL){
        return NULL;
    }

    // le status do arquivo
    fread(&status, 1, 1, bin);
    
    // Se status nao for consistente, fechar arquivo e retornar NULL
    if(status != CONSISTENTE){
        fclose(bin);
        return NULL;
    }
    
    // Caso nenhum erro tenha sido encontrado, retornar FILE* do arquivo aberto
    return bin;
}

/// @brief Calcula byte offset de um dado RRN de acordo com a especificacao.
/// @param RRN Numero relativo de registro.
/// @return Seu byte offset.
long byteoffset_RRN(int RRN){
    return TAM_CABECALHO + TAM_REGISTRO*RRN;
}

/// @brief Determinar se um campo buscado e ou nao string
/// @param nomeCampo nome do campo a determinar
/// @return 1 se for string, 0 caso contrario
int campoDeBuscaEString(char* nomeCampo){
    if (strcmp(nomeCampo, "grupo") == 0) {
        return 0;
    }
    else if (strcmp(nomeCampo, "popularidade") == 0) {
        return 0;
    }
    else if (strcmp(nomeCampo, "peso") == 0) {
        return 0;
    } 
    else if (strcmp(nomeCampo, "nomeTecnologiaOrigem") == 0) {
        return 1;
    } 
    else if (strcmp(nomeCampo, "nomeTecnologiaDestino") == 0) {
        return 1;
    }
    return -1;
}