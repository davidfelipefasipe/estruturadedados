#include <stdio.h>
#include <stdlib.h>

// Definição da estrutura do nó
typedef struct No {
    int valor;
    struct No* proximo;
} No;

// Cria um novo nó dinamicamente
No* criar_no(int valor) {
    No* novo = (No*)malloc(sizeof(No));
    if (novo == NULL) {
        printf("Erro ao alocar memória.\n");
        exit(EXIT_FAILURE);
    }
    novo->valor = valor;
    novo->proximo = NULL;
    return novo;
}

// Insere um novo elemento no início da lista
void inserir_inicio(No** cabeca, int valor) {
    No* novo = criar_no(valor);
    novo->proximo = *cabeca;
    *cabeca = novo;
}

// Insere um novo elemento no fim da lista
void inserir_fim(No** cabeca, int valor) {
    No* novo = criar_no(valor);
    if (*cabeca == NULL) {
        *cabeca = novo;
        return;
    }
    No* atual = *cabeca;
    while (atual->proximo != NULL) {
        atual = atual->proximo;
    }
    atual->proximo = novo;
}

// Busca um valor na lista. Retorna o ponteiro para o nó ou NULL se não encontrar.
No* buscar(No* cabeca, int valor) {
    No* atual = cabeca;
    while (atual != NULL) {
        if (atual->valor == valor) {
            return atual; // Retorna o nó onde o valor foi encontrado
        }
        atual = atual->proximo;
    }
    return NULL; // Valor não presente na lista
}

// Remove a primeira ocorrência de um valor na lista
void remover(No** cabeca, int valor) {
    if (*cabeca == NULL) return; // Lista vazia

    No* atual = *cabeca;
    No* anterior = NULL;

    // Caso 1: O elemento a ser removido é o primeiro nó (cabeça)
    if (atual != NULL && atual->valor == valor) {
        *cabeca = atual->proximo; // Atualiza a cabeça para o segundo nó
        free(atual);              // Libera a memória do antigo primeiro nó
        return;
    }

    // Caso 2: Procura o nó a ser removido mantendo a referência do nó anterior
    while (atual != NULL && atual->valor != valor) {
        anterior = atual;
        atual = atual->proximo;
    }

    // Se o valor não foi encontrado na lista
    if (atual == NULL) return;

    // Desconecta o nó da lista e libera a memória
    anterior->proximo = atual->proximo;
    free(atual);
}

// Exibe os elementos da lista no terminal
void imprimir(No* cabeca) {
    No* atual = cabeca;
    while (atual != NULL) {
        printf("%d -> ", atual->valor);
        atual = atual->proximo;
    }
    printf("NULL\n");
}

// Desaloca a memória de todos os nós da lista
void liberar_lista(No** cabeca) {
    No* atual = *cabeca;
    while (atual != NULL) {
        No* proximo = atual->proximo;
        free(atual);
        atual = proximo;
    }
    *cabeca = NULL;
}

int main() {
    No* lista = NULL;

    inserir_inicio(&lista, 10);
    inserir_inicio(&lista, 5);
    inserir_fim(&lista, 20);
    inserir_fim(&lista, 30);

    printf("Lista inicial:\n");
    imprimir(lista); // Saída: 5 -> 10 -> 20 -> 30 -> NULL

    // 1. Testando a Busca
    int busca = 20;
    No* resultado = buscar(lista, busca);
    if (resultado != NULL) {
        printf("\nValor %d encontrado no endereco de memoria: %p\n", resultado->valor, (void*)resultado);
    } else {
        printf("\nValor %d nao encontrado.\n", busca);
    }

    // 2. Testando Remoção no meio da lista
    printf("\nRemovendo o valor 20:\n");
    remover(&lista, 20);
    imprimir(lista); // Saída: 5 -> 10 -> 30 -> NULL

    // 3. Testando Remoção do primeiro nó (cabeça)
    printf("\nRemovendo o valor 5 (cabeca):\n");
    remover(&lista, 5);
    imprimir(lista); // Saída: 10 -> 30 -> NULL

    liberar_lista(&lista);
    return 0;
}
