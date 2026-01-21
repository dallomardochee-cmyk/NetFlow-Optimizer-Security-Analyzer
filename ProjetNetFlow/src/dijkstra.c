#include <windows.h> 
#include <stdio.h>
#include <stdlib.h>
#include "dijkstra.h"

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

// Fonction pour remonter le chemin 
void afficher_chemin_rec(int* parent, int j) {
    if (parent[j] == -1) {
        printf("%d", j);
        return;
    }
    afficher_chemin_rec(parent, parent[j]);
    printf(" -> %d", j);
}

// --- Algorithme de Dijkstra (Haute Précision) ---
void lancer_dijkstra(Graphe* g, int src, int dest) {
    LARGE_INTEGER frequency;
    LARGE_INTEGER start, end;
    
    // Initialisation du compteur haute performance
    QueryPerformanceFrequency(&frequency);
    QueryPerformanceCounter(&start);

    // Initialisation des distances
    float* dist = (float*)malloc(g->nb_noeuds * sizeof(float));
    int* parent = (int*)malloc(g->nb_noeuds * sizeof(int));
    for (int i = 0; i < g->nb_noeuds; i++) {
        dist[i] = 1e9;
        parent[i] = -1;
    }
    dist[src] = 0;


    MinHeap* h = init_heap(g->nb_noeuds * 4);
    push(h, src, 0);

    while (h->size > 0) {
        ElementHeap e = pop(h);
        int u = e.id;
        if (u == dest) break;

        for (Arete* a = g->tab_noeuds[u].liste_adjacence; a; a = a->suivant) {
            if (dist[u] + a->latence < dist[a->destination]) {
                dist[a->destination] = dist[u] + a->latence;
                parent[a->destination] = u;
                push(h, a->destination, dist[a->destination]);
            }
        }
    }

    QueryPerformanceCounter(&end);
    
    // Calcul du temps en millisecondes avec les QuadPart (entiers 64 bits)
    double temps_ms = (double)(end.QuadPart - start.QuadPart) * 1000.0 / (double)frequency.QuadPart;

    printf("\n\033[1;32m[PERFORMANCE DIJKSTRA]\033[0m");
    printf("\n>> Temps CPU : \033[1;33m%.5f ms\033[0m", temps_ms);
    
    if (dist[dest] >= 1e9) {
        printf("\n>> Statut : Destination inaccessible.\n");
    } else {
        printf("\n>> Chemin : "); 
        afficher_chemin_rec(parent, dest);
        printf("\n>> Cout minimal (Latence) : %.2f ms\n", dist[dest]);
    }

    free(dist);
    free(parent); 
    free(h->data);
    free(h);
}

// --- Algorithme de Bellman-Ford (Haute Précision) ---
void bellman_ford(Graphe* g, int src) {
    LARGE_INTEGER frequency;
    LARGE_INTEGER start, end;
    
    QueryPerformanceFrequency(&frequency);
    QueryPerformanceCounter(&start);

    float* dist = (float*)malloc(g->nb_noeuds * sizeof(float));
    for (int i = 0; i < g->nb_noeuds; i++) dist[i] = 1e9;
    dist[src] = 0;

    for (int i = 1; i < g->nb_noeuds; i++) {
        for (int u = 0; u < g->nb_noeuds; u++) {
            for (Arete* a = g->tab_noeuds[u].liste_adjacence; a; a = a->suivant) {
                if (dist[u] != 1e9 && dist[u] + a->latence < dist[a->destination])
                    dist[a->destination] = dist[u] + a->latence;
            }
        }
    }

    QueryPerformanceCounter(&end);
    double temps_ms = (double)(end.QuadPart - start.QuadPart) * 1000.0 / (double)frequency.QuadPart;

    printf("\n\033[1;32m[PERFORMANCE BELLMAN-FORD]\033[0m");
    printf("\n>> Temps CPU : \033[1;33m%.5f ms\033[0m", temps_ms);
    printf("\n>> Statut : Verification terminee.");
    printf("\n>> Info : Toutes les routes depuis le noeud %d ont ete calculees.", src);
    free(dist);
}