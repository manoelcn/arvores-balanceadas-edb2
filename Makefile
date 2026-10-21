# Makefile — Árvores Balanceadas (IMD0039)
# Autor: Manoel Cândido Neto

CXX      = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2 -Isrc
LDFLAGS  =

SRCDIR   = src
TESTDIR  = tests
BINDIR   = bin
DOCDIR   = docs

TREE_SRC = $(SRCDIR)/avl.cpp $(SRCDIR)/rb.cpp
MARP     ?= marp
SLIDES   = apresentacao/apresentacao.md
SLIDES_PDF = apresentacao/apresentacao.pdf

.PHONY: all clean run logistica testes experimento resultados slides html-slides

all: logistica $(BINDIR)/testes $(BINDIR)/experimento

$(BINDIR):
	mkdir -p $(BINDIR)
	mkdir -p $(DOCDIR)/resultados

logistica: $(BINDIR) $(TREE_SRC) $(SRCDIR)/main.cpp
	$(CXX) $(CXXFLAGS) -o $(BINDIR)/logistica $(SRCDIR)/main.cpp $(TREE_SRC) $(LDFLAGS)

run: logistica
	./$(BINDIR)/logistica

$(BINDIR)/testes: $(BINDIR) $(TREE_SRC) $(TESTDIR)/testes.cpp
	$(CXX) $(CXXFLAGS) -o $(BINDIR)/testes $(TESTDIR)/testes.cpp $(TREE_SRC) $(LDFLAGS)

testes: $(BINDIR)/testes
	./$(BINDIR)/testes

$(BINDIR)/experimento: $(BINDIR) $(TREE_SRC) $(SRCDIR)/experimento.cpp
	$(CXX) $(CXXFLAGS) -o $(BINDIR)/experimento $(SRCDIR)/experimento.cpp $(TREE_SRC) $(LDFLAGS)

experimento: $(BINDIR)/experimento
	./$(BINDIR)/experimento

resultados: $(BINDIR)/testes $(BINDIR)/experimento
	./$(BINDIR)/testes | tee $(DOCDIR)/resultados/testes.log
	./$(BINDIR)/experimento > $(DOCDIR)/resultados/comparacao.csv
	./$(BINDIR)/experimento --exemplos > $(DOCDIR)/resultados/exemplos.txt

slides:
	$(MARP) $(SLIDES) --pdf --allow-local-files --pdf-outlines -o $(SLIDES_PDF)

html-slides:
	$(MARP) $(SLIDES) --allow-local-files -o apresentacao/apresentacao.html

clean:
	rm -rf $(BINDIR)/*.o $(BINDIR)/logistica $(BINDIR)/testes $(BINDIR)/experimento
