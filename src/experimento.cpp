// Comparação AVL x Rubro-Negra

#include "avl.hpp"
#include "rb.hpp"

#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <random>
#include <sstream>
#include <string>
#include <vector>

namespace {

struct BstSimples {
    struct No {
        int k;
        No* e;
        No* d;
    };
    No* raiz = nullptr;

    ~BstSimples() { liberar(raiz); }

    void liberar(No* n) {
        if (!n) return;
        liberar(n->e);
        liberar(n->d);
        delete n;
    }

    void inserir(int k) {
        raiz = ins(raiz, k);
    }

    No* ins(No* n, int k) {
        if (!n) return new No{k, nullptr, nullptr};
        if (k < n->k) n->e = ins(n->e, k);
        else if (k > n->k) n->d = ins(n->d, k);
        return n;
    }

    int altura(No* n) const {
        if (!n) return -1;
        return 1 + std::max(altura(n->e), altura(n->d));
    }

    int altura() const { return altura(raiz); }
};

std::vector<int> crescente(int n) {
    std::vector<int> v(n);
    for (int i = 0; i < n; ++i) v[i] = i + 1;
    return v;
}

std::vector<int> decrescente(int n) {
    std::vector<int> v(n);
    for (int i = 0; i < n; ++i) v[i] = n - i;
    return v;
}

std::vector<int> aleatoria(int n, unsigned seed) {
    std::vector<int> v(n);
    std::mt19937 rng(seed);
    std::uniform_int_distribution<int> dist(1, n * 10);
    for (int i = 0; i < n; ++i) v[i] = dist(rng);
    return v;
}

template <typename Arvore>
void aplicarInsercoes(Arvore& a, const std::vector<int>& v) {
    for (int k : v) a.inserir(k);
}

template <typename Arvore>
void linhaCsv(const char* estrutura, const char* cenario, int n, const Arvore& a,
              long long recol = 0) {
    std::cout << estrutura << ',' << cenario << ',' << n << ',' << a.altura() << ','
              << a.rotacoes() << ',' << recol << '\n';
}

void rodarCsv() {
    std::cout << "estrutura,cenario,n,altura,rotacoes,recoloracoes\n";

    const int tamanhos[] = {100, 1000, 10000};
    const char* nomes[] = {"crescente", "decrescente", "aleatoria"};

    for (int n : tamanhos) {
        for (int c = 0; c < 3; ++c) {
            std::vector<int> v;
            if (c == 0) v = crescente(n);
            else if (c == 1) v = decrescente(n);
            else v = aleatoria(n, static_cast<unsigned>(n + 7));

            AVL avl;
            avl.zerarContadores();
            aplicarInsercoes(avl, v);
            linhaCsv("AVL", nomes[c], n, avl, 0);

            RubroNegra rb;
            rb.zerarContadores();
            aplicarInsercoes(rb, v);
            linhaCsv("RB", nomes[c], n, rb, rb.recoloracoes());

            if (c == 0) {
                BstSimples bst;
                for (int k : v) bst.inserir(k);
                std::cout << "BST,crescente," << n << ',' << bst.altura() << ",0,0\n";
            }
        }
    }

    int n = 5000;
    auto v = aleatoria(n, 99);
    AVL avl;
    RubroNegra rb;
    aplicarInsercoes(avl, v);
    aplicarInsercoes(rb, v);
    for (int i = 0; i < n / 2; ++i) {
        avl.remover(v[i]);
        rb.remover(v[i]);
    }
    linhaCsv("AVL", "insercao_e_remocao", n, avl, 0);
    linhaCsv("RB", "insercao_e_remocao", n, rb, rb.recoloracoes());
}

void rodarExemplos() {
    std::ostringstream out;

    out << "=== Exemplo AVL: inserir 10, 20, 30 ===\n";
    AVL avl;
    avl.inserir(10);
    out << "Apos 10:\n";
    avl.imprimir(out);
    avl.inserir(20);
    out << "Apos 20:\n";
    avl.imprimir(out);
    avl.inserir(30);
    out << "Apos 30 (rotacao):\n";
    avl.imprimir(out);
    out << "Rotacoes: " << avl.rotacoes() << "\n\n";

    out << "=== Exemplo RB: inserir 10, 20, 30 ===\n";
    RubroNegra rb;
    rb.inserir(10);
    out << "Apos 10:\n";
    rb.imprimir(out);
    rb.inserir(20);
    out << "Apos 20:\n";
    rb.imprimir(out);
    rb.inserir(30);
    out << "Apos 30:\n";
    rb.imprimir(out);
    out << "Rotacoes: " << rb.rotacoes() << " Recoloracoes: " << rb.recoloracoes() << "\n\n";

    out << "=== Demonstracao menu (simulada) ===\n";
    AVL demo;
    demo.inserir(1001);
    demo.inserir(1002);
    out << "AVL inserir 1001, 1002; buscar 1002: " << (demo.buscar(1002) ? "sim" : "nao")
        << "; remover 1001; altura final: " << demo.altura() << "\n";
    demo.remover(1001);

    RubroNegra demoRb;
    demoRb.inserir(2001);
    demoRb.inserir(2002);
    out << "RB inserir 2001, 2002; buscar 1999: " << (demoRb.buscar(1999) ? "sim" : "nao")
        << "; remover 2002; altura final: " << demoRb.altura() << "\n";
    demoRb.remover(2002);

    std::cout << out.str();
}

} // namespace

int main(int argc, char** argv) {
    bool exemplos = false;
    for (int i = 1; i < argc; ++i) {
        if (std::string(argv[i]) == "--exemplos") exemplos = true;
    }
    if (exemplos)
        rodarExemplos();
    else
        rodarCsv();
    return 0;
}
