/**
 * Grupo 8
 * João Pedro Gomes - 13839069
 * Luana Hartmann Franco da Cruz - 13676350
*/
#include <stdlib.h>
#include <string.h>
#include "funcionalidades.h"
#include "funcoesFornecidas.h"
#include "arquivos/utils.h"
#include "arquivos/impressao.h"
#include "arquivos/escrita.h"
#include "arquivos/parse.h"
#include "arquivos/leitura.h"
#include "arquivos/tipos.h"
#include "arquivos/defines.h"
#include "arquivos/busca.h"
#include "arvoreb/utils.h"
#include "arvoreb/busca.h"

/// @brief Imprime mensagem de erro correspondente ao status de retorno s
/// @param s Status a ser impresso
void imprimeMensagemErro(StatusDeRetorno s){
    switch (s)
	{
	case sucesso:
		break;
	
	case falha_processamento:
		printf("Falha no processamento do arquivo.\n");
		break;

	case registro_inexistente:
		printf("Registro inexistente.\n");
		break;

	default:
		printf("Erro desconhecido!\n");
		break;
	}
}

StatusDeRetorno funcionalidade1 (char* caminhoCSV, char* caminhoBin){

    // initializes a new c_buffer
    Cabecalho *c_buffer = novo_cabecalho();
    
    // opens binary files for reading and writing
    // checking if they were opened correctly
    FILE *CSV_in = fopen(caminhoCSV, "r");
    if (CSV_in == NULL) return falha_processamento;
    FILE *BIN_out = fopen(caminhoBin, "wb");
    if (BIN_out == NULL) return falha_processamento;
    

    escreverCabecalho(BIN_out, c_buffer);

    // reads the CSV file and writes to the binary file
    parseCSV(CSV_in, BIN_out, c_buffer);

    // Writes updated binary file header
    c_buffer->status = CONSISTENTE;
    escreverCabecalho(BIN_out, c_buffer);

    // Closes files
    fclose(CSV_in);
    fclose(BIN_out);

    binarioNaTela(caminhoBin);

    // frees memory
    free(c_buffer);

    return sucesso;
}

StatusDeRetorno funcionalidade2 (char* caminhoBin) {
    
    Registro *r = novo_registro();
    StatusDeRetorno status = registro_inexistente;

    FILE *BIN_out = abreBinario(caminhoBin, "rb");
    if(BIN_out == NULL) return falha_processamento;

    fseek(BIN_out, 0, SEEK_SET);
    fseek(BIN_out, TAM_CABECALHO, SEEK_CUR);

    // reading and printing of the binary file
    while(fread(&(r->removido), sizeof(char), 1, BIN_out) != 0) {
       
       if (r->removido == NAO_REMOVIDO) {
            status = sucesso;
            leConteudoRegistro(BIN_out, r);
            imprimeRegistro(*r);

            int tam_lixo = TAM_REGISTRO - calcularTamanhoRegistro(r);
            fseek(BIN_out, tam_lixo, SEEK_CUR);
       }
       else{
            fseek(BIN_out, TAM_REGISTRO - 1, SEEK_CUR);
       }
       
    }
   
    free_registro(r);
    fclose(BIN_out);
    return status;
}

StatusDeRetorno funcionalidade3(char* bin_path, int n, char** campo, char** valor){
    FILE *bin = abreBinario(bin_path, "rb");
    if (bin == NULL) 
        return falha_processamento;

    StatusDeRetorno s;
    for (int i = 0; i < n; i++) {
        s = buscaCampo(bin, campo[i], valor[i]);
        
        // em casos de erro
        switch (s)
        {
        case registro_inexistente:
            imprimeMensagemErro(s);
            break;
        
        case falha_processamento:
            return s;
            break;

        default:
            break;
        }
    }

    fclose(bin);

    return sucesso;
}

StatusDeRetorno funcionalidade4 (char* caminhoBin, int RRN){
    FILE* bin = abreBinario(caminhoBin, "rb");
    
    if(bin == NULL) return falha_processamento;

    Registro *r_buffer = novo_registro();
    StatusDeRetorno s = le_RRN(bin, RRN, r_buffer);

    if(s != sucesso) return s;

    imprimeRegistro(*r_buffer);
    free_registro(r_buffer);
    fclose(bin);
    return sucesso;
}

StatusDeRetorno funcionalidade5(char *bin_path, char *index_path){
    // Abrir arquivos e testar se foram abertos corretamente
    FILE* bin = abreBinario(bin_path, "rb");
    if(bin == NULL){
        return falha_processamento;
    }
    
    FILE* index = fopen(index_path, "wb");
    if(index == NULL){
        return falha_processamento;
    }

    // ler limite superior de RRN indicado pelo proximo RRN do arquivo de dados    
    int RRN_maximo;
    fread(&RRN_maximo, sizeof(int), 1, bin);

    // buffer de registro
    Registro *r = novo_registro;
    // status que muda a cada leitura do rrn
    StatusDeRetorno status_leitura;
    // Valor a ser inserido na arvore B
    ArBChaveValor cv;
    // Leitura e insercao de todos os RRNs
    for(int RRN_atual = 0; RRN_atual < RRN_maximo; RRN_atual++){
        // ler RRN atual
        status_leitura = le_RRN(bin, RRN_atual, r);
        
        // caso nao tenha sucesso, foi marcado como removido. pular laco
        if(status_leitura != sucesso){
            break;
        }

        // certificar que a chave e uma string vazia
        cv.chave[0] = '\0';
        // armazenar chave e valor a serem inseridos na arvore
        strcat(cv.chave, r->tecnologiaOrigem.string);
        strcat(cv.chave, r->tecnologiaDestino.string);
        cv.RRNArquivoDados = RRN_atual;

        // inserir na arvore
        arBInsere(index, cv);
    }

    free_registro(r);
    fclose(bin);
    fclose(index);
}

StatusDeRetorno funcionalidade6(char *bin_path, char *index_path, int n, char** campo, char** valor){
    FILE* bin = abreBinario(bin_path, "rb");
    if(bin == NULL){
        return falha_processamento;
    }
    
    FILE* index = arBAbre(index_path, "rb");
    if(index == NULL){
        return falha_processamento;
    }

    Registro *r_buffer = novo_registro();
    StatusDeRetorno s;
    for (int i = 0; i < n; i++) {
        int eBuscaArvoreB = !strcmp(campo[i], "nomeTecnologiaOrigemDestino");
        
        if(eBuscaArvoreB){
            int RRN;
            s = arBBusca(index, valor[i], &RRN);
            if(s == sucesso){
                StatusDeRetorno s = le_RRN(bin, RRN, r_buffer);
                if(s == sucesso) imprimeRegistro(*r_buffer);
            }
        }
        else{
            s = buscaCampo(bin, campo[i], valor[i]);
        }

        // em casos de erro
        switch (s)
        {
        case registro_inexistente:
            imprimeMensagemErro(s);
            break;
        
        case falha_processamento:
            return s;
            break;

        default:
            break;
        }
    }
    free_registro(r_buffer);

    fclose(bin);
    fclose(index);

    return sucesso;

}