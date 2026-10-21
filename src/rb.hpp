// Árvore Rubro-Negra

#ifndef RB_HPP
#define RB_HPP

#include <iostream>
#include <vector>

// Rubro-Negra (estilo CLRS). nullptr = folha preta.
// Rotação dupla = 2 rotações simples. Recoloração: cada nó que muda de cor conta 1.
class RubroNegra {
public:
    RubroNegra();
    ~RubroNegra();

    RubroNegra(const RubroNegra&) = delete;
    RubroNegra& operator=(const RubroNegra&) = delete;

    bool inserir(int k);
    bool remover(int k);
    bool buscar(int k) const;

    void emOrdem(std::ostream& out = std::cout) const;
    void imprimir(std::ostream& out = std::cout) const;

    int altura() const;
    long long rotacoes() const { return rotacoes_; }
    long long recoloracoes() const { return recoloracoes_; }
    void zerarContadores() { rotacoes_ = 0; recoloracoes_ = 0; }

    bool verificarInvariantes() const;
    bool vazia() const { return raiz_ == nullptr; }

private:
    enum class Cor { Vermelho, Preto };

    struct No {
        int chave;
        Cor cor;
        No* esq;
        No* dir;
        No* pai;
    };

    No* raiz_;
    long long rotacoes_;
    long long recoloracoes_;

    static bool ehPreto(const No* n) { return n == nullptr || n->cor == Cor::Preto; }
    static bool ehVermelho(const No* n) { return n != nullptr && n->cor == Cor::Vermelho; }

    No* criarNo(int k);
    void liberar(No* no);

    void rotacaoEsquerda(No* x);
    void rotacaoDireita(No* x);

    void inserirFixup(No* z);
    void removerFixup(No* x);

    No* buscarNo(No* no, int k) const;
    No* minimo(No* no) const;
    void transplantar(No* u, No* v);

    void emOrdemRec(No* no, std::ostream& out) const;
    void imprimirRec(No* no, int nivel, std::ostream& out) const;

    static const char* corStr(Cor c);
    void recolorir(No* no, Cor c);

    bool verificarOrdem(No* no, int minVal, int maxVal) const;
    bool verificarVermelho(No* no) const;
    void coletarPretos(No* no, int contagem, std::vector<int>& vals) const;
    bool pretosIguaisCaminhos() const;
};

#endif
