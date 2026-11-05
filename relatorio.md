# Relatorio do Projeto: Exchange de Criptomoedas

**Disciplina:** ____________________________________  
**Aluno(a):** _____________________________________  
**Professor(a):** __________________________________  
**Data:** _________________________________________

## 1. Apresentacao e objetivo

Este trabalho apresenta uma exchange de criptomoedas feita em linguagem C. O
programa funciona em modo texto e permite que um investidor consulte sua
carteira, movimente reais e negocie Bitcoin (BTC), Ethereum (ETH) e Ripple
(XRP). O objetivo foi atender aos requisitos propostos usando recursos
introdutorios da linguagem: variaveis, estruturas, vetores, funcoes, decisoes,
repeticoes e manipulacao de arquivos.

O sistema foi desenvolvido para um unico investidor de demonstracao, pois o
projeto e individual. Por isso, nao ha cadastro de usuarios ou menu
administrativo. A conta ficticia inicial permite executar o programa e testar
as operacoes sem configurar dados antes do primeiro acesso.

O programa nao representa uma exchange real nem realiza operacoes financeiras.
As cotacoes sao valores de demonstracao, os dados ficam no computador local e
a senha e armazenada em texto no arquivo do investidor. Essa escolha mantem o
exemplo pequeno e compreensivel para fins academicos; nao deve ser usada em um
sistema financeiro de producao.

## 2. Requisitos atendidos

O acesso solicita CPF com onze digitos, sem pontuacao, e senha numerica com seis
digitos. A conta inicial usa CPF `12345678900` e senha `123456`. Depois do
login, o menu apresenta consulta de saldo, consulta de extrato, deposito,
saque, compra, venda, atualizacao de cotacoes e saida. Para consultar saldo ou
extrato e para comprar ou vender criptomoedas, o investidor confirma a senha
novamente.

O saldo mostra nome, CPF, reais disponiveis e quantidade de cada moeda. O
extrato mostra o nome e o CPF do investidor e, para cada movimentacao, data e
hora, sinal (`+` ou `-`), valor e moeda, cotacao (`CT`), taxa (`TX`) e os saldos
resultantes em reais, BTC, ETH e XRP. Depositos aumentam o saldo em reais;
saques so sao aceitos se houver saldo suficiente. Os dois tipos de
movimentacao sao registrados no extrato.

Na compra, o usuario escolhe uma moeda e informa o valor em reais que deseja
converter, antes da taxa. A quantidade adquirida e calculada dividindo esse
valor pela cotacao. A taxa e somada ao valor convertido, e a operacao so e
aceita se o saldo cobrir o total. Na venda, o usuario informa a quantidade de
moedas; o sistema verifica a carteira, calcula o valor bruto pela cotacao e
desconta a taxa antes de creditar os reais.

As taxas sao fixas: BTC tem 2% na compra e 3% na venda; ETH tem 1% na compra e
2% na venda; XRP tem 1% tanto na compra quanto na venda. Uma atualizacao
aleatoria altera cada cotacao por um fator entre 0,95 e 1,05, preservando
assim o limite de variacao de -5% a +5% em relacao ao valor imediatamente
anterior.

\newpage

## 3. Organizacao e funcionamento

O codigo esta dividido em modulos, cada um com um arquivo `.c` e seu cabecalho
`.h`: `entrada` (leitura e validacao de dados digitados), `arquivos`
(persistencia), `cotacoes` (lista de moedas e atualizacao dos precos),
`autenticacao` (login e confirmacao de senha) e `operacoes` (saldo, extrato,
deposito, saque, compra e venda). O arquivo `tipos.h` reune as constantes e as
estruturas, e o `main.c` contem apenas o menu principal. As tarefas ficam
separadas em funcoes pequenas.

A estrutura `Investidor` agrupa CPF, senha, nome, saldo em reais e saldos das
tres criptomoedas. A estrutura `Criptomoeda` agrupa simbolo, nome, cotacao e
taxas. Um vetor com tres elementos representa as moedas disponiveis. Essa
organizacao evita manter muitas variaveis soltas e permite percorrer as moedas
com repeticoes simples.

As funcoes de entrada leem texto com `fgets` e convertem numeros com `strtod`
ou `strtol`. Antes de aceitar uma operacao, o programa verifica entradas
invalidas, valores positivos, formato do CPF e tamanho da senha. Tambem confere
saldo suficiente e evita que um saque ou uma compra deixem o saldo negativo.
Mensagens no console explicam quando uma acao nao pode ser concluida.

O menu principal repete as opcoes ate que o investidor escolha sair. Cada
opcao chama a funcao correspondente: consultar os saldos, ler o extrato,
depositar, sacar, comprar, vender ou atualizar cotacoes. As operacoes de
compra e venda primeiro apresentam as cotacoes e taxas para que o usuario
possa escolher a moeda. Os valores monetarios sao apresentados com duas casas
decimais; as quantidades de criptomoedas usam oito casas.

Para registrar uma operacao, o programa atualiza a carteira, grava o arquivo
do investidor e acrescenta uma linha ao extrato. Se nao conseguir salvar a
carteira, restaura os saldos anteriores. Se falhar ao gravar o extrato, tenta
restaurar a carteira e informa o problema ao usuario. A atualizacao das
cotacoes tambem e mantida em memoria somente quando a gravacao no arquivo
termina com sucesso.

\newpage

## 4. Persistencia dos dados

Os arquivos ficam junto do executavel e sao texto simples, separados pelo
caractere `|`. `investidor.txt` guarda uma linha com os dados do investidor e
os quatro saldos. `cotacoes.txt` contem uma linha para cada moeda e sua
cotacao. `extrato.txt` recebe uma linha por movimentacao. Na primeira
execucao, os arquivos ausentes sao criados com valores iniciais de
demonstracao; nas execucoes seguintes, o programa carrega os valores
existentes, sem sobrescreve-los.

O extrato guarda as informacoes necessarias para reconstruir o historico:
momento da operacao, tipo, moeda, valor, cotacao utilizada, percentual da taxa,
valor da taxa e saldos apos a operacao. Consultar o extrato le essas linhas e
as apresenta em formato legivel: data como `DD-MM-AAAA HH:MM`, sinal `+` para
deposito e compra e `-` para saque e venda, e a taxa (`TX`) em fracao, por
exemplo `0.01` para 1%. Uma linha malformada e sinalizada em vez de ser
apresentada como se fosse uma movimentacao valida.

A persistencia e direta e propositalmente simples: nao ha banco de dados,
bibliotecas externas ou formatos binarios. Para executar corretamente, deve-se
iniciar o programa a partir da pasta que contem os arquivos de dados. Uma
copia de seguranca desses arquivos pode preservar o estado da conta entre
computadores.

\newpage

## 5. Roteiro de verificacao e conclusao

O roteiro abaixo pode ser usado na demonstracao presencial:

1. Compilar com `gcc -std=c11 -Wall -Wextra -pedantic main.c entrada.c arquivos.c cotacoes.c autenticacao.c operacoes.c -o exchange`.
2. Entrar com o CPF e a senha de demonstracao. Testar tambem um CPF ou senha
   incorretos e confirmar que o acesso nao e liberado.
3. Consultar o saldo com a senha correta e testar uma senha incorreta.
4. Depositar um valor, consultar o saldo e confirmar que o novo valor aparece.
5. Tentar sacar mais reais do que estao disponiveis e verificar que a operacao
   e recusada. Depois, realizar um saque menor.
6. Comprar BTC, ETH ou XRP com saldo suficiente; conferir a taxa, o saldo em
   reais e a quantidade adquirida. Tentar uma compra sem saldo suficiente.
7. Vender uma quantidade disponivel e depois tentar vender uma quantidade
   superior a carteira.
8. Atualizar cotacoes e conferir que cada valor mudou em ate 5%. Consultar o
   extrato para verificar nome, CPF, data, sinal, valores, taxa e saldos finais.
9. Fechar e abrir novamente o programa, entrar e confirmar que os saldos,
   cotacoes e movimentacoes continuam gravados.

O projeto demonstra como dividir um programa de console em modulos e funcoes,
validar entradas e persistir informacoes usando arquivos de texto. A separacao
entre dados do investidor, cotacoes e extrato facilita a leitura e a explicacao
do funcionamento. Como melhoria futura, poderiam ser adicionados cadastro de
usuarios, armazenamento seguro de senhas e mecanismos mais robustos para
proteger arquivos contra falhas durante gravacoes. Esses itens nao fazem
parte do escopo individual implementado.
