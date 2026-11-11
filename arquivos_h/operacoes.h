#ifndef OPERACOES_H
#define OPERACOES_H

#include "tipos.h"

void mostrar_saldos(const Investidor *investidor);
void consultar_extrato(const Investidor *investidor);
void depositar(Investidor *investidor);
void sacar(Investidor *investidor);
void comprar(Investidor *investidor);
void vender(Investidor *investidor);

#endif
