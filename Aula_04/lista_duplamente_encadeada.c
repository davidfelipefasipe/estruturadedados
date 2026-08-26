#include <stdio.h>   // Biblioteca padrão para entrada e saída de dados (printf, etc.)
#include <stdlib.h>  // Biblioteca padrão para gerenciamento de memória (malloc, free)
#include <string.h>  // Biblioteca para manipulação de cadeias de caracteres (strlen, strcpy, strcmp)
#include <locale.h>  // Biblioteca para definir a localização regional e suporte a caracteres

// Define diretivas específicas caso o programa seja compilado no Windows
#ifdef _WIN32
#include <windows.h> // Biblioteca da API do Windows usada para ajustar a página de código do terminal
#endif

// Definição da estrutura de cada elemento (Nó) da lista
typedef struct No {
    char *dado;           // Ponteiro para a string (texto em UTF-8)
    struct No *anterior;  // Ponteiro apontando para o nó anterior da lista
    struct No *proximo;   // Ponteiro apontando para o próximo nó da lista
} No;

// Definição da estrutura do cabeçalho da Lista Duplamente Encadeada
typedef struct {
    No *inicio;           // Ponteiro apontando para o primeiro nó da lista
    No *fim;              // Ponteiro apontando para o último nó da lista
    size_t tamanho;       // Armazena o número total de elementos presentes na lista
} ListaDuplamenteEncadeada;

// Função responsável por alocar e inicializar uma nova lista vazia
ListaDuplamenteEncadeada* criar_lista(void) {
    // Aloca a memória necessária para a estrutura da lista na memória Heap
    ListaDuplamenteEncadeada *lista = (ListaDuplamenteEncadeada*)malloc(sizeof(ListaDuplamenteEncadeada));

    // Verifica se a alocação de memória falhou
    if (!lista) return NULL;

    // Define o ponteiro de início como NULO (lista vazia)
    lista->inicio = NULL;

    // Define o ponteiro de fim como NULO (lista vazia)
    lista->fim = NULL;

    // Inicializa a contagem de elementos em 0
    lista->tamanho = 0;

    // Retorna o ponteiro para a lista recém-criada
    return lista;
}

// Função auxiliar para criar e inicializar um novo Nó individual
No* criar_no(const char *texto) {
    // Aloca a memória necessária para a estrutura do nó
    No *novo_no = (No*)malloc(sizeof(No));

    // Se a alocação falhar, interrompe a execução retornando NULO
    if (!novo_no) return NULL;

    // Aloca memória exata para a string (+1 byte para o caractere nulo final '\0')
    novo_no->dado = (char*)malloc(strlen(texto) + 1);

    // Caso a alocação da string falhe, libera a memória do nó alocado anteriormente
    if (!novo_no->dado) {
        free(novo_no); // Evita o vazamento de memória (memory leak)
        return NULL;   // Retorna NULO indicando falha
    }

    // Copia o conteúdo do texto informado para a memória interna do nó
    strcpy(novo_no->dado, texto);

    // O nó recém-criado começa sem apontar para nenhum nó anterior
    novo_no->anterior = NULL;

    // O nó recém-criado começa sem apontar para nenhum próximo nó
    novo_no->proximo = NULL;

    // Retorna o ponteiro do nó criado
    return novo_no;
}

// Função para inserir um novo texto no início da lista
void inserir_inicio(ListaDuplamenteEncadeada *lista, const char *texto) {
    // Tenta criar o novo nó com a string enviada
    No *novo_no = criar_no(texto);

    // Se a criação falhou por falta de memória, encerra a função
    if (!novo_no) return;

    // Verifica se a lista está vazia
    if (lista->inicio == NULL) {
        // Se a lista estiver vazia, o novo nó será simultaneamente o início e o fim
        lista->inicio = lista->fim = novo_no;
    } else {
        // Aponta o próximo elemento do novo nó para o antigo primeiro elemento
        novo_no->proximo = lista->inicio;

        // Faz o ponteiro anterior do antigo primeiro nó apontar para o novo nó
        lista->inicio->anterior = novo_no;

        // Atualiza o ponteiro de início da lista para o novo nó
        lista->inicio = novo_no;
    }

    // Incrementa a quantidade total de elementos na lista
    lista->tamanho++;
}

// Função para inserir um novo texto no fim da lista
void inserir_fim(ListaDuplamenteEncadeada *lista, const char *texto) {
    // Tenta criar o novo nó com a string enviada
    No *novo_no = criar_no(texto);

    // Se a criação falhou por falta de memória, encerra a função
    if (!novo_no) return;

    // Verifica se a lista está vazia
    if (lista->fim == NULL) {
        // Se a lista estiver vazia, o novo nó será simultaneamente o início e o fim
        lista->inicio = lista->fim = novo_no;
    } else {
        // Aponta o ponteiro anterior do novo nó para o antigo último elemento
        novo_no->anterior = lista->fim;

        // Faz o próximo do antigo último elemento apontar para o novo nó
        lista->fim->proximo = novo_no;

        // Atualiza o ponteiro de fim da lista para o novo nó
        lista->fim = novo_no;
    }

    // Incrementa a quantidade total de elementos na lista
    lista->tamanho++;
}

// Função para buscar e remover o primeiro nó que possua o texto especificado
int remover_no(ListaDuplamenteEncadeada *lista, const char *texto) {
    // Ponteiro temporário para percorrer a lista a partir do início
    No *atual = lista->inicio;

    // Percorre todos os nós enquanto não atingir o fim da lista (NULL)
    while (atual != NULL) {
        // Compara a string armazenada com a string procurada
        if (strcmp(atual->dado, texto) == 0) {

            // Se o nó a ser removido NÃO for o primeiro elemento da lista
            if (atual->anterior) {
                // Atualiza o ponteiro do nó anterior para pular o nó atual
                atual->anterior->proximo = atual->proximo;
            } else {
                // Se for o primeiro nó, o novo início passa a ser o nó seguinte
                lista->inicio = atual->proximo;
            }

            // Se o nó a ser removido NÃO for o último elemento da lista
            if (atual->proximo) {
                // Atualiza o ponteiro anterior do nó seguinte para pular o nó atual
                atual->proximo->anterior = atual->anterior;
            } else {
                // Se for o último nó, o novo fim passa a ser o nó anterior
                lista->fim = atual->anterior;
            }

            // Libera a memória alocada para armazenar o texto internamente
            free(atual->dado);

            // Libera a memória alocada para a estrutura do nó
            free(atual);

            // Decrementa a quantidade de elementos na lista
            lista->tamanho--;

            // Retorna 1 (sucesso na remoção)
            return 1;
        }

        // Avança para o próximo nó da lista
        atual = atual->proximo;
    }

    // Retorna 0 (indicando que o texto não foi encontrado)
    return 0;
}

// Exibe os elementos da lista na ordem do primeiro até o último
void imprimir_inicio_ao_fim(const ListaDuplamenteEncadeada *lista) {
    // Ponteiro para navegar começando pelo primeiro nó
    No *atual = lista->inicio;

    // Exibe o cabeçalho no console
    printf("Início -> Fim: [ ");

    // Percorre a lista até que o ponteiro atual seja NULO
    while (atual != NULL) {
        // Imprime o texto do nó atual formatado entre aspas
        printf("\"%s\" ", atual->dado);

        // Avança para o próximo nó
        atual = atual->proximo;
    }

    // Imprime o fechamento de colchetes e uma nova linha
    printf("]\n");
}

// Exibe os elementos da lista na ordem inversa (do último para o primeiro)
void imprimir_fim_ao_inicio(const ListaDuplamenteEncadeada *lista) {
    // Ponteiro para navegar começando pelo último nó
    No *atual = lista->fim;

    // Exibe o cabeçalho no console
    printf("Fim -> Início: [ ");

    // Percorre a lista retrocedendo até que o ponteiro atual seja NULO
    while (atual != NULL) {
        // Imprime o texto do nó atual formatado entre aspas
        printf("\"%s\" ", atual->dado);

        // Volta para o nó anterior
        atual = atual->anterior;
    }

    // Imprime o fechamento de colchetes e uma nova linha
    printf("]\n");
}

// Função responsável por varrer a lista e desalocar toda a memória utilizada
void liberar_lista(ListaDuplamenteEncadeada *lista) {
    // Inicia a varredura a partir do primeiro nó
    No *atual = lista->inicio;

    // Laço para percorrer e deletar cada nó individualmente
    while (atual != NULL) {
        // Salva a referência do próximo nó antes de apagar o atual
        No *proximo = atual->proximo;

        // Libera a memória da string contida no nó
        free(atual->dado);

        // Libera a memória da estrutura do nó
        free(atual);

        // Atualiza a navegação definindo que o "atual" passa a ser o "próximo"
        atual = proximo;
    }

    // Libera a estrutura principal da lista por último
    free(lista);
}

// Função principal de entrada do programa
int main(void) {
    // Ajusta o idioma local e suporte de codificação do sistema operacional
    setlocale(LC_ALL, "");

    // Configuração para garantir que o terminal do Windows interprete UTF-8 corretamente
#ifdef _WIN32
    SetConsoleOutputCP(65001); // Configura o terminal do Windows para a tabela de código 65001 (UTF-8) para saída
    SetConsoleCP(65001);       // Configura a tabela de código 65001 (UTF-8) para leitura de dados
#endif

    // Cria e inicializa uma nova lista duplamente encadeada
    ListaDuplamenteEncadeada *minha_lista = criar_lista();

    // Insere palavras com caracteres especiais UTF-8 para teste no fim da lista
    inserir_fim(minha_lista, "Vatapá");
    inserir_fim(minha_lista, "Maniçoba");
    inserir_fim(minha_lista, "Açaí");

    // Insere uma palavra no início da lista
    inserir_inicio(minha_lista, "Tacacá");

    // Imprime o cabeçalho do teste
    printf("--- Exibindo a Lista ---\n");

    // Imprime a lista do primeiro ao último elemento
    imprimir_inicio_ao_fim(minha_lista);

    // Imprime a lista em ordem reversa (do último ao primeiro)
    imprimir_fim_ao_inicio(minha_lista);

    // Demonstração da remoção de um item contendo caracteres especiais
    printf("\n--- Removendo 'Maniçoba' ---\n");
    remover_no(minha_lista, "Maniçoba");

    // Exibe novamente a lista atualizada
    imprimir_inicio_ao_fim(minha_lista);

    // Libera completamente toda a memória alocada antes de fechar o programa
    liberar_lista(minha_lista);

    // Retorna 0 informando ao sistema que o programa finalizou sem erros
    return 0;
}
