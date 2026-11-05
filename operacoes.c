#include <math.h>
#include <stdio.h>
#include <string.h>

#include "arquivos.h"
#include "autenticacao.h"
#include "cotacoes.h"
#include "entrada.h"
#include "operacoes.h"

static void imprimir_cpf(const char *cpf)
{
    printf("%.3s.%.3s.%.3s-%.2s", cpf, cpf + 3, cpf + 6, cpf + 9);
}

void mostrar_saldos(const Investidor *investidor)
{
    printf("\nNome: %s\n", investidor->nome);
    printf("CPF: ");
    imprimir_cpf(investidor->cpf);
    printf("\n");
    printf("Reais: %.2f\n", investidor->saldo_reais);
    for (int i = 0; i < QUANTIDADE_MOEDAS; i++) {
        printf("%s: %.8f\n", moedas[i].nome, investidor->saldo_moedas[i]);
    }
}

static int concluir_operacao(Investidor *investidor,
                             const Investidor *saldo_anterior,
                             const char *tipo,
                             const char *moeda,
                             double valor,
                             double cotacao,
                             double taxa)
{
    if (!salvar_investidor(investidor)) {
        *investidor = *saldo_anterior;
        printf("Erro ao salvar o saldo. A operacao foi cancelada.\n");
        return 0;
    }

    if (!registrar_movimentacao(investidor, tipo, moeda, valor, cotacao, taxa)) {
        *investidor = *saldo_anterior;
        if (!salvar_investidor(investidor)) {
            printf("Erro ao salvar o extrato e ao restaurar o saldo. "
                   "Verifique os arquivos de dados.\n");
        } else {
            printf("Erro ao gravar o extrato. O saldo anterior foi restaurado.\n");
        }
        return 0;
    }

    printf("Operacao concluida com sucesso.\n");
    mostrar_saldos(investidor);
    return 1;
}

void consultar_extrato(const Investidor *investidor)
{
    FILE *arquivo;
    char linha[LINHA_TAMANHO];
    int quantidade = 0;

    if (!confirmar_senha(investidor)) {
        return;
    }

    arquivo = fopen(ARQUIVO_EXTRATO, "r");
    if (arquivo == NULL) {
        printf("Nao foi possivel abrir o extrato.\n");
        return;
    }

    printf("\n--- Extrato de movimentacoes ---\n");
    printf("Nome: %s\n", investidor->nome);
    printf("CPF: ");
    imprimir_cpf(investidor->cpf);
    printf("\n");
    while (fgets(linha, sizeof(linha), arquivo) != NULL) {
        char data_hora[20];
        char tipo[20];
        char moeda[8];
        double valor, cotacao, taxa, valor_taxa;
        double saldo_reais, saldo_btc, saldo_eth, saldo_xrp;
        char data_formatada[32];
        int ano, mes, dia, hora, minuto;
        int campos = sscanf(linha,
                            "%19[^|]|%19[^|]|%3[^|]|%lf|%lf|%lf|%lf|%lf|%lf|%lf|%lf",
                            data_hora, tipo, moeda, &valor, &cotacao, &taxa,
                            &valor_taxa, &saldo_reais, &saldo_btc, &saldo_eth,
                            &saldo_xrp);

        if (strspn(linha, " \t\r\n") == strlen(linha)) {
            continue;
        }

        if (campos == 11) {
            char sinal = (strcmp(tipo, "Deposito") == 0 ||
                          strcmp(tipo, "Compra") == 0) ? '+' : '-';

            if (sscanf(data_hora, "%d-%d-%d %d:%d", &ano, &mes, &dia, &hora,
                       &minuto) == 5) {
                snprintf(data_formatada, sizeof(data_formatada),
                         "%02d-%02d-%04d %02d:%02d", dia, mes, ano, hora, minuto);
            } else {
                snprintf(data_formatada, sizeof(data_formatada), "%s", data_hora);
            }
            if (strcmp(moeda, "BRL") == 0) {
                strcpy(moeda, "REAL");
            }

            printf("%s %c %.2f %s CT: %.2f TX: %.2f REAL: %.2f BTC: %.8f"
                   " ETH: %.8f XRP: %.8f\n",
                   data_formatada, sinal, valor, moeda, cotacao, taxa / 100.0,
                   saldo_reais, saldo_btc, saldo_eth, saldo_xrp);
            quantidade++;
        } else {
            printf("Uma linha do extrato esta invalida e nao pode ser exibida.\n");
        }
    }

    if (fclose(arquivo) != 0) {
        printf("Erro ao fechar o arquivo do extrato.\n");
    }
    if (quantidade == 0) {
        printf("Ainda nao ha movimentacoes registradas.\n");
    }
}

void depositar(Investidor *investidor)
{
    Investidor anterior = *investidor;
    double valor;

    if (!ler_valor_positivo("Valor do deposito em reais: ", &valor)) {
        return;
    }
    if (!isfinite(investidor->saldo_reais + valor)) {
        printf("O valor informado e muito alto.\n");
        return;
    }

    investidor->saldo_reais += valor;
    concluir_operacao(investidor, &anterior, "Deposito", "BRL", valor, 0.0, 0.0);
}

void sacar(Investidor *investidor)
{
    Investidor anterior = *investidor;
    double valor;

    if (!ler_valor_positivo("Valor do saque em reais: ", &valor)) {
        return;
    }
    if (valor > investidor->saldo_reais) {
        printf("Saldo insuficiente para realizar o saque.\n");
        return;
    }

    investidor->saldo_reais -= valor;
    concluir_operacao(investidor, &anterior, "Saque", "BRL", valor, 0.0, 0.0);
}

void comprar(Investidor *investidor)
{
    Investidor anterior;
    double valor, taxa, total, quantidade;
    int indice;

    if (!confirmar_senha(investidor)) {
        return;
    }

    indice = escolher_moeda();
    if (indice < 0 || !ler_valor_positivo(
            "Valor em reais para converter em criptomoeda (sem a taxa): ",
            &valor)) {
        return;
    }

    taxa = moedas[indice].taxa_compra;
    total = valor * (1.0 + taxa);
    quantidade = valor / moedas[indice].cotacao;
    if (!isfinite(total) || !isfinite(quantidade)) {
        printf("O valor informado e muito alto.\n");
        return;
    }
    if (total > investidor->saldo_reais) {
        printf("Saldo insuficiente para a compra e sua taxa de %.0f%%.\n",
               taxa * 100.0);
        return;
    }

    anterior = *investidor;
    investidor->saldo_reais -= total;
    investidor->saldo_moedas[indice] += quantidade;
    if (!isfinite(investidor->saldo_moedas[indice])) {
        *investidor = anterior;
        printf("O valor informado e muito alto.\n");
        return;
    }

    printf("Taxa: R$ %.2f | Total debitado: R$ %.2f\n", valor * taxa, total);
    concluir_operacao(investidor, &anterior, "Compra", moedas[indice].simbolo,
                      valor, moedas[indice].cotacao, taxa);
}

void vender(Investidor *investidor)
{
    Investidor anterior;
    double quantidade, valor, taxa, liquido;
    int indice;

    if (!confirmar_senha(investidor)) {
        return;
    }

    indice = escolher_moeda();
    if (indice < 0 || !ler_valor_positivo(
            "Quantidade de criptomoeda para vender: ", &quantidade)) {
        return;
    }
    if (quantidade > investidor->saldo_moedas[indice]) {
        printf("Saldo insuficiente dessa criptomoeda.\n");
        return;
    }

    valor = quantidade * moedas[indice].cotacao;
    taxa = moedas[indice].taxa_venda;
    liquido = valor * (1.0 - taxa);
    if (!isfinite(valor) || !isfinite(liquido) ||
        !isfinite(investidor->saldo_reais + liquido)) {
        printf("Nao foi possivel calcular essa venda.\n");
        return;
    }

    anterior = *investidor;
    investidor->saldo_moedas[indice] -= quantidade;
    if (investidor->saldo_moedas[indice] < 0.000000001) {
        investidor->saldo_moedas[indice] = 0.0;
    }
    investidor->saldo_reais += liquido;

    printf("Valor bruto: R$ %.2f | Taxa: R$ %.2f | Valor recebido: R$ %.2f\n",
           valor, valor * taxa, liquido);
    concluir_operacao(investidor, &anterior, "Venda", moedas[indice].simbolo,
                      valor, moedas[indice].cotacao, taxa);
}
