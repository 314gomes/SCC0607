#include <stdio.h>
#include <string.h>
#include "arquivos/leitura.h"
#include "arquivos/defines.h"
#include "arquivos/tipos.h"
#include "arquivos/utils.h"

/// @brief Le um campo inteiro e o escreve para i
/// @param bin FILE* aberto do qual ler
/// @param i buffer do tipo int no qual escrever
void leCampoInt(FILE* bin, int *i){
    fread(i, sizeof(int), 1, bin);
}

/// @brief Ler um unico char de um arquivo binario e escreve-lo em c
/// @param bin FILE* aberto do qual ler
/// @param c Buffer de char no qual escrever
void leCampoChar(FILE* bin, char *c){
    fread(c, sizeof(char), 1, bin);
}

/// @brief Le campo StringVariavel de um arquivo binario e o escreve em s
/// @param bin FILE* aberto do qual ler
/// @param s Buffer StringVariavel no qual escrever
void leCampoStringVariavel(FILE* bin, StringVariavel *s){
    // Campo de string variavel comeca com inteiro de 4 bytes do tamanho
    leCampoInt(bin, &s->tamanho);
    // ler s->tamanho caracteres se string tem tamanho maior que zero
    if(s->tamanho > 0){
        fread(s->string, sizeof(char), s->tamanho, bin);
    }
    // escrever caractere de finalizacao de string no final
    // em caso de tamanho 0, o caractere '\0' sera o primeiro da string
    s->string[s->tamanho] = '\0';
}

/// @brief Le status do registro e escreve no campo correto de r
/// @param bin FILE* aberto do qual ler
/// @param r Buffer de registro no qual escrever
void leStatusRegistro(FILE *bin, Registro *r){
    leCampoChar(bin, &r->removido);
}

/// @brief Le conteudo (nao incluindo status) de um registro e escreve em r
/// @param bin FILE* aberto do qual ler
/// @param r Buffer de registro no qual escrever
void leConteudoRegistro(FILE *bin, Registro *r){  
    leCampoInt(bin, &r->grupo);  
    leCampoInt(bin, &r->popularidade);
    leCampoInt(bin, &r->peso);
    leCampoStringVariavel(bin, &r->tecnologiaOrigem);
    leCampoStringVariavel(bin, &r->tecnologiaDestino);
}

void leCabecalho (FILE *bin, Cabecalho *c) {
    leCampoChar(bin, &c->status);
    leCampoInt(bin, &c->proxRRN);  
    leCampoInt(bin, &c->nroTecnologias);  
    leCampoInt(bin, &c->nroParesTecnologias);  
}
/// @brief Le registro em RRN especifico do arquivo bin e escreve no buffer de
/// registro r
/// @param bin FILE* aberto do qual ler
/// @param RRN RRN o qual acessar
/// @param r Buffer de registro no qual escrever
/// @return Retorna `sucesso` quando nao houverem falhas ou
/// `registro_inexistente` se registro foi marcado como logicamente removido ou
/// nao esta presente no arquivo
StatusDeRetorno le_RRN(FILE *bin, int RRN, Registro *r){
    // calcular byteoffset a partir do RRN 
    long off = byteoffset_RRN(RRN);
    // fseek para a posicao correta do rrn
    fseek(bin, off, SEEK_SET);
    
    // le o status
    leStatusRegistro(bin, r);
    
    // Essa ultima leitura pode setar a flag de fim de arquivo, nesse caso e
    // desejavel retornar que o registro nao esta no arquivo
    if(feof(bin)){
        return registro_inexistente;
    }
    // Sair com valor de retorno `registro_inexistente` caso o registro tenha
    // sido marcado como logicamente removido
    if(r->removido == REMOVIDO){
        return registro_inexistente;
    }
    // Se nenhum erro encontrado...

    // Le conteudo restante
    leConteudoRegistro(bin, r);

    // Retornar com valor de saida sucesso
    return sucesso;
}