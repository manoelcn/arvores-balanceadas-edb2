// Sistema de logística (menu)

#include "avl.hpp"
#include "rb.hpp"

#include <iostream>
#include <limits>
#include <string>

namespace {

void limparEntrada() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

bool lerInteiro(const std::string& prompt, int& valor) {
    std::cout << prompt;
    if (!(std::cin >> valor)) {
        limparEntrada();
        std::cout << "Entrada invalida. Tente novamente.\n";
        return false;
    }
    return true;
}

bool lerOpcao(int& opcao) {
    std::cout << "\n--- Menu ---\n"
              << "1 Inserir encomenda\n"
              << "2 Remover encomenda\n"
              << "3 Buscar encomenda\n"
              << "4 Percurso em ordem\n"
              << "5 Imprimir arvore\n"
              << "6 Altura e contadores\n"
              << "0 Sair\n"
              << "Opcao: ";
    if (!(std::cin >> opcao)) {
        limparEntrada();
        std::cout << "Entrada invalida.\n";
        return false;
    }
    return true;
}

void menuAVL() {
    AVL arvore;
    std::cout << "Arvore selecionada: AVL\n";

    for (;;) {
        int op = -1;
        if (!lerOpcao(op)) continue;
        if (op == 0) {
            std::cout << "Encerrando.\n";
            break;
        }

        int k = 0;
        switch (op) {
        case 1:
            if (!lerInteiro("Chave a inserir: ", k)) break;
            std::cout << (arvore.inserir(k) ? "Inserido.\n" : "Duplicado (inalterado).\n");
            break;
        case 2:
            if (!lerInteiro("Chave a remover: ", k)) break;
            std::cout << (arvore.remover(k) ? "Removido.\n" : "Inexistente.\n");
            break;
        case 3:
            if (!lerInteiro("Chave a buscar: ", k)) break;
            std::cout << (arvore.buscar(k) ? "Encontrado.\n" : "Nao encontrado.\n");
            break;
        case 4:
            std::cout << "Em ordem: ";
            arvore.emOrdem();
            break;
        case 5:
            arvore.imprimir();
            break;
        case 6:
            std::cout << "Altura: " << arvore.altura() << "\n";
            std::cout << "Rotacoes: " << arvore.rotacoes() << "\n";
            if (!arvore.verificarInvariantes()) std::cout << "AVISO: invariantes violados.\n";
            break;
        default:
            std::cout << "Opcao invalida.\n";
            break;
        }
    }
}

void menuRubroNegra() {
    RubroNegra arvore;
    std::cout << "Arvore selecionada: Rubro-Negra\n";

    for (;;) {
        int op = -1;
        if (!lerOpcao(op)) continue;
        if (op == 0) {
            std::cout << "Encerrando.\n";
            break;
        }

        int k = 0;
        switch (op) {
        case 1:
            if (!lerInteiro("Chave a inserir: ", k)) break;
            std::cout << (arvore.inserir(k) ? "Inserido.\n" : "Duplicado (inalterado).\n");
            break;
        case 2:
            if (!lerInteiro("Chave a remover: ", k)) break;
            std::cout << (arvore.remover(k) ? "Removido.\n" : "Inexistente.\n");
            break;
        case 3:
            if (!lerInteiro("Chave a buscar: ", k)) break;
            std::cout << (arvore.buscar(k) ? "Encontrado.\n" : "Nao encontrado.\n");
            break;
        case 4:
            std::cout << "Em ordem: ";
            arvore.emOrdem();
            break;
        case 5:
            arvore.imprimir();
            break;
        case 6:
            std::cout << "Altura: " << arvore.altura() << "\n";
            std::cout << "Rotacoes: " << arvore.rotacoes() << "\n";
            std::cout << "Recoloracoes: " << arvore.recoloracoes() << "\n";
            if (!arvore.verificarInvariantes()) std::cout << "AVISO: invariantes violados.\n";
            break;
        default:
            std::cout << "Opcao invalida.\n";
            break;
        }
    }
}

} // namespace

int main() {
    std::cout << "=== Logistica Black Friday ===\n";
    std::cout << "Escolha a estrutura (1=AVL, 2=Rubro-Negra): ";

    int tipo = 0;
    if (!(std::cin >> tipo)) {
        limparEntrada();
        std::cout << "Entrada invalida. Encerrando.\n";
        return 1;
    }

    if (tipo == 1) {
        menuAVL();
    } else if (tipo == 2) {
        menuRubroNegra();
    } else {
        std::cout << "Opcao invalida. Encerrando.\n";
        return 1;
    }

    return 0;
}
