#include <stdio.h>
#include <stdlib.h>
#include <time.h>      // Pour la portabilité du chrono
#include "graphe.h"
#include "securite.h"

// ========== ALGORITHME DE TARJAN (DFS) ==========
// Algorithme de Tarjan pour trouver les points d'articulation et les ponts
void dfs_articulation(Graphe* g, int u, int v[], int d[], int l[], int p[], int* t, int art[]) {
    int enfants = 0;
    v[u] = 1;
    d[u] = l[u] = ++(*t);

    // Correction : tab_noeuds -> noeuds | liste_adjacence -> aretes
    Arete* a = g->noeuds[u].aretes;
    
    while (a) {
        int destination = a->destination;
        if (!v[destination]) {
            enfants++;
            p[destination] = u;
            dfs_articulation(g, destination, v, d, l, p, t, art);

            // Mise à jour du "low link"
            if (l[destination] < l[u]) l[u] = l[destination];

            // --- DETECTION DES PONTS (Liaisons critiques) ---
            if (l[destination] > d[u]) {
                printf("\033[1;33m[ALERTE PONT]\033[0m Liaison critique : %d <-> %d (Coupure fatale)\n", u, destination);
            }

            // --- DETECTION DES POINTS D'ARTICULATION ---
            if (p[u] == -1 && enfants > 1) art[u] = 1; // Racine avec >1 fils
            if (p[u] != -1 && l[destination] >= d[u]) art[u] = 1; // Noeud interne
        } 
        else if (destination != p[u]) {
            // Mise à jour pour les arêtes de retour (Back-edges)
            if (d[destination] < l[u]) l[u] = d[destination];
        }
// ========== FONCTION PRINCIPALE D'AUDIT ==========
        a = a->suivant; // INDISPENSABLE : Ne pas oublier d'avancer dans la liste !
    }
}

void analyser_securite(Graphe* g) {
    if (!g) return;

    clock_t start = clock();

    int n = g->nb_noeuds;
    int *v = calloc(n, sizeof(int));
    int *d = calloc(n, sizeof(int));
    int *l = calloc(n, sizeof(int));
    int *p = malloc(n * sizeof(int));
    int *art = calloc(n, sizeof(int));
    int t = 0;

    for (int i = 0; i < n; i++) p[i] = -1;

    printf("\n\033[1;31m[AUDIT DE FIABILITE - ALGORITHME DE TARJAN]\033[0m\n");

    for (int i = 0; i < n; i++) {
        if (!v[i]) dfs_articulation(g, i, v, d, l, p, &t, art);
    }

    clock_t end = clock();
    double temps_ms = ((double)(end - start) / CLOCKS_PER_SEC) * 1000.0;

    printf(">> Temps d'analyse : \033[1;33m%.5f ms\033[0m", temps_ms);
    
    int count = 0;
    for (int i = 0; i < n; i++) {
        if (art[i]) {
            printf("\n\033[1;31m[ALERTE SPOF]\033[0m Noeud %d est un point critique (Routeur pivot).", i);
            count++;
        }
    }

    if (count == 0) {
        printf("\n>> Statut : \033[1;32mReseau hautement resilient\033[0m (Maillage optimal).");
    } else {
        printf("\n>> Statut : \033[1;31mReseau vulnerable\033[0m (%d routeurs critiques detectes).", count);
    }
    printf("\n---------------------------------------------\n");

    free(v); free(d); free(l); free(p); free(art);
}