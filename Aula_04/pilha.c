#include <stdio.h>   // Biblioteca para funções de entrada e saída de dados (ex: printf)
#include <stdlib.h>  // Biblioteca para gerenciamento de memória dinâmica (malloc, free)
#include <string.h>  // Biblioteca para manipulação de cadeias de caracteres (strlen, strcpy)
#include <locale.h>  // Biblioteca para definir a localização regional e suporte UTF-8

// Diretivas para suporte ao console do Windows
#ifdef _WIN32
#include <windows.h> // API do Windows para definir a página de código do terminal
#endif

// Definição da estrutura do Nó da Pilha
typedef struct No {
    char *dado;           // Ponteiro para armazenar o texto em UTF-8
    struct No *proximo;   // Ponteiro para o nó imediatamente abaixo na pilha
} No;

// Definição da estrutura da Pilha
typedef struct {
    No *topo;             // Ponteiro que aponta para o elemento no topo da pilha
    size_t tamanho;       // Armazena o número total de elementos empilhados
} Pilha;

// Função responsável por criar e inicializar uma pilha vazia
Pilha* criar_pilha(void) {
    // Aloca a memória necessária para a estrutura da pilha
    Pilha *pilha = (Pilha*)malloc(sizeof(Pilha));

    // Retorna NULO caso a alocação de memória falhe
    if (!pilha) return NULL;

    // Define o ponteiro do topo como NULO (pilha inicia vazia)
    pilha->topo = NULL;

    // Inicializa o tamanho da pilha como zero
    pilha->tamanho = 0;

    // Retorna a pilha criada
    return pilha;
}

// Função para verificar se a pilha está vazia
int esta_vazia(const Pilha *pilha) {
    // Retorna 1 (verdadeiro) se a pilha for nula ou o topo for NULO, senão retorna 0 (falso)
    return (pilha == NULL || pilha->topo == NULL);
}

// Função auxiliar para alocar e inicializar um novo Nó
No* criar_no(const char *texto) {
    // Aloca a memória necessária para a estrutura do nó
    No *novo_no = (No*)malloc(sizeof(No));

    // Se a alocação falhar, encerra retornando NULO
    if (!novo_no) return NULL;

    // Aloca memória exata para a string + 1 byte para o caractere nulo '\0'
    novo_no->dado = (char*)malloc(strlen(texto) + 1);

    // Se a alocação da string falhar, libera a memória do nó e retorna NULO
    if (!novo_no->dado) {
        free(novo_no); // Previne vazamento de memória
        return NULL;
    }

    // Copia o texto para a memória do nó
    strcpy(novo_no->dado, texto);

    // O próximo do novo nó é inicializado como NULO
    novo_no->proximo = NULL;

    // Retorna o nó criado
    return novo_no;
}

// Função Push: Insere um novo elemento no topo da pilha
void empilhar(Pilha *pilha, const char *texto) {
    // Verifica se a pilha é válida
    if (!pilha) return;

    // Cria um novo nó com o texto fornecido
    No *novo_no = criar_no(texto);

    // Se a criação do nó falhar, encerra a função
    if (!novo_no) return;

    // O próximo do novo nó passa a ser o antigo topo da pilha
    novo_no->proximo = pilha->topo;

    // O topo da pilha é atualizado para o novo nó
    pilha->topo = novo_no;

    // Incrementa a contagem de elementos na pilha
    pilha->tamanho++;
}

// Função Pop: Remove e retorna o texto do elemento no topo da pilha
char* desempilhar(Pilha *pilha) {
    // Se a pilha estiver vazia ou inválida, não há o que remover
    if (esta_vazia(pilha)) return NULL;

    // Salva a referência do nó que está atualmente no topo
    No *no_removido = pilha->topo;

    // Ponteiro que vai guardar a cópia da string para retornar ao usuário
    char *texto_retornado = (char*)malloc(strlen(no_removido->dado) + 1);

    // Se a alocação falhar, não realiza a remoção
    if (!texto_retornado) return NULL;

    // Copia o texto do nó removido para a nova string
    strcpy(texto_retornado, no_removido->dado);

    // O topo da pilha passa a ser o nó logo abaixo
    pilha->topo = no_removido->proximo;

    // Libera a memória da string interna do nó desempilhado
    free(no_removido->dado);

    // Libera a memória da estrutura do nó desempilhado
    free(no_removido);

    // Decrementa a quantidade de elementos
    pilha->tamanho--;

    // Retorna a cópia da string (que deve ser liberada com free pelo chamador)
    return texto_retornado;
}

// Função Peek: Retorna o texto do topo sem remover o elemento
const char* consultar_topo(const Pilha *pilha) {
    // Se a pilha estiver vazia, retorna NULO
    if (esta_vazia(pilha)) return NULL;

    // Retorna o ponteiro da string armazenada no topo
    return pilha->topo->dado;
}

// Imprime a pilha do topo até a base
void imprimir_pilha(const Pilha *pilha) {
    // Se a pilha estiver vazia, exibe mensagem informativa
    if (esta_vazia(pilha)) {
        printf("Pilha vazia!\n");
        return;
    }

    // Inicia a navegação a partir do topo
    No *atual = pilha->topo;

    // Exibe o cabeçalho
    printf("--- Conteúdo da Pilha (Topo -> Base) ---\n");

    // Percorre todos os nós até chegar na base (NULL)
    while (atual != NULL) {
        // Imprime o elemento do nó atual
        printf("| %-20s |\n", atual->dado);

        // Avança para o nó abaixo
        atual = atual->proximo;
    }

    // Imprime a base visual da pilha
    printf("----------------------------------------\n");
}

// Libera toda a memória alocada pela pilha
void liberar_pilha(Pilha *pilha) {
    // Se a pilha for nula, encerra
    if (!pilha) return;

    // Aponta para o topo da pilha
    No *atual = pilha->topo;

    // Percorre todos os nós liberando a memória
    while (atual != NULL) {
        // Guarda a referência do próximo nó abaixo
        No *proximo = atual->proximo;

        // Libera a string interna do nó
        free(atual->dado);

        // Libera a estrutura do nó
        free(atual);

        // Avança para o próximo
        atual = proximo;
    }

    // Libera a estrutura principal da pilha
    free(pilha);
}

// Função principal para teste das operações
int main(void) {
    // Ajusta o idioma local do sistema
    setlocale(LC_ALL, "");

    // Configura o console do Windows para exibir caracteres UTF-8
#ifdef _WIN32
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
#endif

    // Cria uma nova pilha
    Pilha *minha_pilha = criar_pilha();

    // Empilha elementos com caracteres especiais em UTF-8
    empilhar(minha_pilha, "Açúcar");
    empilhar(minha_pilha, "Feijão");
    empilhar(minha_pilha, "Pão");
    empilhar(minha_pilha, "Informação");

    // Imprime o estado atual da pilha
    imprimir_pilha(minha_pilha);

    // Consulta o elemento no topo sem remover
    printf("Topo atual: \"%s\"\n\n", consultar_topo(minha_pilha));

    // Desempilha um elemento (o último a entrar: "Informação")
    char *item_removido = desempilhar(minha_pilha);
    if (item_removido) {
        printf("Item desempilhado: \"%s\"\n\n", item_removido);
        free(item_removido); // Libera a memória alocada para o retorno
    }

    // Exibe a pilha após a remoção
    imprimir_pilha(minha_pilha);

    // Libera toda a memória restante da pilha
    liberar_pilha(minha_pilha);

    return 0;
}
