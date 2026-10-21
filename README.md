# Árvores Balanceadas - AVL e Rubro-Negra

**Autor:** Manoel Cândido Neto  
**Disciplina:** Estruturas de Dados Básicas II

Protótipo de logística: registra, busca e remove identificadores de encomendas (inteiros) usando árvore **AVL** ou **Rubro-Negra**, com testes e comparação experimental.

## Funcionalidades

- Menu interativo (`bin/logistica`): inserir, remover, buscar, em ordem, imprimir árvore, altura e contadores de rotações (e recolorações na RB).
- Implementação própria de balanceamento (sem `std::set`/`std::map` no código das árvores).
- Testes automatizados e programa de experimentos (métricas AVL × RB × BST).

## Dependências

Nenhuma biblioteca externa. Compilador **C++17** (`g++`) e `make`.

## Como executar o projeto

1. Entre na raiz do repositório (onde está o `Makefile`).
2. Compile:

   ```bash
   make
   ```

   Gera `bin/logistica`, `bin/testes` e `bin/experimento`. O Makefile cria `bin/` e `docs/resultados/` se ainda não existirem.

3. Use o protótipo interativo:

   ```bash
   make run
   ```

   Equivalente a compilar `logistica` e rodar `./bin/logistica`.

4. Na primeira tela, escolha **1 = AVL** ou **2 = Rubro-Negra**. O menu oferece: **1** inserir, **2** remover, **3** buscar, **4** percurso em ordem, **5** imprimir árvore, **6** altura e contadores, **0** sair.

### Exemplo de uso do menu

```text
$ ./bin/logistica
Escolha a estrutura (1=AVL, 2=Rubro-Negra): 1
Arvore selecionada: AVL

--- Menu ---
1 Inserir encomenda
...
Chave a inserir: 42
```

## Comandos úteis

### Make

| Comando | Descrição |
|---------|-----------|
| `make` ou `make all` | Compila os três programas em `bin/` (não executa testes nem experimento). |
| `make run` | Compila `logistica` (se precisar) e inicia o menu interativo. |
| `make logistica` | Compila apenas `bin/logistica`. |
| `make testes` | Compila `bin/testes` (se precisar) e executa a suíte no terminal. |
| `make experimento` | Compila `bin/experimento` (se precisar) e imprime o CSV no terminal. |
| `make resultados` | Executa testes e experimentos e grava arquivos em `docs/resultados/`. |
| `make clean` | Remove executáveis e `*.o` em `bin/`. |


### Executáveis

```bash
./bin/logistica              # menu interativo
./bin/testes                 # mesma saída que `make testes`
./bin/experimento            # CSV: altura, rotações e recolorações por cenário
./bin/experimento --exemplos # demonstração textual (inserções passo a passo)
```

O CSV cobre inserções em ordem crescente, decrescente e aleatória para `n` em {100, 1000, 10000}, inclui BST só no cenário crescente, e um caso extra de inserção seguida de remoções (`insercao_e_remocao`, n=5000).

### Artefatos de documentação

```bash
make resultados
```

Cria ou atualiza:

- `docs/resultados/testes.log`
- `docs/resultados/comparacao.csv`
- `docs/resultados/exemplos.txt`

## Apresentação

A defesa técnica é o deck em `apresentacao/apresentacao.md`.


## Estrutura do projeto

Estado atual do repositório (código e documentação versionados):

```
.
├── Makefile
├── README.md
├── .gitignore
├── src/
├── tests/
├── docs/
└── apresentacao/
```

Os executáveis `bin/logistica`, `bin/testes` e `bin/experimento` são gerados por `make`. `bin/` é ignorado.

### `src/`

| Arquivo | Função |
|---------|--------|
| `avl.hpp`, `avl.cpp` | Árvore AVL. |
| `rb.hpp`, `rb.cpp` | Árvore rubro-negra. |
| `main.cpp` | Entrada do programa `logistica` (menu). |
| `experimento.cpp` | Entrada de `experimento` (CSV e `--exemplos`). |

As duas árvores expõem operações de inserção, remoção, busca, percursos, impressão, altura, contadores e `verificarInvariantes()`.

### `tests/`

| Arquivo | Função |
|---------|--------|
| `testes.cpp` | Testes para AVL e RB; caso adicional de remoção AVL com fator de balanceamento zero no filho mais alto. |

### `docs/`

| Caminho | Função |
|---------|--------|
| `decisoes.md` | Convenções de implementação (altura, rotações, duplicados, etc.). |
| `resultados/` | Saídas produzidas por `make resultados` (vazio até a primeira execução desse alvo). |

### `apresentacao/`

| Caminho | Função |
|---------|--------|
| `apresentacao.md` | Apresentação técnica em Marp (tema padrão). |

## Documentação de implementação

Ver [docs/decisoes.md](docs/decisoes.md) para convenções de altura, rotações e casos especiais.

---

> **Nota:** Partes do código e da documentação deste repositório foram elaboradas com auxílio de ferramentas de inteligência artificial, sob revisão do autor.
