#ifndef ARQUIVOS_H
#define ARQUIVOS_H

#include "tipos.h"

int garantir_arquivo(const char *caminho, const char *conteudo_inicial);
int criar_arquivos_iniciais(void);
int carregar_investidor(Investidor *investidor);
int carregar_cotacoes(void);
int salvar_investidor(const Investidor *investidor);
int salvar_cotacoes(void);
int registrar_movimentacao(const Investidor *investidor,
                           const char *tipo,
                           const char *moeda,
                           double valor,
                           double cotacao,
                           double taxa);

#endif
