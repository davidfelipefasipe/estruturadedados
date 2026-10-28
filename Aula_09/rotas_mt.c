/*
 * Grafo das rotas entre municipios de Mato Grosso
 * - Representacao: lista de adjacencia (grafo nao direcionado e ponderado)
 * - Algoritmos: BFS, DFS, Dijkstra, Bellman-Ford
 *
 * Compilar: gcc -std=c99 -Wall -o grafo_rotas_mt grafo_rotas_mt.c
 * Executar: ./grafo_rotas_mt
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

/* ---------- Estruturas do grafo ---------- */
typedef struct Aresta {
    int destino;
    int peso;
    struct Aresta *prox;
} Aresta;

typedef struct {
    int n;
    Aresta *adj[NUM_VERTICES];
} Grafo;

/* Lista de arestas (usada pelo Bellman-Ford) */
typedef struct { int u, v, peso; } ArestaLista;
#define MAX_ARESTAS 64
static ArestaLista ARESTAS[MAX_ARESTAS];
static int num_arestas = 0;

void grafo_init(Grafo *g, int n) {
    g->n = n;
    for (int i = 0; i < n; i++) g->adj[i] = NULL;
}

static void inserir_dir(Grafo *g, int u, int v, int peso) {
    Aresta *a = malloc(sizeof(Aresta));
    if (!a) { perror("malloc"); exit(1); }
    a->destino = v;
    a->peso = peso;
    a->prox = g->adj[u];
    g->adj[u] = a;
}

void adicionar_aresta(Grafo *g, int u, int v, int peso) {
    inserir_dir(g, u, v, peso);
    inserir_dir(g, v, u, peso);
    ARESTAS[num_arestas++] = (ArestaLista){u, v, peso};
}

void grafo_free(Grafo *g) {
    for (int i = 0; i < g->n; i++) {
        Aresta *a = g->adj[i];
        while (a) { Aresta *t = a; a = a->prox; free(t); }
        g->adj[i] = NULL;
    }
}

void construir_mapa(Grafo *g) {
    grafo_init(g, NUM_VERTICES);
    adicionar_aresta(g, SANTA_CARMEM, SINOP, 20);
    adicionar_aresta(g, SANTA_CARMEM, VERA, 15);
    adicionar_aresta(g, SINOP, IPIRANGA, 20);
    adicionar_aresta(g, SINOP, SORRISO, 65);
    adicionar_aresta(g, IPIRANGA, TAPURAH, 22);
    adicionar_aresta(g, IPIRANGA, SORRISO, 33);
    adicionar_aresta(g, TAPURAH, SORRISO, 17);
    adicionar_aresta(g, VERA, SORRISO, 40);
    adicionar_aresta(g, VERA, FELIZ_NATAL, 30);
    adicionar_aresta(g, FELIZ_NATAL, NOVA_UBIRATA, 33);
    adicionar_aresta(g, SORRISO, NOVA_UBIRATA, 50);
    adicionar_aresta(g, SORRISO, LUCAS, 45);
    adicionar_aresta(g, LUCAS, NOVA_MUTUM, 40);
    adicionar_aresta(g, NOVA_MUTUM, BOA_ESPERANCA, 35);
    adicionar_aresta(g, BOA_ESPERANCA, NOVA_UBIRATA, 25);
}

/* ---------- Utilidades ---------- */
void imprimir_grafo(const Grafo *g) {
    printf("\n=== Lista de adjacencia ===\n");
    for (int i = 0; i < g->n; i++) {
        printf("%-23s ->", NOMES[i]);
        for (Aresta *a = g->adj[i]; a; a = a->prox)
            printf(" [%s, %d km]", NOMES[a->destino], a->peso);
        printf("\n");
    }
}

/* Imprime o caminho origem -> destino usando o vetor de predecessores */
void imprimir_caminho(const int pai[], int origem, int destino) {
    int pilha[NUM_VERTICES], topo = 0;
    for (int v = destino; v != -1; v = pai[v]) {
        pilha[topo++] = v;
        if (v == origem) break;
    }
    for (int i = topo - 1; i >= 0; i--)
        printf("%s%s", NOMES[pilha[i]], i ? " -> " : "");
}

/* ---------- BFS (menor numero de arestas/"saltos") ---------- */
void bfs(const Grafo *g, int origem) {
    int visitado[NUM_VERTICES] = {0};
    int nivel[NUM_VERTICES], pai[NUM_VERTICES];
    int fila[NUM_VERTICES], ini = 0, fim = 0;

    for (int i = 0; i < g->n; i++) { nivel[i] = -1; pai[i] = -1; }

    visitado[origem] = 1;
    nivel[origem] = 0;
    fila[fim++] = origem;

    printf("\n=== BFS a partir de %s ===\nOrdem de visita: ", NOMES[origem]);
    while (ini < fim) {
        int u = fila[ini++];
        printf("%s ", NOMES[u]);
        for (Aresta *a = g->adj[u]; a; a = a->prox) {
            if (!visitado[a->destino]) {
                visitado[a->destino] = 1;
                nivel[a->destino] = nivel[u] + 1;
                pai[a->destino] = u;
                fila[fim++] = a->destino;
            }
        }
    }
    printf("\n\nMunicipio              Saltos  Caminho (menos cidades)\n");
    for (int i = 0; i < g->n; i++) {
        printf("%-22s %-7d ", NOMES[i], nivel[i]);
        if (nivel[i] >= 0) imprimir_caminho(pai, origem, i);
        else printf("inalcancavel");
        printf("\n");
    }
}

/* ---------- DFS ---------- */
static void dfs_rec(const Grafo *g, int u, int visitado[]) {
    visitado[u] = 1;
    printf("%s ", NOMES[u]);
    for (Aresta *a = g->adj[u]; a; a = a->prox)
        if (!visitado[a->destino]) dfs_rec(g, a->destino, visitado);
}

void dfs(const Grafo *g, int origem) {
    int visitado[NUM_VERTICES] = {0};
    printf("\n=== DFS a partir de %s ===\nOrdem de visita: ", NOMES[origem]);
    dfs_rec(g, origem, visitado);
    printf("\n");
}

/* ---------- Dijkstra (pesos nao negativos) ---------- */
void dijkstra(const Grafo *g, int origem, int destino) {
    int dist[NUM_VERTICES], pai[NUM_VERTICES], fechado[NUM_VERTICES] = {0};

    for (int i = 0; i < g->n; i++) { dist[i] = INF; pai[i] = -1; }
    dist[origem] = 0;

    for (int it = 0; it < g->n; it++) {
        int u = -1;
        for (int i = 0; i < g->n; i++)
            if (!fechado[i] && dist[i] != INF && (u == -1 || dist[i] < dist[u]))
                u = i;
        if (u == -1) break;
        fechado[u] = 1;

        for (Aresta *a = g->adj[u]; a; a = a->prox) {
            if (dist[u] + a->peso < dist[a->destino]) {
                dist[a->destino] = dist[u] + a->peso;
                pai[a->destino] = u;
            }
        }
    }

    printf("\n=== Dijkstra a partir de %s ===\n", NOMES[origem]);
    printf("Municipio              Dist(km)  Caminho\n");
    for (int i = 0; i < g->n; i++) {
        if (destino >= 0 && i != destino) continue;
        printf("%-22s %-9d ", NOMES[i], dist[i] == INF ? -1 : dist[i]);
        if (dist[i] != INF) imprimir_caminho(pai, origem, i);
        else printf("inalcancavel");
        printf("\n");
    }
}

/* ---------- Bellman-Ford (aceita pesos negativos, detecta ciclos) ---------- */
int bellman_ford(const Grafo *g, int origem, int destino) {
    int dist[NUM_VERTICES], pai[NUM_VERTICES];

    for (int i = 0; i < g->n; i++) { dist[i] = INF; pai[i] = -1; }
    dist[origem] = 0;

    /* V-1 passadas relaxando todas as arestas (nos dois sentidos) */
    for (int i = 0; i < g->n - 1; i++) {
        int mudou = 0;
        for (int e = 0; e < num_arestas; e++) {
            int u = ARESTAS[e].u, v = ARESTAS[e].v, w = ARESTAS[e].peso;
            if (dist[u] != INF && dist[u] + w < dist[v]) {
                dist[v] = dist[u] + w; pai[v] = u; mudou = 1;
            }
            if (dist[v] != INF && dist[v] + w < dist[u]) {
                dist[u] = dist[v] + w; pai[u] = v; mudou = 1;
            }
        }
        if (!mudou) break;   /* parada antecipada */
    }

    /* Verificacao de ciclo negativo */
    for (int e = 0; e < num_arestas; e++) {
        int u = ARESTAS[e].u, v = ARESTAS[e].v, w = ARESTAS[e].peso;
        if ((dist[u] != INF && dist[u] + w < dist[v]) ||
            (dist[v] != INF && dist[v] + w < dist[u])) {
            printf("\nCiclo de peso negativo detectado!\n");
            return 0;
        }
    }

    printf("\n=== Bellman-Ford a partir de %s ===\n", NOMES[origem]);
    printf("Municipio              Dist(km)  Caminho\n");
    for (int i = 0; i < g->n; i++) {
        if (destino >= 0 && i != destino) continue;
        printf("%-22s %-9d ", NOMES[i], dist[i] == INF ? -1 : dist[i]);
        if (dist[i] != INF) imprimir_caminho(pai, origem, i);
        else printf("inalcancavel");
        printf("\n");
    }
    return 1;
}

/* ---------- Interface ---------- */
static int ler_municipio(const char *msg) {
    int op;
    printf("\n%s\n", msg);
    for (int i = 0; i < NUM_VERTICES; i++) printf("  %2d - %s\n", i, NOMES[i]);
    printf("Opcao: ");
    if (scanf("%d", &op) != 1 || op < 0 || op >= NUM_VERTICES) {
        int c; while ((c = getchar()) != '\n' && c != EOF);
        return -1;
    }
    return op;
}

int main(void) {
    Grafo g;
    construir_mapa(&g);

    int opcao;
    do {
        printf("\n===== ROTAS ENTRE MUNICIPIOS DE MT =====\n"
               "1 - Mostrar grafo\n"
               "2 - BFS (menos saltos)\n"
               "3 - DFS\n"
               "4 - Dijkstra (origem -> todos ou destino)\n"
               "5 - Bellman-Ford (origem -> todos ou destino)\n"
               "6 - Comparar Dijkstra x Bellman-Ford\n"
               "0 - Sair\n"
               "Opcao: ");
        if (scanf("%d", &opcao) != 1) break;

        int o, d;
        switch (opcao) {
        case 1: imprimir_grafo(&g); break;
        case 2:
            if ((o = ler_municipio("Origem:")) >= 0) bfs(&g, o);
            break;
        case 3:
            if ((o = ler_municipio("Origem:")) >= 0) dfs(&g, o);
            break;
        case 4: case 5: case 6:
            if ((o = ler_municipio("Origem:")) < 0) break;
            d = ler_municipio("Destino:");   /* invalido => todos */
            if (opcao == 4 || opcao == 6) dijkstra(&g, o, d);
            if (opcao == 5 || opcao == 6) bellman_ford(&g, o, d);
            break;
        case 0: break;
        default: printf("Opcao invalida.\n");
        }
    } while (opcao != 0);

    grafo_free(&g);
    return 0;
}
