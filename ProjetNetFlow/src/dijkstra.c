#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "dijkstra.h"

// ========== GESTION DU TAS BINAIRE (MIN-HEAP) ==========
// --- Gestion du Tas Binaire (Min-Heap) ---
MinHeap* init_heap(int cap) {
    MinHeap* h = (MinHeap*)malloc(sizeof(MinHeap));
    h->data = (ElementHeap*)malloc(cap * sizeof(ElementHeap));
    h->size = 0;
    return h;
}

void swap(ElementHeap* a, ElementHeap* b) {
    ElementHeap t = *a;
    *a = *b;
    *b = t;
}

void push(MinHeap* h, int id, float d) {
    h->data[h->size] = (ElementHeap){id, d};
    int i = h->size++;
    while (i > 0 && h->data[i].dist < h->data[(i-1)/2].dist) {
        swap(&h->data[i], &h->data[(i-1)/2]);
        i = (i-1)/2;
    }
}

ElementHeap pop(MinHeap* h) {
    ElementHeap res = h->data[0];
    h->data[0] = h->data[--h->size];
    int i = 0;
    while (2*i+1 < h->size) {
        int j = 2*i+1;
        if (j+1 < h->size && h->data[j+1].dist < h->data[j].dist) j++;
        if (h->data[i].dist <= h->data[j].dist) break;
        swap(&h->data[i], &h->data[j]);
        i = j;
    }
    return res;
}

void afficher_chemin_rec(int* parent, int j) {
    if (j == -1) return; // Sécurité
    if (parent[j] == -1) {
        printf("%d", j);
        return;
    }
    afficher_chemin_rec(parent, parent[j]);
    printf(" -> %d", j);
// ========== ALGORITHME DIJKSTRA (PLUS COURT CHEMIN) ==========
}

// --- 1. Algorithme de Dijkstra (Renommé correctement) ---
void lancer_dijkstra(Graphe* g, int source, int destination) {
    float* dist = malloc(g->nb_noeuds * sizeof(float));
    int* parent = malloc(g->nb_noeuds * sizeof(int));
    
    clock_t start = clock();
    
    for (int i = 0; i < g->nb_noeuds; i++) {
        dist[i] = 1e9;
        parent[i] = -1;
    }
    dist[source] = 0;

    MinHeap* h = init_heap(g->nb_noeuds * 10); // Augmenter capacité pour sécurité
    push(h, source, 0);

    while (h->size > 0) {
        ElementHeap e = pop(h);
        int u = e.id;

        if (u == destination) break;
        if (e.dist > dist[u]) continue;

        for (Arete* a = g->noeuds[u].aretes; a; a = a->suivant) {
            int v = a->destination;
            if (dist[u] + a->latence < dist[v]) {
                dist[v] = dist[u] + a->latence;
                parent[v] = u;
                push(h, v, dist[v]);
            }
        }
    }

    clock_t end = clock();
    double temps_ms = ((double)(end - start) / CLOCKS_PER_SEC) * 1000.0;

    printf("\n\033[1;32m[PERFORMANCE DIJKSTRA]\033[0m");
    printf("\n>> Temps CPU : \033[1;33m%.5f ms\033[0m", temps_ms);
    
    if (dist[destination] >= 1e9) {
        printf("\n>> Statut : Destination [%d] inaccessible.\n", destination);
    } else {
        printf("\n>> Chemin : "); 
        afficher_chemin_rec(parent, destination);
        printf("\n>> Latence totale : %.2f ms\n", dist[destination]);
    }
// ========== ALGORITHME BELLMAN-FORD (DETECTION CYCLES) ==========

    free(dist); free(parent); free(h->data); free(h);
}

// --- 2. Algorithme de Bellman-Ford ---
void bellman_ford(Graphe* g, int source, int dest) {
    if (!g || source < 0 || source >= g->nb_noeuds) return;

    clock_t start = clock();
    float* dist = (float*)malloc(g->nb_noeuds * sizeof(float));
    int* parent = (int*)malloc(g->nb_noeuds * sizeof(int));

    for (int i = 0; i < g->nb_noeuds; i++) {
        dist[i] = 1e9;
        parent[i] = -1;
    }
    dist[source] = 0;

    // Relaxation V-1 fois
    for (int i = 1; i < g->nb_noeuds; i++) {
        int changement = 0;
        for (int u = 0; u < g->nb_noeuds; u++) {
            if (dist[u] == 1e9) continue;
            for (Arete* a = g->noeuds[u].aretes; a; a = a->suivant) {
                if (dist[u] + a->latence < dist[a->destination]) {
                    dist[a->destination] = dist[u] + a->latence;
                    parent[a->destination] = u;
                    changement = 1;
                }
            }
        }
        if (!changement) break;
    }

    // Détection cycles négatifs
    int cycle_negatif = 0;
    for (int u = 0; u < g->nb_noeuds; u++) {
        if (dist[u] == 1e9) continue;
        for (Arete* a = g->noeuds[u].aretes; a; a = a->suivant) {
            if (dist[u] + a->latence < dist[a->destination]) {
                cycle_negatif = 1;
                break;
            }
        }
    }

    clock_t end = clock();
    double temps_ms = ((double)(end - start) / CLOCKS_PER_SEC) * 1000.0;

    printf("\n\033[1;32m[PERFORMANCE BELLMAN-FORD]\033[0m");
    printf("\n>> Temps CPU : \033[1;33m%.5f ms\033[0m", temps_ms);

    if (cycle_negatif) {
        printf("\n\033[1;31m>> STATUT : ALERTE ! Cycle de poids negatif detecte.\033[0m");
    } else if (dist[dest] == 1e9) {
        printf("\n>> Destination %d non atteinte.\n", dest);
    } else {
        printf("\n>> Chemin : ");
        afficher_chemin_rec(parent, dest);
        printf("\n>> Latence totale : %.2f ms\n", dist[dest]);
    }

    free(dist); free(parent);
}