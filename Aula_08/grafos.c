
#include <stdio.h>
#include <stdlib.h>

#define NUM_VERTICES 4

// ==========================================
// 1. MATRIZ DE ADJACÊNCIA
// ==========================================

void initMatrix(int matrix[NUM_VERTICES][NUM_VERTICES]) {
    for (int i = 0; i < NUM_VERTICES; i++) {
        for (int j = 0; j < NUM_VERTICES; j++) {
            matrix[i][j] = 0;
        }
    }
}

void addEdgeMatrix(int matrix[NUM_VERTICES][NUM_VERTICES], int src, int dest) {
    matrix[src][dest] = 1;
    matrix[dest][src] = 1; // Como o grafo é não direcionado, espelha a aresta
}

void printMatrix(int matrix[NUM_VERTICES][NUM_VERTICES]) {
    printf("--- Matriz de Adjacencia ---\n    ");
    for (int i = 0; i < NUM_VERTICES; i++) printf("%d ", i);
    printf("\n  +---------\n");
    for (int i = 0; i < NUM_VERTICES; i++) {
        printf("%d | ", i);
        for (int j = 0; j < NUM_VERTICES; j++) {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}


// ==========================================
// 2. LISTA DE ADJACÊNCIA
// ==========================================

struct Node {
    int dest;
    struct Node* next;
};

struct AdjList {
    struct Node* head;
};

struct GraphList {
    int numVertices;
    struct AdjList* array;
};

struct Node* createNode(int dest) {
    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
    newNode->dest = dest;
    newNode->next = NULL;
    return newNode;
}

struct GraphList* createGraphList(int vertices) {
    struct GraphList* graph = (struct GraphList*)malloc(sizeof(struct GraphList));
    graph->numVertices = vertices;
    graph->array = (struct AdjList*)malloc(vertices * sizeof(struct AdjList));

    for (int i = 0; i < vertices; ++i) {
        graph->array[i].head = NULL;
    }
    return graph;
}

void addEdgeList(struct GraphList* graph, int src, int dest) {
    // Adiciona dest na lista de src
    struct Node* newNode = createNode(dest);
    newNode->next = graph->array[src].head;
    graph->array[src].head = newNode;

    // Adiciona src na lista de dest (grafo não direcionado)
    newNode = createNode(src);
    newNode->next = graph->array[dest].head;
    graph->array[dest].head = newNode;
}

void printGraphList(struct GraphList* graph) {
    printf("--- Lista de Adjacencia ---\n");
    for (int v = 0; v < graph->numVertices; ++v) {
        struct Node* pCrawl = graph->array[v].head;
        printf("[%d]", v);
        while (pCrawl != NULL) {
            printf(" -> %d", pCrawl->dest);
            pCrawl = pCrawl->next;
        }
        printf(" -> NULL\n");
    }
}


// ==========================================
// FUNÇÃO PRINCIPAL (MAIN)
// ==========================================

int main() {
    // --- Testando a Matriz ---
    int matrix[NUM_VERTICES][NUM_VERTICES];
    initMatrix(matrix);

    addEdgeMatrix(matrix, 0, 1);
    addEdgeMatrix(matrix, 0, 2);
    addEdgeMatrix(matrix, 1, 2);
    addEdgeMatrix(matrix, 2, 3);

    printMatrix(matrix);

    // --- Testando a Lista ---
    struct GraphList* graphList = createGraphList(NUM_VERTICES); //cria um grafo de lista com 4 vértices

    addEdgeList(graphList, 0, 1); //faz a ligação do 0 para o 1
    addEdgeList(graphList, 0, 2); //faz a ligação do 0 para o 2
    addEdgeList(graphList, 1, 2); //faz a ligação do 1 para o 2
    addEdgeList(graphList, 2, 3); //faz a ligação do 2 para o 3

    printGraphList(graphList);

    return 0;
}
