# Exchange de Criptomoedas em C

Projeto de console, em linguagem C, que simula uma exchange de criptomoedas.
O investidor entra com CPF e senha, consulta saldo e extrato, deposita e saca
reais, compra e vende Bitcoin (BTC), Ethereum (ETH) e Ripple (XRP) e atualiza as
cotações. Todos os dados ficam gravados em arquivos de texto, então nada se perde
ao fechar o programa.

Disciplina: Desenvolvimento de Algoritmos (CCP230) — projeto individual.

> Programa acadêmico: as cotações são fictícias e nenhuma operação é real.

## Estrutura de pastas

```text
Projeto_crypto/
  codigo/                      arquivos .c (código-fonte)
  arquivos_h/                  arquivos .h (cabeçalhos)
  arquivos_de_persistencia/    arquivos .txt de dados e o executável
  README.md
  Relatorio_Exchange_Criptomoedas.docx
```

## Como compilar e executar

Todos os arquivos `.c` precisam ser compilados juntos. Na pasta do projeto (a que
contém `codigo`, `arquivos_h` e `arquivos_de_persistencia`), com o GCC:

```text
gcc -std=c11 -Wall -Wextra -pedantic codigo/main.c codigo/entrada.c codigo/arquivos.c codigo/cotacoes.c codigo/autenticacao.c codigo/operacoes.c -o arquivos_de_persistencia/exchange
```

O executável é gerado dentro de `arquivos_de_persistencia`. Entre nessa pasta e
execute:

```text
cd arquivos_de_persistencia
.\exchange.exe        (Windows)
./exchange            (Linux/macOS)
```

Observações:

- O programa procura os arquivos `.txt` na **pasta onde é executado**. Por isso,
  execute-o de dentro de `arquivos_de_persistencia`.
- Não é preciso usar a opção `-I`: os `#include` dos arquivos `.c` já apontam para
  `../arquivos_h/`.
- Se aparecer o erro `undefined reference`, algum arquivo `.c` ficou fora do
  comando de compilação.

## Conta de demonstração

O investidor de demonstração fica no arquivo `investidor.txt` (CPF, senha, nome e
saldos). A senha inicial é `123456`.

Na primeira execução, se o arquivo não existir, ele é criado com R$ 10.000,00,
0,01 BTC, 0,25 ETH e 100 XRP. O sistema não tem cadastro de usuários nem menu
administrativo, conforme o escopo individual.

## Menu

Depois do login (CPF com 11 números e senha de 6 dígitos, com até 3 tentativas):

```text
1 - Consultar saldo
2 - Consultar extrato
3 - Depositar reais
4 - Sacar reais
5 - Comprar criptomoedas
6 - Vender criptomoedas
7 - Atualizar cotações
8 - Sair
```

Saldo, extrato, compra e venda pedem a senha novamente.

## Regras implementadas

| Moeda | Taxa na compra | Taxa na venda | Cotação inicial |
|---|---|---|---|
| Bitcoin (BTC) | 2% | 3% | R$ 350.000,00 |
| Ethereum (ETH) | 1% | 2% | R$ 18.000,00 |
| Ripple (XRP) | 1% | 1% | R$ 3,00 |

- **Compra:** o usuário informa o valor em reais. A taxa é somada a esse valor
  (`total = valor × (1 + taxa)`) e a quantidade comprada é `valor ÷ cotação`. A
  compra só é aceita se o saldo cobrir o total.
- **Venda:** o usuário informa a quantidade de moedas. O valor bruto é
  `quantidade × cotação` e a taxa é descontada dele (`líquido = bruto × (1 − taxa)`).
- **Depósito e saque:** o saque só é aceito se houver saldo suficiente. O saldo
  nunca fica negativo.
- **Atualização das cotações:** cada moeda é multiplicada por um fator aleatório
  entre 0,95 e 1,05, ou seja, variação de -5% a +5% sobre o valor anterior.
- **Entradas:** valores devem ser digitados com ponto decimal (por exemplo,
  `125.50`). Entradas inválidas são recusadas com uma mensagem.
- **Falhas de gravação:** cada operação guarda uma cópia do saldo anterior e a
  restaura se não conseguir gravar os arquivos.

## Organização do código

| Módulo | Arquivos | Conteúdo |
|---|---|---|
| Tipos | `tipos.h` | constantes, estruturas `Investidor` e `Criptomoeda` e o vetor `moedas` |
| Entrada | `entrada.c/.h` | `ler_texto`, `ler_valor_positivo`, `ler_opcao`, `cpf_valido`, `senha_valida` |
| Arquivos | `arquivos.c/.h` | criar, carregar e salvar os dados e `registrar_movimentacao` |
| Cotações | `cotacoes.c/.h` | vetor de moedas, `mostrar_cotacoes`, `escolher_moeda`, `atualizar_cotacoes` |
| Autenticação | `autenticacao.c/.h` | `autenticar` e `confirmar_senha` |
| Operações | `operacoes.c/.h` | saldo, extrato, depósito, saque, compra e venda |
| Principal | `main.c` | menu e laço principal do programa |

## Arquivos de dados

Ficam em `arquivos_de_persistencia`, em texto simples, com os campos separados
pelo caractere `|`:

- `investidor.txt`: uma linha com `cpf|senha|nome|reais|btc|eth|xrp`.
- `cotacoes.txt`: uma linha por moeda, como `BTC|350000.00`.
- `extrato.txt`: uma linha por movimentação, sempre acrescentada ao final.

Arquivos que não existem são criados com valores de demonstração; os que já
existem são lidos e nunca sobrescritos na abertura. Não apague nem edite esses
arquivos enquanto o programa estiver aberto.

## Formato do extrato

O extrato mostra o nome e o CPF do investidor e, em seguida, uma linha por
movimentação:

```text
09-11-2026 15:15 + 1000.00 XRP CT: 3.66 TX: 0.01 REAL: 8990.00 BTC: 0.01000000 ETH: 0.25000000 XRP: 373.12271295
```

- Data e hora no formato `DD-MM-AAAA HH:MM`.
- Sinal: `+` para depósito e compra, `-` para saque e venda.
- Valor em reais e a moeda (`REAL`, `BTC`, `ETH` ou `XRP`).
- `CT`: cotação usada. `TX`: taxa aplicada, em fração (`0.01` = 1%).
- Saldos de reais, BTC, ETH e XRP depois da operação.

## Roteiro de teste

1. Entrar com dados corretos e com dados errados.
2. Consultar o saldo com a senha certa e com uma senha errada.
3. Depositar, conferir o saldo, tentar sacar mais do que existe e depois sacar um
   valor menor.
4. Comprar e vender, conferindo taxa e saldos; tentar comprar sem saldo e vender
   mais do que se tem.
5. Atualizar as cotações e conferir que cada uma mudou em até 5%.
6. Consultar o extrato.
7. Fechar e abrir o programa de novo e conferir que tudo continua salvo.

## Limitações

- A senha é gravada em texto no arquivo do investidor.
- Os valores em reais usam `double`, sem tratamento exato de centavos.
- Existe um único investidor, sem cadastro.
- Os arquivos são regravados por inteiro, sem proteção contra queda de energia no
  meio da gravação.

## Relatório

O documento `Relatorio_Exchange_Criptomoedas.docx` descreve o projeto, a
organização do código, a persistência dos dados e as dificuldades enfrentadas.
