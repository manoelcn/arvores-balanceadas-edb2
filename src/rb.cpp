// Árvore Rubro-Negra

#include "rb.hpp"

#include <algorithm>
#include <functional>
#include <limits>
#include <vector>

const char* RubroNegra::corStr(Cor c) {
    return c == Cor::Vermelho ? "R" : "B";
}

RubroNegra::RubroNegra() : raiz_(nullptr), rotacoes_(0), recoloracoes_(0) {}

RubroNegra::~RubroNegra() { liberar(raiz_); }

RubroNegra::No* RubroNegra::criarNo(int k) {
    No* n = new No{};
    n->chave = k;
    n->cor = Cor::Vermelho;
    n->esq = n->dir = n->pai = nullptr;
    return n;
}

void RubroNegra::recolorir(No* no, Cor c) {
    if (!no || no->cor == c) return;
    no->cor = c;
    ++recoloracoes_;
}

void RubroNegra::rotacaoEsquerda(No* x) {
    ++rotacoes_;
    No* y = x->dir;
    x->dir = y->esq;
    if (y->esq != nullptr) y->esq->pai = x;
    y->pai = x->pai;
    if (x->pai == nullptr) {
        raiz_ = y;
    } else if (x == x->pai->esq) {
        x->pai->esq = y;
    } else {
        x->pai->dir = y;
    }
    y->esq = x;
    x->pai = y;
}

void RubroNegra::rotacaoDireita(No* x) {
    ++rotacoes_;
    No* y = x->esq;
    x->esq = y->dir;
    if (y->dir != nullptr) y->dir->pai = x;
    y->pai = x->pai;
    if (x->pai == nullptr) {
        raiz_ = y;
    } else if (x == x->pai->esq) {
        x->pai->esq = y;
    } else {
        x->pai->dir = y;
    }
    y->dir = x;
    x->pai = y;
}

void RubroNegra::inserirFixup(No* z) {
    while (z->pai != nullptr && z->pai->cor == Cor::Vermelho) {
        if (z->pai == z->pai->pai->esq) {
            No* y = z->pai->pai->dir;
            if (ehVermelho(y)) {
                // Tio vermelho: só recolorir pai, tio e avô.
                recolorir(z->pai, Cor::Preto);
                recolorir(y, Cor::Preto);
                recolorir(z->pai->pai, Cor::Vermelho);
                z = z->pai->pai;
            } else {
                // Tio preto: rotação simples ou dupla e recoloração.
                if (z == z->pai->dir) {
                    z = z->pai;
                    rotacaoEsquerda(z);
                }
                recolorir(z->pai, Cor::Preto);
                recolorir(z->pai->pai, Cor::Vermelho);
                rotacaoDireita(z->pai->pai);
            }
        } else {
            No* y = z->pai->pai->esq;
            if (ehVermelho(y)) {
                recolorir(z->pai, Cor::Preto);
                recolorir(y, Cor::Preto);
                recolorir(z->pai->pai, Cor::Vermelho);
                z = z->pai->pai;
            } else {
                if (z == z->pai->esq) {
                    z = z->pai;
                    rotacaoDireita(z);
                }
                recolorir(z->pai, Cor::Preto);
                recolorir(z->pai->pai, Cor::Vermelho);
                rotacaoEsquerda(z->pai->pai);
            }
        }
    }
    recolorir(raiz_, Cor::Preto);
}

bool RubroNegra::inserir(int k) {
    if (buscar(k)) return false;

    No* z = criarNo(k);
    No* y = nullptr;
    No* x = raiz_;

    while (x != nullptr) {
        y = x;
        if (k < x->chave) {
            x = x->esq;
        } else {
            x = x->dir;
        }
    }

    z->pai = y;
    if (y == nullptr) {
        raiz_ = z;
    } else if (k < y->chave) {
        y->esq = z;
    } else {
        y->dir = z;
    }

    inserirFixup(z);
    return true;
}

RubroNegra::No* RubroNegra::buscarNo(No* no, int k) const {
    while (no != nullptr) {
        if (k == no->chave) return no;
        no = (k < no->chave) ? no->esq : no->dir;
    }
    return nullptr;
}

bool RubroNegra::buscar(int k) const { return buscarNo(raiz_, k) != nullptr; }

RubroNegra::No* RubroNegra::minimo(No* no) const {
    while (no->esq != nullptr) no = no->esq;
    return no;
}

void RubroNegra::transplantar(No* u, No* v) {
    if (u->pai == nullptr) {
        raiz_ = v;
    } else if (u == u->pai->esq) {
        u->pai->esq = v;
    } else {
        u->pai->dir = v;
    }
    if (v != nullptr) v->pai = u->pai;
}

void RubroNegra::removerFixup(No* x) {
    while (x != raiz_ && ehPreto(x)) {
        if (x->pai == nullptr) break;

        if (x == x->pai->esq) {
            No* w = x->pai->dir;
            if (ehVermelho(w)) {
                recolorir(w, Cor::Preto);
                recolorir(x->pai, Cor::Vermelho);
                rotacaoEsquerda(x->pai);
                w = x->pai->dir;
            }
            if (ehPreto(w ? w->esq : nullptr) && ehPreto(w ? w->dir : nullptr)) {
                if (w != nullptr) recolorir(w, Cor::Vermelho);
                x = x->pai;
            } else {
                if (w != nullptr && ehPreto(w->dir)) {
                    recolorir(w->esq, Cor::Preto);
                    recolorir(w, Cor::Vermelho);
                    rotacaoDireita(w);
                    w = x->pai->dir;
                }
                if (w != nullptr) recolorir(w, x->pai->cor);
                recolorir(x->pai, Cor::Preto);
                if (w != nullptr) recolorir(w->dir, Cor::Preto);
                rotacaoEsquerda(x->pai);
                x = raiz_;
            }
        } else {
            No* w = x->pai->esq;
            if (ehVermelho(w)) {
                recolorir(w, Cor::Preto);
                recolorir(x->pai, Cor::Vermelho);
                rotacaoDireita(x->pai);
                w = x->pai->esq;
            }
            if (ehPreto(w ? w->dir : nullptr) && ehPreto(w ? w->esq : nullptr)) {
                if (w != nullptr) recolorir(w, Cor::Vermelho);
                x = x->pai;
            } else {
                if (w != nullptr && ehPreto(w->esq)) {
                    recolorir(w->dir, Cor::Preto);
                    recolorir(w, Cor::Vermelho);
                    rotacaoEsquerda(w);
                    w = x->pai->esq;
                }
                if (w != nullptr) recolorir(w, x->pai->cor);
                recolorir(x->pai, Cor::Preto);
                if (w != nullptr) recolorir(w->esq, Cor::Preto);
                rotacaoDireita(x->pai);
                x = raiz_;
            }
        }
    }
    if (x != nullptr) recolorir(x, Cor::Preto);
}

bool RubroNegra::remover(int k) {
    No* z = buscarNo(raiz_, k);
    if (z == nullptr) return false;

    No* y = z;
    Cor yCorOriginal = y->cor;
    No* x = nullptr;
    No* xPai = nullptr;
    bool xEhEsquerda = false;
    No folhaTemp{};
    folhaTemp.cor = Cor::Preto;
    folhaTemp.esq = folhaTemp.dir = nullptr;
    folhaTemp.chave = 0;
    bool usouFolhaTemp = false;

    if (z->esq == nullptr) {
        x = z->dir;
        xPai = z->pai;
        xEhEsquerda = (z->pai != nullptr && z == z->pai->esq);
        transplantar(z, z->dir);
    } else if (z->dir == nullptr) {
        x = z->esq;
        xPai = z->pai;
        xEhEsquerda = (z->pai != nullptr && z == z->pai->esq);
        transplantar(z, z->esq);
    } else {
        y = minimo(z->dir);
        yCorOriginal = y->cor;
        x = y->dir;
        if (y->pai == z) {
            xPai = y;
            xEhEsquerda = false;
            if (x != nullptr) x->pai = y;
        } else {
            xPai = y->pai;
            xEhEsquerda = (y->pai != nullptr && y == y->pai->esq);
            transplantar(y, y->dir);
            y->dir = z->dir;
            if (y->dir != nullptr) y->dir->pai = y;
        }
        transplantar(z, y);
        y->esq = z->esq;
        if (y->esq != nullptr) y->esq->pai = y;
        y->cor = z->cor;
    }

    delete z;

    if (yCorOriginal == Cor::Preto) {
        if (x == nullptr) {
            usouFolhaTemp = true;
            folhaTemp.pai = xPai;
            x = &folhaTemp;
            if (xPai != nullptr) {
                if (xEhEsquerda)
                    xPai->esq = &folhaTemp;
                else
                    xPai->dir = &folhaTemp;
            }
        }
        removerFixup(x);
    }

    if (usouFolhaTemp && folhaTemp.pai != nullptr) {
        if (folhaTemp.pai->esq == &folhaTemp) folhaTemp.pai->esq = nullptr;
        if (folhaTemp.pai->dir == &folhaTemp) folhaTemp.pai->dir = nullptr;
        folhaTemp.pai = nullptr;
    }
    return true;
}

void RubroNegra::emOrdemRec(No* no, std::ostream& out) const {
    if (no == nullptr) return;
    emOrdemRec(no->esq, out);
    out << no->chave << ' ';
    emOrdemRec(no->dir, out);
}

void RubroNegra::emOrdem(std::ostream& out) const {
    emOrdemRec(raiz_, out);
    out << '\n';
}

void RubroNegra::imprimirRec(No* no, int nivel, std::ostream& out) const {
    if (no == nullptr) return;
    imprimirRec(no->dir, nivel + 1, out);
    for (int i = 0; i < nivel; ++i) out << "    ";
    out << no->chave << '(' << corStr(no->cor) << ")\n";
    imprimirRec(no->esq, nivel + 1, out);
}

void RubroNegra::imprimir(std::ostream& out) const {
    if (raiz_ == nullptr) {
        out << "(vazia)\n";
        return;
    }
    imprimirRec(raiz_, 0, out);
}

int RubroNegra::altura() const {
    std::function<int(No*)> h = [&](No* n) -> int {
        if (n == nullptr) return -1;
        return 1 + std::max(h(n->esq), h(n->dir));
    };
    return h(raiz_);
}

void RubroNegra::liberar(No* no) {
    if (no == nullptr) return;
    liberar(no->esq);
    liberar(no->dir);
    delete no;
}

bool RubroNegra::verificarOrdem(No* no, int minVal, int maxVal) const {
    if (no == nullptr) return true;
    if (no->chave <= minVal || no->chave >= maxVal) return false;
    return verificarOrdem(no->esq, minVal, no->chave) &&
           verificarOrdem(no->dir, no->chave, maxVal);
}

bool RubroNegra::verificarVermelho(No* no) const {
    if (no == nullptr) return true;
    if (no->cor == Cor::Vermelho) {
        if (ehVermelho(no->esq) || ehVermelho(no->dir)) return false;
    }
    return verificarVermelho(no->esq) && verificarVermelho(no->dir);
}

void RubroNegra::coletarPretos(No* no, int contagem, std::vector<int>& vals) const {
    if (no == nullptr) {
        vals.push_back(contagem);
        return;
    }
    int prox = contagem + (no->cor == Cor::Preto ? 1 : 0);
    coletarPretos(no->esq, prox, vals);
    coletarPretos(no->dir, prox, vals);
}

bool RubroNegra::pretosIguaisCaminhos() const {
    std::vector<int> vals;
    coletarPretos(raiz_, 0, vals);
    if (vals.empty()) return true;
    for (int v : vals) {
        if (v != vals[0]) return false;
    }
    return true;
}

bool RubroNegra::verificarInvariantes() const {
    if (raiz_ != nullptr && raiz_->cor != Cor::Preto) return false;
    if (!verificarOrdem(raiz_, std::numeric_limits<int>::min(), std::numeric_limits<int>::max()))
        return false;
    if (!verificarVermelho(raiz_)) return false;
    return pretosIguaisCaminhos();
}
