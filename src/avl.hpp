// Árvore AVL

#ifndef AVL_HPP
#define AVL_HPP

#include <iostream>
#include <string>
#include <vector>

// Árvore AVL para chaves inteiras (identificadores de encomendas).
// Altura de subárvore vazia: -1. Rotação dupla conta como 2 rotações simples.
class AVL {
public:
    AVL();
    ~AVL();

    AVL(const AVL&) = delete;
    AVL& operator=(const AVL&) = delete;

    bool inserir(int k);
    bool remover(int k);
    bool buscar(int k) const;

    void emOrdem(std::ostream& out = std::cout) const;
    void imprimir(std::ostream& out = std::cout) const;

    int altura() const;
    long long rotacoes() const { return rotacoes_; }
    void zerarContadores() { rotacoes_ = 0; }

    bool verificarInvariantes() const;
    bool vazia() const { return raiz_ == nullptr; }

private:
    struct No {
        int chave;
        No* esq;
        No* dir;
        int alt; // altura do nó (folha = 0)
        explicit No(int k);
    };

    No* raiz_;
    long long rotacoes_;

    static int alturaNo(const No* n);
    static int fatorBalanceamento(const No* n);

    No* rotacaoDireita(No* y);
    No* rotacaoEsquerda(No* x);

    No* inserirRec(No* no, int k, bool& inserido);
    No* removerRec(No* no, int k, bool& removido, bool& alturaDiminuiu);
    No* minimo(No* no) const;
    No* rebalancear(No* no);

    void emOrdemRec(const No* no, std::ostream& out) const;
    void imprimirRec(const No* no, int nivel, std::ostream& out) const;
    void liberar(No* no);

    bool verificarRec(const No* no, int minVal, int maxVal) const;
};

#endif
