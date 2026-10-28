/*
 * Árvore Geradora Mínima (AGM) - Prim e Kruskal
 * - Contexto: Rodovias ligando municípios de Mato Grosso com custo mínimo (km)
 * - Compilar: gcc -std=c99 -Wall -o agm_mt agm_mt.c
 * - Executar: ./agm_mt
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define INF INT_MAX

/* ---------- Municipios (vertices) ---------- */
enum {
    SANTA_CARMEM, SINOP, IPIRANGA, TAPURAH, SORRISO, VERA,
    FELIZ_NATAL, NOVA_UBIRATA, LUCAS, NOVA_MUTUM, BOA_ESPERANCA,
    NUM_VERTICES
};

static const char *NOMES[NUM_VERTICES] = {
    "Santa Carmem", "Sinop", "Ipiranga do Norte", "Tapurah", "Sorriso",
    "Vera", "Feliz Natal", "Nova Ubirata", "Lucas do Rio Verde",
    "Nova Mutum", "Boa Esperanca do Norte"
};

/* ---------- Estruturas para Kruskal ---------- */
typedef struct {
    int u, v, peso;
} ArestaKruskal;

#define MAX_ARESTAS 64
static ArestaKruskal ARESTAS[MAX_ARESTAS];
static int total_arestas = 0;

void adicionar_aresta_kruskal(int u, int v, int peso) {
    // Evita duplicatas em grafo não direcionado para o Kruskal
    ARESTAS[total_arestas++] = (ArestaKruskal){u, v, peso};
}

/* Função de comparação para o qsort (ordenar arestas por peso crescente) */
int comparar_arestas(const void *a, const void *b) {
    ArestaKruskal *fa = (ArestaKruskal *)a;
    ArestaKruskal *fb = (ArestaKruskal *)b;
    return fa->peso - fb->peso;
}

/* Estrutura Union-Find (Disjoint Set) para detectar ciclos no Kruskal */
int pai_dsu[NUM_VERTICES];

int encontrar(int i) {
    if (pai_dsu[i] == i)
        return i;
    return pai_dsu[i] = encontrar(pai_dsu[i]); // Compressão de caminho
}

void unir(int i, int j) {
    int raiz_i = encontrar(i);
    int raiz_j = encontrar(j);
    if (raiz_i != raiz_j) {
        pai_dsu[raiz_i] = raiz_j;
    }
}

/* ---------- Kruskal ---------- */
void kruskal() {
    // Inicializa o Union-Find
    for (int i = 0; i < NUM_VERTICES; i++) {
        pai_dsu[i] = i;
    }

    // Ordena as arestas em ordem crescente de peso
    qsort(ARESTAS, total_arestas, sizeof(ArestaKruskal), comparar_arestas);

    printf("\n=== Algoritmo de Kruskal (AGM) ===\n");
    printf("Arestas selecionadas para interligar a regiao:\n");

    int custo_total = 0;
    int arestas_incluidas = 0;

    for (int i = 0; i < total_arestas; i++) {
        int u = ARESTAS[i].u;
        int v = ARESTAS[i].v;
        int peso = ARESTAS[i].peso;

        // Se u e v estão em componentes diferentes, não forma ciclo
        if (encontrar(u) != encontrar(v)) {
            unir(u, v);
            printf("  %-22s <---> %-22s : %3d km\n", NOMES[u], NOMES[v], peso);
            custo_total += peso;
            arestas_incluidas++;

            // Uma árvore com V vértices possui exatamente V-1 arestas
            if (arestas_incluidas == NUM_VERTICES - 1) break;
        }
    }
    printf("Custo Total Minimo (Kruskal): %d km\n", custo_total);
}


/* ---------- Prim ---------- */
void prim(int matriz_adj[NUM_VERTICES][NUM_VERTICES], int origem) {
    int pai[NUM_VERTICES];
    int chave[NUM_VERTICES];
    int na_mst[NUM_VERTICES];

    for (int i = 0; i < NUM_VERTICES; i++) {
        chave[i] = INF;
        na_mst[i] = 0;
        pai[i] = -1;
    }

    chave[origem] = 0;

    for (int count = 0; count < NUM_VERTICES - 1; count++) {
        // Encontra o vértice com a menor chave que ainda não está na MST
        int min = INF, u = -1;
        for (int v = 0; v < NUM_VERTICES; v++) {
            if (!na_mst[v] && chave[v] < min) {
                min = chave[v];
                u = v;
            }
        }

        if (u == -1) break; // Grafo desconexo

        na_mst[u] = 1;

        // Atualiza as chaves dos vizinhos do vértice escolhido
        for (int v = 0; v < NUM_VERTICES; v++) {
            if (matriz_adj[u][v] && !na_mst[v] && matriz_adj[u][v] < chave[v]) {
                pai[v] = u;
                chave[v] = matriz_adj[u][v];
            }
        }
    }

    printf("\n=== Algoritmo de Prim (A partir de %s) ===\n", NOMES[origem]);
    printf("Arestas selecionadas para a AGM:\n");
    int custo_total = 0;
    for (int i = 0; i < NUM_VERTICES; i++) {
        if (pai[i] != -1) {
            printf("  %-22s <---> %-22s : %3d km\n", NOMES[pai[i]], NOMES[i], matriz_adj[i][pai[i]]);
            custo_total += matriz_adj[i][pai[i]];
        }
    }
    printf("Custo Total Minimo (Prim): %d km\n", custo_total);
}


/* ---------- Função Principal (Teste Básico) ---------- */
int main(void) {
    // Matriz de adjacência para o algoritmo de Prim
    int matriz_adj[NUM_VERTICES][NUM_VERTICES];
    for (int i = 0; i < NUM_VERTICES; i++)
        for (int j = 0; j < NUM_VERTICES; j++)
            matriz_adj[i][j] = 0;

    // Macro auxiliar para popular o grafo em ambas as estruturas (Matriz e Lista de Arestas)
    #define ADD(u, v, p) do { \
        matriz_adj[u][v] = p; \
        matriz_adj[v][u] = p; \
        adicionar_aresta_kruskal(u, v, p); \
    } while(0)

    // Construindo o mapa rodoviário de MT
    ADD(SANTA_CARMEM, SINOP, 20);
    ADD(SANTA_CARMEM, VERA, 15);
    ADD(SINOP, IPIRANGA, 20);
    ADD(SINOP, SORRISO, 65);
    ADD(IPIRANGA, TAPURAH, 22);
    ADD(IPIRANGA, SORRISO, 33);
    ADD(TAPURAH, SORRISO, 17);
    ADD(VERA, SORRISO, 40);
    ADD(VERA, FELIZ_NATAL, 30);
    ADD(FELIZ_NATAL, NOVA_UBIRATA, 33);
    ADD(SORRISO, NOVA_UBIRATA, 50);
    ADD(SORRISO, LUCAS, 45);
    ADD(LUCAS, NOVA_MUTUM, 40);
    ADD(NOVA_MUTUM, BOA_ESPERANCA, 35);
    ADD(BOA_ESPERANCA, NOVA_UBIRATA, 25);

    #undef ADD

    // Executa os testes básicos de AGM
    kruskal();
    prim(matriz_adj, SANTA_CARMEM);

    return 0;
}
