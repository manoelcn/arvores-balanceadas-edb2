---
marp: true
theme: default
paginate: true
size: 16:9
math: katex
style: |
  section { font-size: 22px; padding: 36px 46px 30px; }
  h1 { color: #232F3E; font-size: 32px; margin-bottom: 0.28em; }
  h2 { color: #232F3E; font-size: 24px; }
  p { margin: 0.25em 0 0.4em; }
  ul { margin: 0.2em 0; }
  li { margin: 0.12em 0; }
  table { font-size: 18px; }
  pre { font-size: 15px; line-height: 1.25; }
  section.compact { font-size: 20px; }
  section.compact table { font-size: 15px; }
  section.compact h1 { font-size: 30px; }
  section.lead { text-align: center; }
  section.lead h1 { font-size: 46px; }
  section.lead p { font-size: 24px; }
  .katex { font-size: 1.05em; white-space: nowrap; }
  .katex-display { display: block; text-align: center; margin: 0.35em 0 0.55em; }
  section.compact .katex { font-size: 1em; }
---

<!-- _class: lead -->

# Árvores Balanceadas

**Manoel Cândido Neto**

Estruturas de Dados Básicas II

https://github.com/manoelcn/arvores-balanceadas-edb2

---

# O contexto

## Black Friday: o momento em que a logística não pode travar

Nesse dia, milhares de encomendas entram, são rastreadas e saem do sistema ao mesmo tempo. Cada uma é identificada por um número, e o sistema precisa lidar com três ações o tempo todo:

- **Registrar** o pacote que acabou de chegar
- **Localizar** o pacote quando o rastreio pedir
- **Remover** o pacote quando for entregue ou cancelado

**O problema**: em uma árvore de busca comum, se os números chegam em sequência, ela degenera em uma lista e a busca fica lenta justamente quando o volume é maior.

**O que proponho**: um protótipo com duas árvores balanceadas, a **AVL** e a **Rubro-Negra**, testadas na mesma carga, para escolher com dados qual delas sustenta melhor o rastreio na Black Friday.

---

# A proposta

## Um protótipo para testar duas árvores na mesma carga

Cada encomenda vira um número inteiro dentro do sistema. O protótipo trata esse número com as três operações do dia a dia da logística:

- **Registrar** quando o pacote chega
- **Buscar** quando o rastreio precisa localizá-lo
- **Remover** quando a entrega é concluída ou cancelada

Essas operações foram implementadas duas vezes, uma com **AVL** e outra com **Rubro-Negra**. As duas recebem a mesma sequência de dados, e a comparação mostra qual mantém a busca rápida quando o volume cresce.

---

# O que a altura muda na prática

Os identificadores ficam numa árvore binária de busca: valor menor à esquerda, maior à direita.

- A altura é a quantidade de saltos da raiz até o ponto mais fundo
- Uma busca percorre no máximo essa altura. Árvore baixa, rastreio curto
- Se os números chegam em ordem (1, depois 2, depois 3), cada um cai à direita do anterior
- A árvore vira uma lista. Com $n$ encomendas, a altura chega a $n - 1$
- Balancear reorganiza os ponteiros depois de inserir ou remover, para a altura não acompanhar $n$

---

# O desafio técnico

O experimento inseriu as chaves em ordem crescente e mediu a altura final. A BST da tabela é a árvore sem balanceamento, incluída só nesse cenário.

| Estrutura | n = 100 | n = 1000 | n = 10000 |
|-----------|--------:|---------:|----------:|
| BST | 99 | 999 | 9999 |
| AVL | 6 | 9 | 13 |
| Rubro-Negra | 10 | 16 | 23 |

Com 10000 encomendas em ordem, achar uma na BST pode levar 9999 comparações. Na AVL são 13 saltos; na Rubro-Negra, 23.

---

# A solução desenvolvida

O código é C++. A lógica das árvores é do projeto (`src/avl.cpp` e `src/rb.cpp`): o menu não pede para o operador escolher rotações.

- `bin/logistica` é o menu. A opção 1 abre a AVL; a opção 2 abre a Rubro-Negra
- Dá para inserir, remover, buscar, listar em ordem, imprimir a árvore e ver altura e contadores
- `bin/testes` confere as regras. `bin/experimento` mede as duas árvores na mesma carga
- Número já cadastrado é recusado. Buscar ou remover um número ausente deixa a árvore como está
- Vazia: altura $-1$. Um nó: altura $0$. Com dois filhos, a chave troca com o sucessor (o menor à direita) e esse sucessor é apagado

---

# Árvore AVL: estratégia

A AVL não deixa as duas subárvores de um nó diferirem em mais de um nível. Quem aplica isso é `rebalancear`, em `src/avl.cpp`, depois de cada inserção ou remoção.

O fator é $fb = \mathrm{altura}(\mathrm{esq}) - \mathrm{altura}(\mathrm{dir})$. O valor válido fica em $\{-1, 0, 1\}$.

- **LL:** o nó pende à esquerda, em linha. Uma `rotacaoDireita`
- **RR:** pende à direita, em linha. Uma `rotacaoEsquerda`
- **LR e RL:** o filho pende para o lado oposto. O programa gira o filho e depois o pai
- Cada giro simples soma 1 no contador. Uma rotação dupla aparece, portanto, como 2

---

# Árvore AVL: exemplo de rotação

Inserimos 10, 20 e 30 (`./bin/experimento --exemplos`). Na impressão, o filho direito fica acima do pai e o esquerdo fica abaixo.

```text
Apos 20:
    20(fb=0)
10(fb=-1)

Apos 30 (rotacao):
    30(fb=0)
20(fb=0)
    10(fb=0)
```

Depois do 20 a raiz ainda é 10 e o 20 está à direita, com $fb = -1$. O 30 levaria esse fator a $fb = -2$. Uma `rotacaoEsquerda` sobe o 20: 10 à esquerda e 30 à direita, todos com $fb = 0$. O contador marcou **1** rotação.

---

# Árvore Rubro-Negra: propriedades

A Rubro-Negra pinta cada nó e aceita uma árvore um pouco mais alta que a AVL. A função `verificarInvariantes`, no arquivo `rb.cpp`, confere cinco regras.

- Todo nó é vermelho ou preto, e a raiz permanece preta
- Onde não há filho, a posição conta como folha preta
- Um nó vermelho só pode ter filhos pretos
- Todo caminho da raiz até a folha tem a mesma quantidade de nós pretos
- Juntas, as regras limitam o caminho mais longo a no máximo o dobro do mais curto

---

# Como a Rubro-Negra se corrige

O número novo entra vermelho. Se o pai também é vermelho, duas cores iguais seguidas quebram a regra, e `inserirFixup` desfaz o conflito.

- O tio é o outro filho do avô. Com tio vermelho, o programa só troca a cor do pai, do tio e do avô
- Com tio preto, inclusive quando o tio não existe, ele gira uma ou duas vezes e depois recolore
- Na remoção, `removerFixup` trata o irmão do nó que saiu. O desenho segue o CLRS
- No fim da inserção a raiz é pintada de preto, mesmo tendo nascido vermelha

---

# Árvore Rubro-Negra: exemplo de ajuste

A mesma sequência 10, 20 e 30. Na saída, `R` é vermelho e `B` é preto. O desenho usa a mesma convenção: direita acima, esquerda abaixo.

```text
Apos 20:
    20(R)
10(B)

Apos 30:
    30(R)
20(B)
    10(R)
```

Depois do 20 a árvore ainda vale: raiz preta e um filho vermelho. O 30 entra vermelho debaixo de outro vermelho, e o tio não existe, então conta como preto. O ajuste gira e recolore. A raiz passa a ser 20, preta, com 10 e 30 vermelhos. Medimos **1** rotação e **3** recolorações.

---

# Demonstração do sistema

No menu `./bin/logistica` o operador escolhe a árvore e cadastra duas encomendas. Esta sessão mostra o programa em uso. Ela não faz parte da suíte automática.

| Passo | AVL | Rubro-Negra |
|-------|-----|-------------|
| Inserir | 1001 e 1002 → Inserido | 2001 e 2002 → Inserido |
| Busca existente | 1002 → Encontrado | 2002 → Encontrado |
| Busca ausente | 9999 → Nao encontrado | 1999 → Nao encontrado |
| Remover | 1001 → Removido | 2002 → Removido |
| Estado final | só `1002`, altura 0, 0 rotações | `2001(B)`, altura 0, 0 rotações, 1 recoloração |

O texto `Nao encontrado` é a mensagem do programa. A recoloração da Rubro-Negra é a raiz, que nasce vermelha e termina preta.

---

<!-- _class: compact -->

# Testes realizados

`make testes` repete a mesma bateria na AVL e na Rubro-Negra. A última linha foi `Todos os testes passaram.`

| O que ficou garantido | AVL | RB |
|-----------------------|-----|----|
| Inserir em ordem, na ordem inversa e misturado | ✅ | ✅ |
| Achar um número presente e falhar em um ausente | ✅ | ✅ |
| Remover folha, nó com um filho e nó com dois filhos | ✅ | ✅ |
| Remover ausente; vazia (altura $-1$) e um nó (altura $0$) | ✅ | ✅ |
| Recusar duplicata e aguentar 10 mil operações | ✅ | ✅ |

As 10 mil operações são conferidas contra um conjunto de referência, com os invariantes checados a cada passo. No teste de remoção, 20, 50 e 70 são folha, nó com dois filhos e nó com um filho. Um caso extra da AVL, filho com $fb = 0$, também passou.

---

# Como ler a comparação

`./bin/experimento` aplica a mesma sequência nas duas árvores e grava altura, rotações e recolorações.

- **Altura:** depois da carga, quantos saltos a busca mais funda precisa dar
- **Rotação:** quantas vezes os ponteiros foram reorganizados. Um giro duplo conta 2
- **Recoloração:** só na Rubro-Negra. Troca a cor do nó e não entra na conta de rotações
- **$n$:** tamanho da sequência. No sorteio aleatório, número repetido é ignorado, então a árvore pode ter menos nós do que $n$
- A ordem decrescente produziu os mesmos valores da crescente, nos três tamanhos

---

<!-- _class: compact -->

# Análise comparativa

| Cenário | n | Alt AVL | Alt RB | Rot AVL | Rot RB | Rec RB |
|---------|--:|--------:|-------:|--------:|-------:|-------:|
| crescente | 100 | 6 | 10 | 93 | 89 | 445 |
| crescente | 1000 | 9 | 16 | 990 | 983 | 4909 |
| crescente | 10000 | 13 | 23 | 9986 | 9976 | 49877 |
| aleatoria | 10000 | 15 | 15 | 6591 | 5500 | 22021 |
| insercao_e_remocao | 5000 | 12 | 13 | 4385 | 3849 | 15402 |

Em ordem, com 10000 chaves, a AVL fica na altura 13 (9986 rotações) e a Rubro-Negra na 23 (9976). No aleatório desse tamanho as duas ficam na altura 15; a Rubro-Negra faz 5500 rotações e a AVL 6591, além de 22021 recolorações. A linha `insercao_e_remocao` insere as 5000 chaves e depois remove a primeira metade. No aleatório menor, n=100: AVL altura 7 e 70 rotações; RB altura 8, 55 rotações e 225 recolorações. Em n=1000: AVL altura 11 e 606 rotações; RB altura 11, 535 rotações e 2209 recolorações.

---

<!-- _class: compact -->

# O que a teoria previa

Os tetos usuais usam o $n$ do CSV. O programa mede a altura; ele não imprime esses limites.

$$
h_{\mathrm{AVL}} \approx 1{,}44 \cdot \log_2 n
\qquad\qquad
h_{\mathrm{RB}} \le 2 \cdot \log_2(n+1)
$$

| n crescente | Teto AVL | Altura AVL | Teto RB | Altura RB |
|------------:|---------:|-----------:|--------:|----------:|
| 100 | 9,57 | 6 | 13,32 | 10 |
| 1000 | 14,35 | 9 | 19,93 | 16 |
| 10000 | 19,13 | 13 | 26,58 | 23 |

Com 10000 chaves em ordem, a AVL mediu 13, abaixo do teto 19,13. A Rubro-Negra mediu 23, abaixo de 26,58 e perto do máximo que a regra dela permite.

---

<!-- _class: compact -->

# Vantagens, limitações e aplicações

A busca acompanha a altura. A atualização acompanha as rotações e, na Rubro-Negra, as recolorações. A tabela usa o aleatório e o crescente de $n = 10000$.

| | AVL | Rubro-Negra |
|--|-----|-------------|
| Altura | Mais baixa no fluxo ordenado: 13 contra 23 | Sobe mais quando o identificador chega em ordem |
| Atualização | 6591 rotações no aleatório | 5500 rotações e 22021 recolorações no mesmo caso |
| Muita leitura | A busca percorre menos saltos no caso ordenado | A altura extra aparece quando a chave vem em ordem |
| Muita escrita | Gira mais quando a chave é aleatória | É o desenho usado em `std::map` e TreeMap |

O par de rotações vem do CSV. O uso em `std::map` é a prática comum de biblioteca, não uma medida deste repositório.

---

# Recomendação técnica

Para este cadastro de encomendas, a estrutura escolhida é a **AVL**.

A busca é frequente, e o caso que separa as duas árvores é a chegada em ordem, o mesmo caso em que a BST vira lista. Com 10000 chaves, a AVL ficou na altura 13 e a Rubro-Negra na 23. As rotações foram 9986 e 9976, e a Rubro-Negra ainda trocou de cor 49877 vezes.

No ritmo misto, inserção seguida de remoção, as alturas ficaram em 12 e 13. A Rubro-Negra fez 3849 rotações e a AVL 4385. No aleatório de 10000 chaves as alturas empataram em 15, e aí a Rubro-Negra girou menos (5500 contra 6591).

A economia de giros da Rubro-Negra aparece com chave aleatória. A folga de altura da AVL aparece quando o identificador chega em ordem, que é quando o rastreio mais precisa da árvore baixa.

---

# Referências e repositório

- Cormen, Leiserson, Rivest e Stein. *Introduction to Algorithms* (CLRS). A correção da Rubro-Negra segue esse modelo, anotado em docs/decisoes.md
- Código, testes e medições: https://github.com/manoelcn/arvores-balanceadas-edb2
- Autor: **Manoel Cândido Neto**
