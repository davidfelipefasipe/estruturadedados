#include <stdio.h>   // Biblioteca padrão para entrada e saída de dados (printf, etc.)
#include <stdlib.h>  // Biblioteca padrão para gerenciamento de memória (malloc, free)
#include <string.h>  // Biblioteca para manipulação de strings (strlen, strcpy, strcmp)
#include <locale.h>  // Biblioteca para configuração do locale e caracteres especiais

// Diretivas para suporte a UTF-8 no terminal do Windows
#ifdef _WIN32
#include <windows.h> // API do Windows para ajustar a página de código do console
#endif

// Definição da estrutura do Nó para lista simplesmente encadeada
typedef struct No {
    char *dado;           // Ponteiro para a string (texto em UTF-8)
    struct No *proximo;   // Ponteiro apenas para o próximo nó da lista
} No;

// Definição da estrutura da Lista Circular Simplesmente Encadeada
typedef struct {
    No *inicio;           // Ponteiro apontando para o primeiro nó da lista
    No *fim;              // Ponteiro apontando para o último nó (cujo proximo aponta para o inicio)
    size_t tamanho;       // Armazena a quantidade de elementos na lista
} ListaCircularSimples;

// Função que cria e inicializa uma lista circular vazia
ListaCircularSimples* criar_lista(void) {
    // Aloca a memória necessária para a estrutura da lista
    ListaCircularSimples *lista = (ListaCircularSimples*)malloc(sizeof(ListaCircularSimples));

    // Retorna NULO caso a alocação de memória falhe
    if (!lista) return NULL;

    // Define o ponteiro de início como NULO
    lista->inicio = NULL;

    // Define o ponteiro de fim como NULO
    lista->fim = NULL;

    // Inicializa o tamanho da lista como 0
    lista->tamanho = 0;

    // Retorna a lista criada
    return lista;
}

// Função auxiliar para criar um novo nó individual
No* criar_no(const char *texto) {
    // Aloca a memória necessária para a estrutura do nó
    No *novo_no = (No*)malloc(sizeof(No));

    // Retorna NULO se a alocação do nó falhar
    if (!novo_no) return NULL;

    // Aloca memória para a string + 1 byte para o caractere nulo ('\0')
    novo_no->dado = (char*)malloc(strlen(texto) + 1);

    // Libera o nó e retorna NULO caso a alocação da string falhe
    if (!novo_no->dado) {
        free(novo_no); // Evita vazamento de memória
        return NULL;
    }

    // Copia o texto para o espaço alocado internamente no nó
    strcpy(novo_no->dado, texto);

    // Inicializa o ponteiro proximo como NULO
    novo_no->proximo = NULL;

    // Retorna o ponteiro do nó criado
    return novo_no;
}

// Insere um novo nó no início da lista circular
void inserir_inicio(ListaCircularSimples *lista, const char *texto) {
    // Cria o novo nó com a string informada
    No *novo_no = criar_no(texto);

    // Se a criação do nó falhar, encerra a função
    if (!novo_no) return;

    // Caso a lista esteja vazia
    if (lista->inicio == NULL) {
        // O nó aponta para si mesmo formando o círculo
        novo_no->proximo = novo_no;

        // O início e o fim da lista passam a ser este novo nó
        lista->inicio = lista->fim = novo_no;
    } else {
        // O próximo do novo nó aponta para o antigo início
        novo_no->proximo = lista->inicio;

        // Atualiza o início da lista para ser o novo nó
        lista->inicio = novo_no;

        // O último nó da lista deve apontar para o novo início para manter a circularidade
        lista->fim->proximo = lista->inicio;
    }

    // Incrementa a contagem de elementos
    lista->tamanho++;
}

// Insere um novo nó no fim da lista circular
void inserir_fim(ListaCircularSimples *lista, const char *texto) {
    // Cria o novo nó com a string informada
    No *novo_no = criar_no(texto);

    // Se a criação do nó falhar, encerra a função
    if (!novo_no) return;

    // Caso a lista esteja vazia
    if (lista->fim == NULL) {
        // O nó aponta para si mesmo formando o círculo
        novo_no->proximo = novo_no;

        // O início e o fim da lista passam a ser este novo nó
        lista->inicio = lista->fim = novo_no;
    } else {
        // O novo nó aponta para o início da lista
        novo_no->proximo = lista->inicio;

        // O antigo fim da lista passa a apontar para o novo nó
        lista->fim->proximo = novo_no;

        // Atualiza o ponteiro fim da lista para o novo nó
        lista->fim = novo_no;
    }

    // Incrementa a contagem de elementos
    lista->tamanho++;
}

// Remove o primeiro nó que contiver o texto pesquisado
int remover_no(ListaCircularSimples *lista, const char *texto) {
    // Se a lista estiver vazia, não há o que remover
    if (lista->inicio == NULL) return 0;

    // Ponteiro para caminhar na lista
    No *atual = lista->inicio;

    // Ponteiro para rastrear o nó anterior (inicia apontando para o fim)
    No *anterior = lista->fim;

    // Percorre a lista circular elemento por elemento
    for (size_t i = 0; i < lista->tamanho; i++) {
        // Compara se o conteúdo do nó atual é igual ao texto procurado
        if (strcmp(atual->dado, texto) == 0) {

            // Caso especial: existe apenas 1 elemento na lista
            if (lista->tamanho == 1) {
                lista->inicio = NULL;
                lista->fim = NULL;
            } else {
                // O nó anterior passa a apontar para o nó seguinte ao atual
                anterior->proximo = atual->proximo;

                // Se o nó removido for o primeiro da lista
                if (atual == lista->inicio) {
                    lista->inicio = atual->proximo; // Atualiza o ponteiro de início
                }

                // Se o nó removido for o último da lista
                if (atual == lista->fim) {
                    lista->fim = anterior; // Atualiza o ponteiro de fim
                }
            }

            // Libera a memória alocada para o texto do nó
            free(atual->dado);

            // Libera a memória alocada para o próprio nó
            free(atual);

            // Decrementa a quantidade de elementos
            lista->tamanho--;

            // Retorna 1 indicando remoção bem-sucedida
            return 1;
        }

        // Avança o ponteiro anterior para o nó atual
        anterior = atual;

        // Avança o ponteiro atual para o próximo nó
        atual = atual->proximo;
    }

    // Retorna 0 caso a string não seja encontrada na lista
    return 0;
}

// Imprime o conteúdo da lista circular do início ao fim
void imprimir_lista(const ListaCircularSimples *lista) {
    // Se a lista estiver vazia, exibe colchetes vazios
    if (lista->inicio == NULL) {
        printf("Lista Circular: [ ] (vazia)\n");
        return;
    }

    // Inicia a leitura a partir do primeiro nó
    No *atual = lista->inicio;

    // Exibe o cabeçalho
    printf("Lista Circular: [ ");

    // Laço executado até completar uma volta inteira na lista
    do {
        // Imprime o texto do nó atual
        printf("\"%s\" ", atual->dado);

        // Avança para o próximo nó
        atual = atual->proximo;

    } while (atual != lista->inicio); // Para quando retornar ao primeiro elemento

    // Indica visualmente a circularidade conectando o último elemento de volta ao primeiro
    printf("] -> (volta para \"%s\")\n", lista->inicio->dado);
}

// Libera toda a memória alocada pela lista circular
void liberar_lista(ListaCircularSimples *lista) {
    // Se a lista já estiver vazia, apenas libera a estrutura principal
    if (lista->inicio == NULL) {
        free(lista);
        return;
    }

    // Inicia a varredura pelo primeiro nó
    No *atual = lista->inicio;
    No *proximo_no;

    // Percorre todos os nós desalocando um por um
    for (size_t i = 0; i < lista->tamanho; i++) {
        // Salva a referência do próximo nó antes de destruir o atual
        proximo_no = atual->proximo;

        // Libera a memória da string
        free(atual->dado);

        // Libera a memória da estrutura do nó
        free(atual);

        // Avança para o próximo nó salvo
        atual = proximo_no;
    }

    // Libera a estrutura principal da lista por último
    free(lista);
}

// Função principal de testes
int main(void) {
    // Configura a localização do sistema
    setlocale(LC_ALL, "");

    // Ajusta o console do Windows para exibir UTF-8 corretamente
#ifdef _WIN32
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
#endif

    // Cria a lista circular
    ListaCircularSimples *minha_lista = criar_lista();

    // Insere elementos com caracteres especiais
    inserir_fim(minha_lista, "Carrossel");
    inserir_fim(minha_lista, "Dança");
    inserir_fim(minha_lista, "Canção");
    inserir_inicio(minha_lista, "Apresentação");

    printf("--- Exibindo a Lista Circular ---\n");
    imprimir_lista(minha_lista);

    printf("\n--- Removendo 'Dança' ---\n");
    remover_no(minha_lista, "Dança");
    imprimir_lista(minha_lista);

    printf("\n--- Removendo o Primeiro Item ('Apresentação') ---\n");
    remover_no(minha_lista, "Apresentação");
    imprimir_lista(minha_lista);

    // Libera a memória utilizada
    liberar_lista(minha_lista);

    return 0;
}
