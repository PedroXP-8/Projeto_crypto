#ifndef TIPOS_H
#define TIPOS_H

#define CPF_TAMANHO 12
#define SENHA_TAMANHO 7
#define NOME_TAMANHO 80
#define QUANTIDADE_MOEDAS 3
#define LINHA_TAMANHO 512

#define ARQUIVO_INVESTIDOR "investidor.txt"
#define ARQUIVO_COTACOES "cotacoes.txt"
#define ARQUIVO_EXTRATO "extrato.txt"

typedef struct {
    char cpf[CPF_TAMANHO];
    char senha[SENHA_TAMANHO];
    char nome[NOME_TAMANHO];
    double saldo_reais;
    double saldo_moedas[QUANTIDADE_MOEDAS];
} Investidor;

typedef struct {
    char simbolo[4];
    char nome[16];
    double cotacao;
    double taxa_compra;
    double taxa_venda;
} Criptomoeda;

/* Vetor de moedas, definido em cotacoes.c */
extern Criptomoeda moedas[QUANTIDADE_MOEDAS];

#endif
