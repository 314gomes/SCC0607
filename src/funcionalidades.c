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
#include "arvoreb/escrita.h"
#include "arvoreb/insercao.h"
//#include "listaSE.h"

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
/// @brief Extrai dados de arquivo CSV em `caminhoCSV` substituindo ou criando
/// novo arquivo binario em `caminhoBin` de acordo com especificacao
/// @param caminhoCSV Caminho para arquivo CSV
/// @param caminhoBin Caminho para arquivo binario
/// @return `sucesso` se nenhum erro foi encontrado, `falha_processamento` caso
/// contrario
StatusDeRetorno funcionalidade1 (char* caminhoCSV, char* caminhoBin){

    // Inicializar novo buffer de cabecalho
    Cabecalho *c_buffer = novo_cabecalho();
    
    // Abrir arquivo CSV em modo de leitura de texto
    FILE *CSV_in = fopen(caminhoCSV, "r");
    // Se nao foi aberto corretaomente, retornar com erro
    if (CSV_in == NULL) 
        return falha_processamento;
    // Abrir arquivo binario em modo de escrita binaria
    FILE *BIN_out = fopen(caminhoBin, "wb");
    // Retornar com erro caso nao tenha sido aberto corretamente
    if (BIN_out == NULL) 
        return falha_processamento;
    
    // Escrever cabecalho inicial (inicialmente com status inconsistente)
    escreverCabecalho(BIN_out, c_buffer);

    // Sobrescrever binario com dados do CSV
    parseCSV(CSV_in, BIN_out, c_buffer);

    // Sobrescrever cabecalho atualizado
    c_buffer->status = CONSISTENTE;
    escreverCabecalho(BIN_out, c_buffer);

    // fechar arquivos
    fclose(CSV_in);
    fclose(BIN_out);

    // utilizar funcao fornecida para teste do [run.codes]
    binarioNaTela(caminhoBin);

    // frees memory
    free(c_buffer);

    return sucesso;
}

/// @brief Abrir caminhoBin e exibir todos os seus dados de acordo com
/// especificacao.
/// @param caminhoBin Caminho de arquivo binario a ser exibido.
/// @return `sucess` no caso de execucao correta, `registro_inexistente` se
/// os registros todos foram marcados como removidos, `erro_processamento` se
/// outros erros foram encontrados.
StatusDeRetorno funcionalidade2 (char* caminhoBin) {
    // buffer de registro para leitura sequencial do arquivo
    Registro *r = novo_registro();
    // status de retorno inicialmente marcado como registro_inexistente
    StatusDeRetorno status = registro_inexistente;
    // abrir binario em modo de leitura binaria
    FILE *BIN_out = abreBinario(caminhoBin, "rb");
    // caso a abertura retorne erros, encerrar
    if(BIN_out == NULL) return falha_processamento;

    // pular o cabecalho na leitura    
    fseek(BIN_out, TAM_CABECALHO, SEEK_SET);

    // a leitura do campo removido do registro nao retornar 0 (indicano fim do
    // arquivo)
    while(fread(&(r->removido), sizeof(char), 1, BIN_out) != 0) {
       // se nao foi removido
       if (r->removido == NAO_REMOVIDO) {
            // marcar que houve um registro nao removido no arquivo
            status = sucesso;
            // le conteudo do registro
            leConteudoRegistro(BIN_out, r);
            // imprime de acordo com especificacao
            imprimeRegistro(*r);

            // calcular tamanho do lixo do registro            
            int tam_lixo = TAM_REGISTRO - calcularTamanhoRegistro(r);
            // pular para proximo registro
            fseek(BIN_out, tam_lixo, SEEK_CUR);
       }
       // caso tenha sido marcado como removido
       else{
            // pular para proximo registro
            fseek(BIN_out, TAM_REGISTRO - 1, SEEK_CUR);
       }
       
    }
    
    // limpar memoria do regitro
    free_registro(r);
    // fechar arquivo
    fclose(BIN_out);
    // imprimir status
    return status;
}

/// @brief  Funcao principal da funcionalidade 3. Abre arquivo binario
/// 'caminhoBin', recebe 'n' buscas e imprime registros que satisfazem a busca.
/// @param caminhoBin Caminho do arquivo binario para abrir
/// @param n Numero de buscas a serem feitas.
/// @param campo lista de strings com nomes dos campos de busca
/// @param valor lista de strings com valores de busca
/// @return `sucess` quando resultar em execucao correta, `erro_processamento`
/// se erros foram encontrados.
StatusDeRetorno funcionalidade3(char* bin_path, int n, char** campo, char** valor){
    // abrir arquivo binario
    FILE *bin = abreBinario(bin_path, "rb");
    // encerrar com erro caso abertura tenha sido incorreta
    if (bin == NULL) 
        return falha_processamento;

    // variavel local para status de retorno de cada busca
    StatusDeRetorno s;
    // para cada busca...
    for (int i = 0; i < n; i++) {
        // atualizar status com resultado da busca correspondente
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

    // fechar arquivo
    fclose(bin);

    // nenhum erro encontrado, retornar sucesso
    return sucesso;
}

/// @brief Funcao principal da funcionalidade 4. Abre arquivo vinario
/// `caminhoBin` e imprime registro em RRN especificado
/// @param caminhoBin Caminho para arquivo binario a ser aberto
/// @param RRN Numero relativo de registro a ler
/// @return `sucess` quando resultar em execucao correta, `registro_inexistente`
/// se registro no RRN foi marcado como removido ou nao esta presente no
/// arquivo, ou `erro_processamento` se outro erro foi encontrado.
StatusDeRetorno funcionalidade4 (char* caminhoBin, int RRN){
    // abrir arquivo binario em modo de leitura binaria
    FILE* bin = abreBinario(caminhoBin, "rb");
    // se houveram erros na abertura, encerrar com erro
    if(bin == NULL)
        return falha_processamento;

    // alocar memoria para novo buffer de registro a ser lido
    Registro *r_buffer = novo_registro();
    // ler registro no rrn e salvar em variavel de status de retorno da leitura
    StatusDeRetorno s = le_RRN(bin, RRN, r_buffer);

    // se teve sucesso imprime o registro
    if(s == sucesso){
        imprimeRegistro(*r_buffer);
    }

    // limpar memoria do buffer
    free_registro(r_buffer);
    // fechar arquivo
    fclose(bin);
    // retornar status da leitura
    return s;
}

/// @brief Funcao principal da funcionalidade 5. Ler dados de um arquivo binario
/// e inserir suas chaves no arquivo de indices arvore b.
/// @param bin_path Caminho para arquivo de dados binario.
/// @param index_path Camiho para arquivo de indices arvore b.
/// @return `sucesso` em caso de execucao correta, `falha_processamento` caso
/// algum erro tenha sido encontrado
StatusDeRetorno funcionalidade5(char *bin_path, char *index_path){
    // Abrir arquivos e testar se foram abertos corretamente
    FILE* bin = abreBinario(bin_path, "rb");
    if(bin == NULL){
        return falha_processamento;
    }
    
    FILE* index = fopen(index_path, "w+b");
    if(index == NULL){
        return falha_processamento;
    }

    // buffer de cabecalho da arvore
    ArBCabecalho arbc;
    // escrever cabecalho inicial (que indica que a arvore esta vazia e
    // inconsistente)
    arbc.status = '0';
    arbc.noRaiz = -1;
    arbc.RRNProxNo = 0;
    arBEscreveCabecalho(index, &arbc);

    // ler limite superior de RRN indicado pelo proximo RRN do arquivo de dados    
    int RRN_maximo;
    fread(&RRN_maximo, sizeof(int), 1, bin);

    // buffer de registro
    Registro *r = novo_registro();
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
            continue;
        }

        // caso nao tenha origem ou destino, pular laco
        if(r->tecnologiaOrigem.tamanho == 0){
            continue;
        }
        if(r->tecnologiaDestino.tamanho == 0){
            continue;
        }
        // certificar que a chave e uma string vazia
        cv.chave[0] = '\0';
        // armazenar chave e valor a serem inseridos na arvore
        strcat(cv.chave, r->tecnologiaOrigem.string);
        strcat(cv.chave, r->tecnologiaDestino.string);
        cv.RRNArquivoDados = RRN_atual;

        // inserir na arvore
        arBInsere(index, &cv);
    }

    // atualizar valor do status para condizente
    arbc.status = '1';
    // posicionar stream para atualizar o status
    fseek(index, 0, SEEK_SET);
    // sobrescrever status
    fwrite(&arbc.status, sizeof(char), 1, index);

    // limpar memoria do registro
    free_registro(r);
    // fechar arquivos
    fclose(bin);
    fclose(index);

    // utilizar funcao fornecida para analise no [run.codes]
    binarioNaTela(index_path);

    // retornar sucesso
    return sucesso;
}

/// @brief Funcao principal da funcionalidade 6. Realiza n buscas podendo
/// algumas delas serem realizadas usando busca em arvore b.
/// @param bin_path Caminho para o arquivo de dados binario
/// @param index_path Caminho para o arquivo de indices
/// @param n Numero de buscas a serem realizadas
/// @param campo lista de strings contendo os nomes dos campos
/// @param valor lista de strings contendo os valores de busca
/// @return `sucesso` em caso de execucao correta ou `falha_processamento` caso
/// algum erro tenha sido encontrado.
StatusDeRetorno funcionalidade6(char *bin_path, char *index_path, int n, char** campo, char** valor){
    // Abrira arquivos e testar se foram abertos corretamente
    FILE* bin = abreBinario(bin_path, "rb");
    if(bin == NULL){
        return falha_processamento;
    }
    
    FILE* index = arBAbre(index_path, "rb");
    if(index == NULL){
        return falha_processamento;
    }

    // buffer de registro usado no caso de busca em arvore b
    Registro *r_buffer = novo_registro();
    // status de retorno da busca realizada
    StatusDeRetorno s;
    // rodar uma vez a cada busca
    for (int i = 0; i < n; i++) {
        // determinar se a busca deve ser feita usando a arvore b
        int eBuscaArvoreB = !strcmp(campo[i], "nomeTecnologiaOrigemDestino");
        
        // se deve ser feita na arvore b
        if(eBuscaArvoreB){
            // variavel para guardar o RRN
            int RRN;
            // buscar chave na arvore b
            s = arBBusca(index, valor[i], &RRN);
            // se encontrada
            if(s == sucesso){
                // ler RRN retornado para o buffer de registro
                StatusDeRetorno s = le_RRN(bin, RRN, r_buffer);
                // imprimir registro lido
                if(s == sucesso) imprimeRegistro(*r_buffer);
            }
        }
        // se nao deve ser feita em arvore b
        else{
            // buscar utilizando busca sequencial como no trabalho introdutorio
            s = buscaCampo(bin, campo[i], valor[i]);
        }

        // em casos de erro na busca
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
    // liberar memoria do registro    
    free_registro(r_buffer);

    // fechar arquivos
    fclose(bin);
    fclose(index);

    // retornar com sucesso
    return sucesso;
}

/// @brief Funcao principal para a funcionalidade 7. Abre os arquivos binarios em 'bin_path' e
/// 'index_path' e insere novos registros tanto no arquivo de registros quanto no arquivo de indices da 
/// arvore B.
/// @param bin_path Caminho para o arquivo binario de registros.
/// @param index_path Caminho para o arquivo binario de indices.
/// @param n Numero de registros que serao inseridos.
/// @param linhas Armazena em cada linha um dos registros novos que serao inseridos.
/// @return Retorna 'sucesso' quando a insercao foi executada sem erros, `erro_processamento` se algum outro erro
/// foi encontrado.
StatusDeRetorno funcionalidade7 (char *bin_path, char *index_path, int n, char** linhas) {
    FILE* bin = abreBinario(bin_path, "r+");
    if(bin == NULL){
        return falha_processamento;
    }
    
    FILE* index = arBAbre(index_path, "r+");
    if(index == NULL){
        return falha_processamento;
    }

    Registro *r_buffer = novo_registro(); 
    Cabecalho *c_buffer = novo_cabecalho();  
    listaSE tec = novaLista(); 

    // Valor a ser inserido na arvore B
    ArBChaveValor cv;

    // fseek necessario ja que a funcao de abertura le o status do cabecalho
    fseek(bin, 0, SEEK_SET);

    leCabecalho (bin, c_buffer);
    c_buffer->status = INCONSISTENTE;
    escreverCabecalho(bin, c_buffer);

    //printf("proxRRN = %d\n\n", c_buffer->proxRRN);


    // percorrer arquivo de dados para armazenar todas as tecnologias ja contidas nele
    for(int RRN_atual = 0; RRN_atual < c_buffer->proxRRN; RRN_atual++){
        // ler RRN atual
        StatusDeRetorno status_leitura = le_RRN(bin, RRN_atual, r_buffer);
        
        // caso nao tenha sucesso, foi marcado como removido. pular laco
        if(status_leitura != sucesso){
            continue;
        }

        // caso tenha tecnologia, inserir na lista
        if(r_buffer->tecnologiaOrigem.tamanho != 0){
            insereOrdenadoSemRepeticao(r_buffer->tecnologiaOrigem.string, &tec);
        }
        if(r_buffer->tecnologiaDestino.tamanho != 0){
            insereOrdenadoSemRepeticao(r_buffer->tecnologiaDestino.string, &tec);
        }
    }

    fseek(bin, 0, SEEK_END);

    r_buffer->removido = NAO_REMOVIDO;
    for (int i = 0; i < n; i++) {

        parseLinhaCSV(linhas[i], r_buffer);

        escreverRegistro(bin, r_buffer);
        c_buffer->proxRRN++;


        // caso tenha tecnologia, inserir na lista
        if(r_buffer->tecnologiaOrigem.tamanho != 0){
            insereOrdenadoSemRepeticao(r_buffer->tecnologiaOrigem.string, &tec);
        }
        if(r_buffer->tecnologiaDestino.tamanho != 0){
            insereOrdenadoSemRepeticao(r_buffer->tecnologiaDestino.string, &tec);
        }

        // caso nao tenha origem ou destino, pular laco
        if(r_buffer->tecnologiaOrigem.tamanho == 0){
            continue;
        }
        if(r_buffer->tecnologiaDestino.tamanho == 0){
            continue;
        }

        // nem origem nem destino e nulo, incrementar numero de pares
        c_buffer->nroParesTecnologias++;

        // certificar que a chave e uma string vazia
        cv.chave[0] = '\0';
        // armazenar chave e valor a serem inseridos na arvore
        strcat(cv.chave, r_buffer->tecnologiaOrigem.string);
        strcat(cv.chave, r_buffer->tecnologiaDestino.string);
        cv.RRNArquivoDados = c_buffer->proxRRN - 1;

        // inserir na arvore
        arBInsere(index, &cv);
    }

    c_buffer->status = CONSISTENTE;
    // atualizar nro de tecnologias com o tamanho da lista de tecnologias sem
    // repeticao
    c_buffer->nroTecnologias = tec.tam;
    escreverCabecalho(bin, c_buffer);

    fclose(bin);
    fclose(index);

    binarioNaTela(bin_path);
    binarioNaTela(index_path);

    // frees memory
    free_registro(r_buffer);
    free(c_buffer);
    destroiLista(&tec);

    return sucesso;
}