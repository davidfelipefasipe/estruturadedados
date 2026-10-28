/*
 * Grafo de rotas entre cidades (ponderado, não direcionado)
 * - Representação: lista de adjacências
 * - Menor caminho em km: Dijkstra
 * - Menos paradas: BFS
 *
 * Compilar: gcc -Wall -Wextra -o grafo_rotas grafo_rotas.c
 * Executar: ./grafo_rotas
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>

#define MAX_CIDADES 64
#define TAM_NOME    40
#define INF         INT_MAX

typedef struct Aresta {
    int destino;
    int km;
    struct Aresta *prox;
} Aresta;

typedef struct {
    char nome[TAM_NOME];
    Aresta *adj;
} Cidade;

typedef struct {
    Cidade cidades[MAX_CIDADES];
    int n;
} Grafo;

/* ---------- utilitários ---------- */

static int iguais(const char *a, const char *b) {
    while (*a && *b) {
        if (tolower((unsigned char)*a) != tolower((unsigned char)*b)) return 0;
        a++; b++;
    }
    return *a == *b;
}

static void ler_linha(const char *prompt, char *buf, size_t tam) {
    printf("%s", prompt);
    if (!fgets(buf, (int)tam, stdin)) { buf[0] = '\0'; return; }
    buf[strcspn(buf, "\r\n")] = '\0';
}

static int ler_int(const char *prompt) {
    char buf[32];
    ler_linha(prompt, buf, sizeof buf);
    return atoi(buf);
}

/* ---------- operações do grafo ---------- */

static int buscar_cidade(const Grafo *g, const char *nome) {
    for (int i = 0; i < g->n; i++)
        if (iguais(g->cidades[i].nome, nome)) return i;
    return -1;
}

static int adicionar_cidade(Grafo *g, const char *nome) {
    if (g->n >= MAX_CIDADES) { puts("Limite de cidades atingido."); return -1; }
    if (buscar_cidade(g, nome) != -1) { puts("Cidade já existe."); return -1; }
    strncpy(g->cidades[g->n].nome, nome, TAM_NOME - 1);
    g->cidades[g->n].nome[TAM_NOME - 1] = '\0';
    g->cidades[g->n].adj = NULL;
    return g->n++;
}

static void inserir_aresta(Grafo *g, int de, int para, int km) {
    /* se a rota já existe, apenas atualiza a distância */
    for (Aresta *a = g->cidades[de].adj; a; a = a->prox) {
        if (a->destino == para) { a->km = km; return; }
    }
    Aresta *nova = malloc(sizeof *nova);
    if (!nova) { perror("malloc"); exit(EXIT_FAILURE); }
    nova->destino = para;
    nova->km = km;
    nova->prox = g->cidades[de].adj;
    g->cidades[de].adj = nova;
}

static int adicionar_rota(Grafo *g, const char *a, const char *b, int km) {
    int i = buscar_cidade(g, a), j = buscar_cidade(g, b);
    if (i == -1 || j == -1) { puts("Cidade não encontrada."); return 0; }
    if (i == j)             { puts("Origem e destino iguais."); return 0; }
    if (km <= 0)            { puts("Distância deve ser positiva."); return 0; }
    inserir_aresta(g, i, j, km);
    inserir_aresta(g, j, i, km);   /* rota de mão dupla */
    return 1;
}

static void listar(const Grafo *g) {
    if (g->n == 0) { puts("Nenhuma cidade cadastrada."); return; }
    for (int i = 0; i < g->n; i++) {
        printf("%s:\n", g->cidades[i].nome);
        if (!g->cidades[i].adj) puts("   (sem rotas)");
        for (Aresta *a = g->cidades[i].adj; a; a = a->prox)
            printf("   -> %s (%d km)\n", g->cidades[a->destino].nome, a->km);
    }
}

static void liberar(Grafo *g) {
    for (int i = 0; i < g->n; i++) {
        Aresta *a = g->cidades[i].adj;
        while (a) { Aresta *t = a->prox; free(a); a = t; }
        g->cidades[i].adj = NULL;
    }
    g->n = 0;
}

/* ---------- algoritmos ---------- */

static void imprimir_caminho(const Grafo *g, const int prev[], int destino) {
    int pilha[MAX_CIDADES], topo = 0;
    for (int v = destino; v != -1; v = prev[v]) pilha[topo++] = v;
    while (topo--) {
        printf("%s", g->cidades[pilha[topo]].nome);
        if (topo) printf(" -> ");
    }
    putchar('\n');
}

/* Dijkstra O(V^2): menor distância total em km */
static void menor_rota_km(const Grafo *g, int origem, int destino) {
    int dist[MAX_CIDADES], prev[MAX_CIDADES], visitado[MAX_CIDADES] = {0};
    for (int i = 0; i < g->n; i++) { dist[i] = INF; prev[i] = -1; }
    dist[origem] = 0;

    for (int k = 0; k < g->n; k++) {
        int u = -1;
        for (int i = 0; i < g->n; i++)
            if (!visitado[i] && dist[i] != INF && (u == -1 || dist[i] < dist[u])) u = i;
        if (u == -1 || u == destino) break;
        visitado[u] = 1;
        for (Aresta *a = g->cidades[u].adj; a; a = a->prox) {
            if (dist[u] + a->km < dist[a->destino]) {
                dist[a->destino] = dist[u] + a->km;
                prev[a->destino] = u;
            }
        }
    }

    if (dist[destino] == INF) { puts("Não há rota entre essas cidades."); return; }
    printf("Menor distância: %d km\nRota: ", dist[destino]);
    imprimir_caminho(g, prev, destino);
}

/* BFS: menor número de trechos (ignora a distância) */
static void menos_paradas(const Grafo *g, int origem, int destino) {
    int prev[MAX_CIDADES], visitado[MAX_CIDADES] = {0};
    int fila[MAX_CIDADES], ini = 0, fim = 0;
    for (int i = 0; i < g->n; i++) prev[i] = -1;

    visitado[origem] = 1;
    fila[fim++] = origem;
    while (ini < fim) {
        int u = fila[ini++];
        if (u == destino) break;
        for (Aresta *a = g->cidades[u].adj; a; a = a->prox) {
            if (!visitado[a->destino]) {
                visitado[a->destino] = 1;
                prev[a->destino] = u;
                fila[fim++] = a->destino;
            }
        }
    }

    if (!visitado[destino]) { puts("Não há rota entre essas cidades."); return; }

    int trechos = 0, total = 0;
    for (int v = destino; prev[v] != -1; v = prev[v]) {
        trechos++;
        for (Aresta *a = g->cidades[prev[v]].adj; a; a = a->prox)
            if (a->destino == v) { total += a->km; break; }
    }
    printf("Menor número de trechos: %d (%d km no total)\nRota: ", trechos, total);
    imprimir_caminho(g, prev, destino);
}

/* ---------- dados de exemplo (distâncias aproximadas, apenas ilustrativas) ---------- */

static void carregar_exemplo(Grafo *g) {
    const char *nomes[] = {"Sorriso", "Sinop", "Lucas do Rio Verde", "Nova Mutum",
                           "Cuiaba", "Diamantino", "Tangara da Serra", "Rondonopolis"};
    for (size_t i = 0; i < sizeof nomes / sizeof *nomes; i++) adicionar_cidade(g, nomes[i]);

    adicionar_rota(g, "Sorriso", "Sinop", 85);
    adicionar_rota(g, "Sorriso", "Lucas do Rio Verde", 60);
    adicionar_rota(g, "Lucas do Rio Verde", "Nova Mutum", 55);
    adicionar_rota(g, "Nova Mutum", "Cuiaba", 240);
    adicionar_rota(g, "Nova Mutum", "Diamantino", 110);
    adicionar_rota(g, "Diamantino", "Cuiaba", 190);
    adicionar_rota(g, "Diamantino", "Tangara da Serra", 135);
    adicionar_rota(g, "Tangara da Serra", "Cuiaba", 240);
    adicionar_rota(g, "Cuiaba", "Rondonopolis", 215);
}

/* ---------- menu ---------- */

static void menu(void) {
    puts("\n=== Rotas entre cidades ===");
    puts("1) Listar cidades e rotas");
    puts("2) Adicionar cidade");
    puts("3) Adicionar rota");
    puts("4) Menor rota em km (Dijkstra)");
    puts("5) Rota com menos trechos (BFS)");
    puts("0) Sair");
}

int main(void) {
    Grafo g = { .n = 0 };
    carregar_exemplo(&g);

    int op;
    char a[TAM_NOME], b[TAM_NOME];
    do {
        menu();
        op = ler_int("Opção: ");
        switch (op) {
        case 1:
            listar(&g);
            break;
        case 2:
            ler_linha("Nome da cidade: ", a, sizeof a);
            if (a[0] && adicionar_cidade(&g, a) != -1) puts("Cidade adicionada.");
            break;
        case 3:
            ler_linha("Cidade A: ", a, sizeof a);
            ler_linha("Cidade B: ", b, sizeof b);
            if (adicionar_rota(&g, a, b, ler_int("Distância (km): "))) puts("Rota adicionada.");
            break;
        case 4:
        case 5: {
            ler_linha("Origem: ", a, sizeof a);
            ler_linha("Destino: ", b, sizeof b);
            int i = buscar_cidade(&g, a), j = buscar_cidade(&g, b);
            if (i == -1 || j == -1) { puts("Cidade não encontrada."); break; }
            if (op == 4) menor_rota_km(&g, i, j);
            else         menos_paradas(&g, i, j);
            break;
        }
        case 0:
            puts("Até logo!");
            break;
        default:
            puts("Opção inválida.");
        }
    } while (op != 0);

    liberar(&g);
    return 0;
}
