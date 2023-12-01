#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include "arquivos/parse.h"
#include "arquivos/tipos.h"
#include "arquivos/utils.h"
#include "arquivos/escrita.h"
#include "listaSE.h"

/// @brief Determinar se string `s` e apenas espaco em branco (ou vazia).
/// @param s String para testar.
/// @return `1` se `s` e apenas espaco em branco ou vazia, `0` caso contrario
int isempty(const char *s)
{
    // Se string contem apenas finalizador
	if ((*s) == '\0'){
        return 1;
    }

    // Iterar pela string
	while (*s) {
        // Retornar falso no primeiro caractere nao nulo encontrado
        if (!isspace(*s))
            return 0;
        s++;
	}

    // caso contrario retorna 1
	return 1;
}

/// @brief Extrai inteiro do token `tkn` e a escreve em `n`
/// @param tkn token string a ser analisado
/// @param n buffer de string no qual escrever
void parseTokenInt(char* tkn, int *n){
    // Especificacao diz que campos string nulos devem receber -1
    if (isempty(tkn)){
        *n = -1;
    } else {
        // converte string para inteiro
        *n = atoi(tkn);
    }
}

/// @brief Extrai tipo StringVariavel de token `tkn` e escreve para `str`.
/// @param tkn Token string a ser analisada.
/// @param n Buffer de string variavel no qual escrever.
void parseTokenStringVariavel(char* tkn, StringVariavel *str){
    // Strings vazias permanecem vazias e tem tamanho 0
    if (isempty(tkn)){
        *(str->string) = '\0';
        str->tamanho = 0;
    } else {
        // copiar string e determinar tamanho
        strcpy(str->string, tkn);
        str->tamanho = strlen(tkn);
    }
}

/// @brief Extrai dados de linha de arquivo CSV e as escreve para um buffer de
/// acordo com a especificacao
/// @param CSV_line String contendo linha de arquivo CSV de acoro com a
/// espcificacao
/// @param r_buffer Buffer de registro.
void parseLinhaCSV(char *CSV_line, Registro *r_buffer){
    int field = 0;
    char *tkn = NULL;

    // Iterar sobre cada token da linha CSV, cada um separado por ","
    while ((tkn = strsep(&CSV_line, ",")) != NULL) {
        switch (field) {
            case 0:
                parseTokenStringVariavel(tkn, &(r_buffer->tecnologiaOrigem));
                break;
            case 1:
                parseTokenInt(tkn, &(r_buffer->grupo));
                break;
            case 2:
                parseTokenInt(tkn, &(r_buffer->popularidade));
                break;
            case 3:
                parseTokenStringVariavel(tkn, &(r_buffer->tecnologiaDestino));
                break;
            case 4:
                parseTokenInt(tkn, &(r_buffer->peso));
                break;
            default:
                break;
        }
        field++;
    }
}

/// @brief Extrair dados de um arquivo CSV inteiro e escreve-los em BIN_out
/// @param CSV_in Arquivo de entrada CSV
/// @param BIN_out Arquivo binario de saida de acordo com especificacao
/// @param c_buffer Buffer para o cabecalho atualizado
void parseCSV(FILE *CSV_in, FILE *BIN_out, Cabecalho *c_buffer){
    // buffer da linha lida
    char CSV_line_buffer[100];
    // buffer de registro contendo dados extraidos de cada linha
    Registro *r_buffer = novo_registro();
    // lista encadeada ordenada para contagem de tecnologias distintas
    listaSE tec = novaLista();

    // Pula primeira linha (que contem apenas nomes dos campos)
    fgets(CSV_line_buffer, sizeof(CSV_line_buffer), CSV_in);

    // Iterar sobre cada linha do arquivo CSV
    while (fgets(CSV_line_buffer, sizeof(CSV_line_buffer), CSV_in)) {
        c_buffer->proxRRN++;

        // extrair da linha
        parseLinhaCSV(CSV_line_buffer, r_buffer);

        // Algumas variaveis temporarias para legibilidade do codigo
        char *strOrigem = r_buffer->tecnologiaOrigem.string;
        char *strDestino = r_buffer->tecnologiaDestino.string;

        int origemIsNull = isempty(strOrigem);
        int destinoIsNull = isempty(strDestino);

        // Determinar se par existe 
        int parExists = !(origemIsNull) && !(destinoIsNull);

        // Adicionar tecnologias nao nulas na lista de tecnologias
        if(!origemIsNull)
            insereOrdenadoSemRepeticao(strOrigem, &tec);
        if(!destinoIsNull)
            insereOrdenadoSemRepeticao(strDestino, &tec);

        // Incrementar numero de pares de tecnologia de acordo
        if(parExists)
            c_buffer->nroParesTecnologias++;

        // Escrever buffer para arquivo
        escreverRegistro(BIN_out, r_buffer);
    }

    // Numero de tecnologias diferentes e apenas o tamanho da lista
    c_buffer->nroTecnologias = tec.tam;

    // Liberar memoria da lista encadeada
    destroiLista(&tec);

    // liberar memoria do buffer de registro
    free_registro(r_buffer);
}
