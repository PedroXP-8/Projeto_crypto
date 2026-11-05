#include <stdio.h>
#include <stdlib.h>

#include "arquivos.h"
#include "cotacoes.h"
#include "entrada.h"
#include "tipos.h"

Criptomoeda moedas[QUANTIDADE_MOEDAS] = {
    {"BTC", "Bitcoin", 350000.00, 0.02, 0.03},
    {"ETH", "Ethereum", 18000.00, 0.01, 0.02},
    {"XRP", "Ripple", 3.00, 0.01, 0.01}
};

void mostrar_cotacoes(void)
{
    printf("\n--- Cotacoes atuais ---\n");
    for (int i = 0; i < QUANTIDADE_MOEDAS; i++) {
        printf("%d - %s (%s): R$ %.2f | compra %.0f%% | venda %.0f%%\n",
               i + 1, moedas[i].nome, moedas[i].simbolo, moedas[i].cotacao,
               moedas[i].taxa_compra * 100.0, moedas[i].taxa_venda * 100.0);
    }
}

int escolher_moeda(void)
{
    int opcao;

    mostrar_cotacoes();
    opcao = ler_opcao("Escolha a moeda (1 a 3, ou 0 para cancelar): ", 0,
                      QUANTIDADE_MOEDAS);
    if (opcao <= 0) {
        return -1;
    }

    return opcao - 1;
}

void atualizar_cotacoes(void)
{
    double cotacoes_anteriores[QUANTIDADE_MOEDAS];

    for (int i = 0; i < QUANTIDADE_MOEDAS; i++) {
        cotacoes_anteriores[i] = moedas[i].cotacao;
        moedas[i].cotacao *= 1.0 + ((double)rand() / RAND_MAX * 0.10 - 0.05);
    }

    if (!salvar_cotacoes()) {
        for (int i = 0; i < QUANTIDADE_MOEDAS; i++) {
            moedas[i].cotacao = cotacoes_anteriores[i];
        }
        printf("Erro ao salvar as cotacoes. Os valores anteriores foram mantidos.\n");
        return;
    }

    printf("Cotacoes atualizadas. Cada variacao ficou entre -5%% e +5%%.\n");
    mostrar_cotacoes();
}
