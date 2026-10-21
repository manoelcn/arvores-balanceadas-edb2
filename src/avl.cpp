// Árvore AVL


#include "avl.hpp"

#include <algorithm>
#include <cmath>
#include <limits>

AVL::No::No(int k) : chave(k), esq(nullptr), dir(nullptr), alt(0) {}

AVL::AVL() : raiz_(nullptr), rotacoes_(0) {}

AVL::~AVL() { liberar(raiz_); }

int AVL::alturaNo(const No* n) { return n ? n->alt : -1; }

int AVL::fatorBalanceamento(const No* n) {
    if (!n) return 0;
    return alturaNo(n->esq) - alturaNo(n->dir);
}

AVL::No* AVL::rotacaoDireita(No* y) {
    ++rotacoes_;
    No* x = y->esq;
    No* t2 = x->dir;

    x->dir = y;
    y->esq = t2;

    y->alt = 1 + std::max(alturaNo(y->esq), alturaNo(y->dir));
    x->alt = 1 + std::max(alturaNo(x->esq), alturaNo(x->dir));

    return x;
}

AVL::No* AVL::rotacaoEsquerda(No* x) {
    ++rotacoes_;
    No* y = x->dir;
    No* t2 = y->esq;

    y->esq = x;
    x->dir = t2;

    x->alt = 1 + std::max(alturaNo(x->esq), alturaNo(x->dir));
    y->alt = 1 + std::max(alturaNo(y->esq), alturaNo(y->dir));

    return y;
}

AVL::No* AVL::rebalancear(No* no) {
    if (!no) return no;

    no->alt = 1 + std::max(alturaNo(no->esq), alturaNo(no->dir));
    int fb = fatorBalanceamento(no);

    // Esquerda pesada: LL (rotação à direita) ou LR (esquerda no filho, depois direita).
    if (fb > 1) {
        if (fatorBalanceamento(no->esq) < 0) {
            no->esq = rotacaoEsquerda(no->esq);
        }
        return rotacaoDireita(no);
    }
    // Direita pesada: RR (rotação à esquerda) ou RL (direita no filho, depois esquerda).
    if (fb < -1) {
        if (fatorBalanceamento(no->dir) > 0) {
            no->dir = rotacaoDireita(no->dir);
        }
        return rotacaoEsquerda(no);
    }
    return no;
}

AVL::No* AVL::inserirRec(No* no, int k, bool& inserido) {
    if (!no) {
        inserido = true;
        return new No(k);
    }

    if (k < no->chave) {
        no->esq = inserirRec(no->esq, k, inserido);
    } else if (k > no->chave) {
        no->dir = inserirRec(no->dir, k, inserido);
    } else {
        inserido = false;
        return no;
    }

    if (!inserido) return no;

    no->alt = 1 + std::max(alturaNo(no->esq), alturaNo(no->dir));
    return rebalancear(no);
}

bool AVL::inserir(int k) {
    bool inserido = false;
    raiz_ = inserirRec(raiz_, k, inserido);
    return inserido;
}

AVL::No* AVL::minimo(No* no) const {
    while (no && no->esq) no = no->esq;
    return no;
}

AVL::No* AVL::removerRec(No* no, int k, bool& removido, bool& alturaDiminuiu) {
    if (!no) {
        removido = false;
        alturaDiminuiu = false;
        return nullptr;
    }

    if (k < no->chave) {
        no->esq = removerRec(no->esq, k, removido, alturaDiminuiu);
        if (removido && alturaDiminuiu) {
            int hAntes = no->alt;
            no = rebalancear(no);
            alturaDiminuiu = (no->alt < hAntes);
        }
        return no;
    }
    if (k > no->chave) {
        no->dir = removerRec(no->dir, k, removido, alturaDiminuiu);
        if (removido && alturaDiminuiu) {
            int hAntes = no->alt;
            no = rebalancear(no);
            alturaDiminuiu = (no->alt < hAntes);
        }
        return no;
    }

    removido = true;

    if (!no->esq || !no->dir) {
        No* filho = no->esq ? no->esq : no->dir;
        delete no;
        alturaDiminuiu = true;
        return filho;
    }

    No* sucessor = minimo(no->dir);
    no->chave = sucessor->chave;
    bool subDiminuiu = false;
    no->dir = removerRec(no->dir, sucessor->chave, removido, subDiminuiu);
    if (subDiminuiu) {
        int hAntes = no->alt;
        no = rebalancear(no);
        alturaDiminuiu = (no->alt < hAntes);
    } else {
        alturaDiminuiu = false;
    }
    return no;
}

bool AVL::remover(int k) {
    bool removido = false;
    bool alturaDiminuiu = false;
    raiz_ = removerRec(raiz_, k, removido, alturaDiminuiu);
    return removido;
}

bool AVL::buscar(int k) const {
    const No* cur = raiz_;
    while (cur) {
        if (k == cur->chave) return true;
        cur = (k < cur->chave) ? cur->esq : cur->dir;
    }
    return false;
}

void AVL::emOrdemRec(const No* no, std::ostream& out) const {
    if (!no) return;
    emOrdemRec(no->esq, out);
    out << no->chave << ' ';
    emOrdemRec(no->dir, out);
}

void AVL::emOrdem(std::ostream& out) const {
    emOrdemRec(raiz_, out);
    out << '\n';
}

void AVL::imprimirRec(const No* no, int nivel, std::ostream& out) const {
    if (!no) return;
    imprimirRec(no->dir, nivel + 1, out);
    for (int i = 0; i < nivel; ++i) out << "    ";
    out << no->chave << "(fb=" << fatorBalanceamento(no) << ")\n";
    imprimirRec(no->esq, nivel + 1, out);
}

void AVL::imprimir(std::ostream& out) const {
    if (!raiz_) {
        out << "(vazia)\n";
        return;
    }
    imprimirRec(raiz_, 0, out);
}

int AVL::altura() const { return alturaNo(raiz_); }

void AVL::liberar(No* no) {
    if (!no) return;
    liberar(no->esq);
    liberar(no->dir);
    delete no;
}

bool AVL::verificarRec(const No* no, int minVal, int maxVal) const {
    if (!no) return true;
    if (no->chave <= minVal || no->chave >= maxVal) return false;
    if (std::abs(fatorBalanceamento(no)) > 1) return false;
    int h = alturaNo(no);
    int hCalc = 1 + std::max(alturaNo(no->esq), alturaNo(no->dir));
    if (h != hCalc) return false;
    return verificarRec(no->esq, minVal, no->chave) &&
           verificarRec(no->dir, no->chave, maxVal);
}

bool AVL::verificarInvariantes() const {
    return verificarRec(raiz_, std::numeric_limits<int>::min(), std::numeric_limits<int>::max());
}
