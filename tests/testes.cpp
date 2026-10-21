// Testes

#include "avl.hpp"
#include "rb.hpp"

#include <cassert>
#include <cstdio>
#include <random>
#include <set>
#include <sstream>
#include <string>
#include <vector>

template <typename Arvore>
void inserirLista(Arvore& a, const std::vector<int>& v) {
    for (int k : v) {
        a.inserir(k);
        assert(a.verificarInvariantes());
    }
}

template <typename Arvore>
void testeInsercoes(const char* nome) {
    {
    Arvore a;
    std::vector<int> crescente;
    for (int i = 1; i <= 50; ++i) crescente.push_back(i);
    inserirLista(a, crescente);
    }

    {
    Arvore a;
    std::vector<int> decrescente;
    for (int i = 50; i >= 1; --i) decrescente.push_back(i);
    inserirLista(a, decrescente);
    }

    {
    Arvore a;
    std::mt19937 rng(42);
    std::uniform_int_distribution<int> dist(1, 500);
    for (int i = 0; i < 200; ++i) {
        int k = dist(rng);
        a.inserir(k);
        assert(a.verificarInvariantes());
    }
    }

    std::printf("  [%s] TST-01 insercoes OK\n", nome);
}

template <typename Arvore>
void testeBusca(const char* nome) {
    Arvore a;
    for (int k : {10, 5, 15, 3, 7}) a.inserir(k);
    assert(a.buscar(7));
    assert(a.buscar(99) == false);
    std::printf("  [%s] TST-02 busca OK\n", nome);
}

template <typename Arvore>
void testeRemocoes(const char* nome) {
    Arvore a;
    for (int k : {50, 30, 70, 20, 40, 60, 80}) a.inserir(k);

    assert(a.remover(20));
    assert(a.verificarInvariantes());
    assert(a.remover(50));
    assert(a.verificarInvariantes());
    assert(a.remover(70));
    assert(a.verificarInvariantes());

    Arvore b;
    b.inserir(42);
    assert(b.remover(42));
    assert(b.vazia());
    assert(b.verificarInvariantes());

    std::printf("  [%s] TST-03 remocoes OK\n", nome);
}

template <typename Arvore>
void testeRemoverInexistente(const char* nome) {
    Arvore a;
    a.inserir(1);
    assert(!a.remover(2));
    assert(a.verificarInvariantes());
    std::printf("  [%s] TST-04 remover inexistente OK\n", nome);
}

template <typename Arvore>
void testeVaziaUmElemento(const char* nome) {
    Arvore a;
    assert(a.vazia());
    assert(a.altura() == -1);
    assert(!a.buscar(1));
    assert(!a.remover(1));

    a.inserir(10);
    assert(!a.vazia());
    assert(a.altura() == 0);
    assert(a.verificarInvariantes());
    std::printf("  [%s] TST-05 vazia/um elemento OK\n", nome);
}

template <typename Arvore>
void testeDuplicados(const char* nome) {
    Arvore a;
    assert(a.inserir(5));
    assert(!a.inserir(5));
    assert(a.verificarInvariantes());
    std::printf("  [%s] TST-06 duplicados OK\n", nome);
}

void testeAvlFbZeroRemocao() {
    AVL a;
    for (int k : {10, 20, 30, 40, 50, 25}) a.inserir(k);
    assert(a.verificarInvariantes());
    assert(a.remover(25));
    assert(a.verificarInvariantes());
    assert(a.remover(20));
    assert(a.verificarInvariantes());
    std::printf("  [AVL] remocao com fb=0 no filho mais alto OK\n");
}

template <typename Arvore>
void testeAleatorio(const char* nome) {
    Arvore a;
    std::set<int> ref;
    std::mt19937 rng(12345);
    std::uniform_int_distribution<int> op(0, 2);
    std::uniform_int_distribution<int> val(1, 10000);

    for (int i = 0; i < 10000; ++i) {
        int escolha = op(rng);
        int k = val(rng);
        if (escolha == 0) {
            bool ok = a.inserir(k);
            if (ref.count(k)) assert(!ok);
            else {
                assert(ok);
                ref.insert(k);
            }
        } else if (escolha == 1) {
            bool ok = a.remover(k);
            if (!ref.count(k)) assert(!ok);
            else {
                assert(ok);
                ref.erase(k);
            }
        } else {
            assert(a.buscar(k) == (ref.count(k) > 0));
        }
        assert(a.verificarInvariantes());
    }
    std::printf("  [%s] TST-08 aleatorio (10000 ops) OK\n", nome);
}

template <typename Arvore>
void rodarSuite(const char* nome) {
    testeInsercoes<Arvore>(nome);
    testeBusca<Arvore>(nome);
    testeRemocoes<Arvore>(nome);
    testeRemoverInexistente<Arvore>(nome);
    testeVaziaUmElemento<Arvore>(nome);
    testeDuplicados<Arvore>(nome);
    std::printf("  [%s] TST-07 invariantes apos cada op (incluido acima)\n", nome);
    testeAleatorio<Arvore>(nome);
}

int main() {
    std::printf("Executando testes...\n");
    rodarSuite<AVL>("AVL");
    testeAvlFbZeroRemocao();
    rodarSuite<RubroNegra>("RB");
    std::printf("Todos os testes passaram.\n");
    return 0;
}
