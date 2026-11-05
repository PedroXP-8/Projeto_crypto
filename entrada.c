#include <ctype.h>
#include <errno.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "entrada.h"

int ler_texto(const char *mensagem, char *destino, size_t tamanho)
{
    size_t comprimento;
    int caractere;

    printf("%s", mensagem);
    if (fgets(destino, (int)tamanho, stdin) == NULL) {
        return 0;
    }

    comprimento = strcspn(destino, "\n");
    if (destino[comprimento] == '\n') {
        destino[comprimento] = '\0';
    } else if (!feof(stdin)) {
        caractere = getchar();
        if (caractere != '\n' && caractere != EOF) {
            while ((caractere = getchar()) != '\n' && caractere != EOF) {
            }
            printf("Entrada muito longa. Tente novamente.\n");
            return 0;
        }
    }

    return 1;
}

int ler_valor_positivo(const char *mensagem, double *valor)
{
    char texto[64];
    char *fim;
    double numero;

    if (!ler_texto(mensagem, texto, sizeof(texto))) {
        return 0;
    }

    errno = 0;
    numero = strtod(texto, &fim);
    while (isspace((unsigned char)*fim)) {
        fim++;
    }

    if (fim == texto || *fim != '\0' || errno == ERANGE ||
        !isfinite(numero) || numero <= 0.0) {
        printf("Informe um valor numerico maior que zero, usando ponto decimal.\n");
        return 0;
    }

    *valor = numero;
    return 1;
}

int ler_opcao(const char *mensagem, int minimo, int maximo)
{
    char texto[32];
    char *fim;
    long opcao;

    if (!ler_texto(mensagem, texto, sizeof(texto))) {
        return -1;
    }

    errno = 0;
    opcao = strtol(texto, &fim, 10);
    while (isspace((unsigned char)*fim)) {
        fim++;
    }

    if (fim == texto || *fim != '\0' || errno == ERANGE ||
        opcao < minimo || opcao > maximo) {
        printf("Opcao invalida.\n");
        return 0;
    }

    return (int)opcao;
}

int cpf_valido(const char *cpf)
{
    size_t i;

    if (strlen(cpf) != 11) {
        return 0;
    }

    for (i = 0; i < 11; i++) {
        if (!isdigit((unsigned char)cpf[i])) {
            return 0;
        }
    }

    return 1;
}

int senha_valida(const char *senha)
{
    size_t i;

    if (strlen(senha) != 6) {
        return 0;
    }

    for (i = 0; i < 6; i++) {
        if (!isdigit((unsigned char)senha[i])) {
            return 0;
        }
    }

    return 1;
}
