# Decisões de implementação

**Autor:** Manoel Cândido Neto

**Disciplina:** Estruturas de Dados Básicas II

## Convenções gerais

| Tópico | Decisão |
|--------|---------|
| Altura de subárvore vazia | **-1** (folha com um nó tem altura **0**) |
| Implementação | **Recursiva** (inserção, remoção e percursos) |
| Rotação dupla (AVL e RB) | Conta como **2** rotações simples (cada `rotacaoEsquerda`/`rotacaoDireita` incrementa 1) |
| Recoloração (RB) | Cada nó cuja cor **efetivamente muda** incrementa o contador em 1 |

## Casos especiais (RF / especificação)

### Duplicado na inserção

Rejeitar: `inserir(k)` retorna `false` e a árvore permanece inalterada.

### Buscar ou remover chave inexistente

`buscar` retorna `false`; `remover` retorna `false`. Nenhuma estrutura é alterada. O menu informa o usuário.

### Árvore vazia

Raiz nula (AVL) ou raiz = sentinela NIL (RB). Altura **-1**. Operações de busca/remoção falham; impressão mostra `(vazia)`.

### Árvore com um elemento

Altura **0**. Remoção desse nó deixa a árvore vazia.

### Remoção com dois filhos

Troca a chave com o **sucessor em ordem** (mínimo da subárvore direita) e remove o sucessor (que tem no máximo um filho).

### AVL: filho com fator 0 na remoção

Ao propagar redução de altura, se o rebalanceamento mantém a altura do nó (caso clássico com filho de fb 0), a redução **não** sobe para o ancestral — implementado comparando altura antes e depois do `rebalancear`.

## Rubro-Negra

Modelo **CLRS** com filhos ausentes como `nullptr` (tratados como folha preta). Nó temporário preto na remoção quando o substituto é `nullptr`, para o fix-up distinguir esquerda/direita. Raiz sempre preta após inserção. Remoção com `transplant` e `RB-DELETE-FIXUP`.

## Ferramentas

- Compilação: `Makefile` + `g++ -std=c++17`
- Slides: `apresentacao/apresentacao.md` (Marp)
- `std::set` apenas em `tests/testes.cpp` para conferência
