#ifndef ENTRADA_H
#define ENTRADA_H

#include <stddef.h>

int ler_texto(const char *mensagem, char *destino, size_t tamanho);
int ler_valor_positivo(const char *mensagem, double *valor);
int ler_opcao(const char *mensagem, int minimo, int maximo);
int cpf_valido(const char *cpf);
int senha_valida(const char *senha);

#endif
