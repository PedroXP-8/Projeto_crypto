#include <errno.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#include "../arquivos_h/arquivos.h"
#include "../arquivos_h/entrada.h"

int garantir_arquivo(const char *caminho, const char *conteudo_inicial)
{
    FILE *arquivo = fopen(caminho, "r");

    if (arquivo != NULL) {
        fclose(arquivo);
        return 1;
    }

    if (errno != ENOENT) {
        printf("Nao foi possivel abrir %s.\n", caminho);
        return 0;
    }

    arquivo = fopen(caminho, "w");
    if (arquivo == NULL) {
        printf("Nao foi possivel criar %s.\n", caminho);
        return 0;
    }

    if (fputs(conteudo_inicial, arquivo) == EOF) {
        fclose(arquivo);
        printf("Nao foi possivel gravar %s.\n", caminho);
        return 0;
    }
    if (fclose(arquivo) != 0) {
        printf("Nao foi possivel gravar %s.\n", caminho);
        return 0;
    }

    return 1;
}

int criar_arquivos_iniciais(void)
{
    const char *investidor_inicial =
        "45971742917|123456|Pedro Henrique|10000.00|0.01000000|0.25000000|100.00000000\n";

    return garantir_arquivo(ARQUIVO_INVESTIDOR, investidor_inicial) &&
           garantir_arquivo(ARQUIVO_COTACOES,
                            "BTC|350000.00\nETH|18000.00\nXRP|3.00\n") &&
           garantir_arquivo(ARQUIVO_EXTRATO, "");
}

int carregar_investidor(Investidor *investidor)
{
    FILE *arquivo = fopen(ARQUIVO_INVESTIDOR, "r");
    int campos;

    if (arquivo == NULL) {
        printf("Erro ao abrir %s.\n", ARQUIVO_INVESTIDOR);
        return 0;
    }

    campos = fscanf(arquivo,
                    " %11[^|]|%6[^|]|%79[^|]|%lf|%lf|%lf|%lf",
                    investidor->cpf,
                    investidor->senha,
                    investidor->nome,
                    &investidor->saldo_reais,
                    &investidor->saldo_moedas[0],
                    &investidor->saldo_moedas[1],
                    &investidor->saldo_moedas[2]);
    fclose(arquivo);

    if (campos != 7 || !cpf_valido(investidor->cpf) ||
        !senha_valida(investidor->senha) ||
        !isfinite(investidor->saldo_reais) || investidor->saldo_reais < 0.0) {
        printf("O arquivo do investidor esta incompleto ou invalido.\n");
        return 0;
    }

    for (int i = 0; i < QUANTIDADE_MOEDAS; i++) {
        if (!isfinite(investidor->saldo_moedas[i]) ||
            investidor->saldo_moedas[i] < 0.0) {
            printf("O saldo de criptomoedas no arquivo e invalido.\n");
            return 0;
        }
    }

    return 1;
}

int carregar_cotacoes(void)
{
    FILE *arquivo = fopen(ARQUIVO_COTACOES, "r");
    char simbolo[4];

    if (arquivo == NULL) {
        printf("Erro ao abrir %s.\n", ARQUIVO_COTACOES);
        return 0;
    }

    for (int i = 0; i < QUANTIDADE_MOEDAS; i++) {
        if (fscanf(arquivo, " %3[^|]|%lf", simbolo, &moedas[i].cotacao) != 2 ||
            strcmp(simbolo, moedas[i].simbolo) != 0 ||
            !isfinite(moedas[i].cotacao) || moedas[i].cotacao <= 0.0) {
            printf("O arquivo de cotacoes esta incompleto ou invalido.\n");
            fclose(arquivo);
            return 0;
        }
    }

    fclose(arquivo);
    return 1;
}

int salvar_investidor(const Investidor *investidor)
{
    FILE *arquivo = fopen(ARQUIVO_INVESTIDOR, "w");
    int sucesso;

    if (arquivo == NULL) {
        return 0;
    }

    sucesso = fprintf(arquivo, "%s|%s|%s|%.8f|%.8f|%.8f|%.8f\n",
                      investidor->cpf, investidor->senha, investidor->nome,
                      investidor->saldo_reais, investidor->saldo_moedas[0],
                      investidor->saldo_moedas[1], investidor->saldo_moedas[2]) >= 0;
    if (fclose(arquivo) != 0) {
        sucesso = 0;
    }

    return sucesso;
}

int salvar_cotacoes(void)
{
    FILE *arquivo = fopen(ARQUIVO_COTACOES, "w");
    int sucesso = 1;

    if (arquivo == NULL) {
        return 0;
    }

    for (int i = 0; i < QUANTIDADE_MOEDAS; i++) {
        if (fprintf(arquivo, "%s|%.8f\n", moedas[i].simbolo,
                    moedas[i].cotacao) < 0) {
            sucesso = 0;
            break;
        }
    }

    if (fclose(arquivo) != 0) {
        sucesso = 0;
    }

    return sucesso;
}

int registrar_movimentacao(const Investidor *investidor,
                                 const char *tipo,
                                 const char *moeda,
                                 double valor,
                                 double cotacao,
                                 double taxa)
{
    FILE *arquivo = fopen(ARQUIVO_EXTRATO, "a");
    time_t agora;
    struct tm horario;
    struct tm *horario_local;
    char data_hora[20];
    int sucesso;

    if (arquivo == NULL) {
        return 0;
    }

    agora = time(NULL);
    horario_local = localtime(&agora);
    if (horario_local == NULL) {
        fclose(arquivo);
        return 0;
    }
    horario = *horario_local;

    if (strftime(data_hora, sizeof(data_hora), "%Y-%m-%d %H:%M:%S",
                 &horario) == 0) {
        fclose(arquivo);
        return 0;
    }

    sucesso = fprintf(arquivo,
                      "%s|%s|%s|%.8f|%.8f|%.2f|%.8f|%.8f|%.8f|%.8f|%.8f\n",
                      data_hora, tipo, moeda, valor, cotacao, taxa * 100.0,
                      valor * taxa, investidor->saldo_reais,
                      investidor->saldo_moedas[0], investidor->saldo_moedas[1],
                      investidor->saldo_moedas[2]) >= 0;
    if (fclose(arquivo) != 0) {
        sucesso = 0;
    }

    return sucesso;
}
