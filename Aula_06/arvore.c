// Uma Árvore Binária de Busca (BST - Binary Search Tree) é uma das variações mais importantes
// de árvores em estruturas de dados. Nela, cada nó possui no máximo dois filhos
//(chamados de esquerda e direita), e a regra fundamental é que todos os valores à esquerda
//de um nó são menores que ele, enquanto todos os valores à direita são maiores.

//Abaixo encontra-se uma implementação completa em linguagem C, contendo alocação dinâmica,
//inserção e os três principais métodos de percurso (travessia), com comentários detalhados
//linha por linha.



#include <stdio.h>
#include <stdlib.h>

// Definição da estrutura do nó da árvore
typedef struct No {
    int valor;          // Dado armazenado no nó
    struct No* esquerda;// Ponteiro para o filho à esquerda (valores menores)
    struct No* direita; // Ponteiro para o filho à direita (valores maiores)
} No;

// Função para criar um novo nó com alocação dinâmica de memória
No* criarNo(int valor) {
    No* novoNo = (No*) malloc(sizeof(No));

    // Verifica se a memória foi alocada com sucesso
    if (novoNo == NULL) {
        printf("Erro ao alocar memoria!\n");
        exit(1);
    }

    novoNo->valor = valor;
    novoNo->esquerda = NULL; // Inicialmente os filhos são nulos
    novoNo->direita = NULL;

    return novoNo;
}

// Função recursiva para inserir um valor na Árvore Binária de Busca
No* inserir(No* raiz, int valor) {
    // Se a árvore (ou sub-árvore) estiver vazia, chegamos ao ponto de inserção
    if (raiz == NULL) {
        return criarNo(valor);
    }

    // Se o valor for menor que o nó atual, vai para a sub-árvore esquerda
    if (valor < raiz->valor) {
        raiz->esquerda = inserir(raiz->esquerda, valor);
    }
    // Se o valor for maior, vai para a sub-árvore direita
    else if (valor > raiz->valor) {
        raiz->direita = inserir(raiz->direita, valor);
    }
    // Se o valor já existe, ignoramos para evitar duplicatas na BST

    return raiz; // Retorna o ponteiro do nó (atualizado ou inalterado)
}

// Percurso Em Ordem (In-order): Esquerda -> Raiz -> Direita
// Numa BST, este percurso exibe os elementos em ordem crescente.
void imprimirEmOrdem(No* raiz) {
    if (raiz != NULL) {
        imprimirEmOrdem(raiz->esquerda); // Visita a sub-árvore esquerda
        printf("%d ", raiz->valor);      // Visita a raiz
        imprimirEmOrdem(raiz->direita);  // Visita a sub-árvore direita
    }
}

// Percurso Pré-Ordem (Pre-order): Raiz -> Esquerda -> Direita
// Útil para clonar ou salvar a estrutura da árvore.
void imprimirPreOrdem(No* raiz) {
    if (raiz != NULL) {
        printf("%d ", raiz->valor);      // Visita a raiz primeiro
        imprimirPreOrdem(raiz->esquerda);// Visita a sub-árvore esquerda
        imprimirPreOrdem(raiz->direita); // Visita a sub-árvore direita
    }
}

// Percurso Pós-Ordem (Post-order): Esquerda -> Direita -> Raiz
// Muito utilizado para liberar memória de forma segura (de baixo para cima).
void imprimirPosOrdem(No* raiz) {
    if (raiz != NULL) {
        imprimirPosOrdem(raiz->esquerda);// Visita a sub-árvore esquerda
        imprimirPosOrdem(raiz->direita); // Visita a sub-árvore direita
        printf("%d ", raiz->valor);      // Visita a raiz por último
    }
}

// Função para liberar toda a memória alocada pela árvore recursivamente
void liberarArvore(No* raiz) {
    if (raiz != NULL) {
        liberarArvore(raiz->esquerda);
        liberarArvore(raiz->direita);
        free(raiz); // Libera o nó atual após liberar seus filhos
    }
}

// Função recursiva para buscar um valor na Árvore Binária de Busca
// Retorna um ponteiro para o nó se encontrar, ou NULL caso o valor não exista
No* buscar(No* raiz, int valor) {
    // Caso base 1: Se a raiz for nula, o valor não está na árvore
    // Caso base 2: Se o valor do nó atual for igual ao procurado, encontramos!
    if (raiz == NULL || raiz->valor == valor) {
        return raiz;
    }

    // Se o valor procurado for menor que o nó atual, buscamos na sub-árvore esquerda
    if (valor < raiz->valor) {
        return buscar(raiz->esquerda, valor);
    }

    // Caso contrário (se for maior), buscamos na sub-árvore direita
    return buscar(raiz->direita, valor);
}

// Função auxiliar para encontrar o nó com o menor valor (usado na remoção de nós com dois filhos)
No* encontrarMinimo(No* raiz) {
    No* atual = raiz;
    // O menor valor em uma BST está sempre no nó mais à esquerda possível
    while (atual != NULL && atual->esquerda != NULL) {
        atual = atual->esquerda;
    }
    printf("menor valor = %d ", atual->valor);
    return atual;
}

// Função recursiva para remover um valor da Árvore Binária de Busca
No* remover(No* raiz, int valor) {
    // Se a árvore estiver vazia, retorna NULL
    if (raiz == NULL) {
        return raiz;
    }

    // Se o valor a ser removido for menor que o nó atual, está na sub-árvore esquerda
    if (valor < raiz->valor) {
        raiz->esquerda = remover(raiz->esquerda, valor);
    }
    // Se o valor a ser removido for maior que o nó atual, está na sub-árvore direita
    else if (valor > raiz->valor) {
        raiz->direita = remover(raiz->direita, valor);
    }
    // Se o valor for igual ao atual, este é o nó a ser removido!
    else {
        // Caso 1: Nó sem filhos (folha) ou com apenas um filho
        if (raiz->esquerda == NULL) {
            No* temp = raiz->direita;
            free(raiz);
            return temp;
        }
        else if (raiz->direita == NULL) {
            No* temp = raiz->esquerda;
            free(raiz);
            return temp;
        }

        // Caso 2: Nó com dois filhos
        // Encontra o sucessor em ordem (o menor valor da sub-árvore direita)
        No* temp = encontrarMinimo(raiz->direita);

        // Substitui o valor do nó atual pelo valor do sucessor
        raiz->valor = temp->valor;

        // Remove o sucessor antigo da sub-árvore direita recursivamente
        raiz->direita = remover(raiz->direita, temp->valor);
    }

    return raiz;
}

// Função principal para demonstrar o uso
int main() {
    No* raiz = NULL;

    // Inserindo elementos na árvore
    // Estrutura gerada: raiz 50, com galhos à esquerda e à direita
    raiz = inserir(raiz, 50);
    inserir(raiz, 30);
    inserir(raiz, 20);
    inserir(raiz, 40);
    inserir(raiz, 70);
    inserir(raiz, 60);
    inserir(raiz, 80);
    inserir(raiz, 55);
    inserir(raiz, 45);
    inserir(raiz, 14);
    inserir(raiz, 65);
    inserir(raiz, 64);
    inserir(raiz, 66);

    printf("=== DEMONSTRACAO DE ARVORE BINARIA DE BUSCA (BST) ===\n\n");

    printf("Percurso Em Ordem (Crescente): ");
    imprimirEmOrdem(raiz);
    printf("\n");

    printf("Percurso Pre-Ordem:           ");
    imprimirPreOrdem(raiz);
    printf("\n");

    printf("Percurso Pos-Ordem:           ");
    imprimirPosOrdem(raiz);
    printf("\n");


    printf("=== TESTE DE BUSCA NA BST ===\n\n");

    // Testando a busca por um valor existente
    int valorProcurado = 60;
    No* resultado = buscar(raiz, valorProcurado);

    if (resultado != NULL) {
        printf("Valor %d ENCONTRADO na arvore!\n", valorProcurado);
    } else {
        printf("Valor %d NAO encontrado na arvore.\n", valorProcurado);
    }

    // Testando a busca por um valor inexistente
    int valorAusente = 99;
    resultado = buscar(raiz, valorAusente);

    if (resultado != NULL) {
        printf("Valor %d ENCONTRADO na arvore!\n", valorAusente);
    } else {
        printf("Valor %d NAO encontrado na arvore.\n", valorAusente);
    }

    printf("\nArvore antes da remocao (Em Ordem): ");
    imprimirEmOrdem(raiz);
    printf("\n");

    // 1. Removendo um nó folha (ex: 20)

//    int valorRemover1 = 20;
//    printf("\nRemovendo o valor %d (No folha)...\n", valorRemover1);
//    raiz = remover(raiz, valorRemover1);
//    printf("\nEm ordem apos remover %d: ", valorRemover1);
//    imprimirEmOrdem(raiz);
//    printf("\n\n");

    // 2. Removendo um nó com dois filhos (ex: 30, possui filhos 20(ja removido)->null e 40)
    // Vamos remover o 50 (raiz da árvore, que tem duas sub-árvores)

    int valorRemover2 = 60;
    printf("\nRemovendo o valor %d (Raiz com dois filhos)...\n", valorRemover2);
    raiz = remover(raiz, valorRemover2);
    printf("\nEm ordem apos remover %d: ", valorRemover2);
    imprimirEmOrdem(raiz);
    printf("\n\n");

    // Limpando a memória antes de encerrar o programa
    liberarArvore(raiz);
    raiz = NULL;

    return 0;
}



// Entendendo os Tipos de Percurso
// Em Ordem: Em uma BST, essa travessia é excelente porque garante que os dados sejam impressos de forma perfeitamente ordenada do menor para o maior.

// Pré-Ordem: Visita o nó pai antes dos filhos. É muito comum quando precisamos duplicar uma árvore exatamente como ela está estruturada.

// Pós-Ordem: Visita os filhos antes do pai. É essencial para funções de limpeza de memória (free), pois garante que os nós inferiores sejam apagados antes do nó raiz.
