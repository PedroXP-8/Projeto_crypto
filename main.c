#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "arquivos.h"
#include "autenticacao.h"
#include "cotacoes.h"
#include "entrada.h"
#include "operacoes.h"
#include "tipos.h"

void mostrar_menu(void)
{
    printf("\n--- Exchange de Criptomoedas ---\n");
    printf("1 - Consultar saldo\n");
    printf("2 - Consultar extrato\n");
    printf("3 - Depositar reais\n");
    printf("4 - Sacar reais\n");
    printf("5 - Comprar criptomoedas\n");
    printf("6 - Vender criptomoedas\n");
    printf("7 - Atualizar cotacoes\n");
    printf("8 - Sair\n");
}

int main(void)
{
    Investidor investidor;
    int opcao;

    if (!criar_arquivos_iniciais()) {
        return 1;
    }

    if (!carregar_investidor(&investidor) || !carregar_cotacoes()) {
        return 1;
    }

    srand((unsigned int)time(NULL));
    printf("Exchange de Criptomoedas\n");
    if (!autenticar(&investidor)) {
        return 0;
    }

    do {
        mostrar_menu();
        opcao = ler_opcao("Escolha uma opcao: ", 1, 8);

        switch (opcao) {
        case 1:
            if (confirmar_senha(&investidor)) {
                mostrar_saldos(&investidor);
            }
            break;
        case 2:
            consultar_extrato(&investidor);
            break;
        case 3:
            depositar(&investidor);
            break;
        case 4:
            sacar(&investidor);
            break;
        case 5:
            comprar(&investidor);
            break;
        case 6:
            vender(&investidor);
            break;
        case 7:
            atualizar_cotacoes();
            break;
        case 8:
            printf("Saindo do sistema.\n");
            break;
        default:
            break;
        }
    } while (opcao != 8 && opcao != -1);

    return 0;
}
