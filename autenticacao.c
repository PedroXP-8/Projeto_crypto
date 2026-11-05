#include <stdio.h>
#include <string.h>

#include "autenticacao.h"
#include "entrada.h"

int confirmar_senha(const Investidor *investidor)
{
    char senha[SENHA_TAMANHO];

    if (!ler_texto("Confirme sua senha de 6 digitos: ", senha, sizeof(senha))) {
        return 0;
    }

    if (!senha_valida(senha) || strcmp(senha, investidor->senha) != 0) {
        printf("Senha incorreta.\n");
        return 0;
    }

    return 1;
}

int autenticar(const Investidor *investidor)
{
    char cpf[CPF_TAMANHO];
    char senha[SENHA_TAMANHO];

    for (int tentativa = 1; tentativa <= 3; tentativa++) {
        if (!ler_texto("CPF (somente numeros): ", cpf, sizeof(cpf)) ||
            !ler_texto("Senha de 6 digitos: ", senha, sizeof(senha))) {
            return 0;
        }

        if (!cpf_valido(cpf) || !senha_valida(senha) ||
            strcmp(cpf, investidor->cpf) != 0 ||
            strcmp(senha, investidor->senha) != 0) {
            printf("CPF ou senha incorretos. Tentativa %d de 3.\n", tentativa);
        } else {
            printf("\nLogin realizado. Bem-vindo(a), %s!\n", investidor->nome);
            return 1;
        }
    }

    printf("Numero maximo de tentativas atingido.\n");
    return 0;
}
