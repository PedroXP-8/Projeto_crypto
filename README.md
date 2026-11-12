# Exchange de Criptomoedas em C

Projeto de console com persistencia em arquivos de texto. O programa trabalha
com Bitcoin (BTC), Ethereum (ETH) e Ripple (XRP).

## Compilar e executar

O codigo esta dividido em varios arquivos `.c`, que devem ser compilados juntos.
Com G

```text
gcc -std=c11 -Wall -Wextra -pedantic main.c entrada.c arquivos.c cotacoes.c autenticacao.c operacoes.c -o exchange
```

Entre dentro da pasta 'arquivos_de_persistência' e execute:

```text
.\exchange.exe
```

Execute o programa a partir desta pasta, pois os arquivos de dados ficam no
diretorio atual.

## Organizacao do codigo

| Arquivo | Conteudo |
|---|---|
| `tipos.h` | constantes, estruturas `Investidor` e `Criptomoeda` e o vetor `moedas` |
| `entrada.c/.h` | leitura e validacao de texto, numeros, opcoes, CPF e senha |
| `arquivos.c/.h` | criacao, leitura e gravacao dos arquivos de dados e do extrato |
| `cotacoes.c/.h` | vetor de moedas, exibicao e atualizacao das cotacoes |
| `autenticacao.c/.h` | login e confirmacao de senha |
| `operacoes.c/.h` | saldo, extrato, deposito, saque, compra e venda |
| `main.c` | menu principal |

## Conta de demonstracao

- CPF: `12345678900`
- Senha: `123456`

Essa conta e ficticia e existe para permitir o primeiro acesso. O sistema nao
tem cadastro nem menu administrativo, conforme o escopo individual.

## Arquivos de dados

- `investidor.txt`: identificacao, senha e saldos da carteira.
- `cotacoes.txt`: cotacoes atuais de BTC, ETH e XRP.
- `extrato.txt`: movimentacoes e saldos registrados depois de cada operacao.

Os arquivos sao criados com valores de demonstracao quando ainda nao existem.
As alteracoes sao gravadas durante o uso do programa e continuam disponiveis
quando ele for aberto novamente. Os arquivos usam o caractere `|` para separar
os campos. Nao apague nem edite esses arquivos enquanto o programa estiver
aberto.

## Regras implementadas

- Compra: BTC 2%, ETH 1% e XRP 1%. A taxa e acrescentada ao valor em reais
  informado e precisa caber no saldo disponivel.
- Venda: BTC 3%, ETH 2% e XRP 1%. A taxa e descontada do valor bruto da venda.
- Cotacoes iniciais de demonstracao: BTC R$ 350.000,00, ETH R$ 18.000,00 e
  XRP R$ 3,00.
- Cada atualizacao muda cada cotacao aleatoriamente em um percentual entre
  -5% e +5% da cotacao anterior.
- Valores devem ser digitados com ponto decimal, por exemplo `125.50`.
- Consulta de saldo, consulta de extrato, compra e venda pedem a senha novamente.

## Formato do extrato

O extrato mostra o nome e o CPF do investidor e, em seguida, uma linha por
movimentacao:

```text
05-11-2026 11:56 + 100.00 XRP CT: 3.00 TX: 0.01 REAL: 10299.00 BTC: 0.01000000 ETH: 0.25000000 XRP: 133.33333333
```

Data e hora (`DD-MM-AAAA HH:MM`), sinal (`+` para deposito e compra, `-` para
saque e venda), valor em reais e moeda, cotacao (`CT`), taxa aplicada (`TX`,
em fracao: `0.01` = 1%) e saldos de reais, BTC, ETH e XRP apos a operacao.

## Relatorio

O documento [relatorio.md](relatorio.md) descreve os requisitos, a organizacao
do programa, a persistencia e um roteiro de verificacao manual.
