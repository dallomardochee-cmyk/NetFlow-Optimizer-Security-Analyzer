#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include "securite.h"

// Algorithme récursif pour trouver les points d'articulation (Tarjan)
void dfs_articulation(Graphe* g, int u, int v[], int d[], int l[], int p[], int* t, int art[]) {
    int enfants = 0;
    v[u] = 1;
    d[u] = l[u] = ++(*t);

    Arete* a = g->tab_noeuds[u].liste_adjacence;
    while (a) {
        int destination = a->destination;
        if (!v[destination]) {
            enfants++;
            p[destination] = u;
            dfs_articulation(g, destination, v, d, l, p, t, art);

            // Vérifie si le sous-arbre de 'destination' a une connexion vers un ancêtre de 'u'
            if (l[destination] < l[u]) l[u] = l[destination];

            // Cas 1 : u est la racine et a plus d'un enfant
            if (p[u] == -1 && enfants > 1) art[u] = 1;
            // Cas 2 : u n'est pas la racine et low value de l'enfant >= discovery value de u
            if (p[u] != -1 && l[destination] >= d[u]) art[u] = 1;
        } 
        else if (destination != p[u]) {
            if (d[destination] < l[u]) l[u] = d[destination];
        }
        a = a->suivant;
    }
}
void analyser_securite(Graphe* g) {
    // Mesure Haute Précision
    LARGE_INTEGER frequency, start, end;
    QueryPerformanceFrequency(&frequency);
    QueryPerformanceCounter(&start);

    int n = g->nb_noeuds;
    int *v = calloc(n, sizeof(int));
    int *d = calloc(n, sizeof(int));
    int *l = calloc(n, sizeof(int));
    int *p = malloc(n * sizeof(int));
    int *art = calloc(n, sizeof(int));
    int t = 0;

    for (int i = 0; i < n; i++) p[i] = -1;

    // Lancement du DFS sur tous les composants du réseau
    for (int i = 0; i < n; i++) {
        if (!v[i]) dfs_articulation(g, i, v, d, l, p, &t, art);
    }

    QueryPerformanceCounter(&end);
    double temps_ms = (double)(end.QuadPart - start.QuadPart) * 1000.0 / (double)frequency.QuadPart;

    printf("\n\033[1;31m[AUDIT DE FIABILITE - DFS]\033[0m");
    printf("\n>> Temps d'analyse : \033[1;33m%.5f ms\033[0m", temps_ms);
    
    int count = 0;
    for (int i = 0; i < n; i++) {
        if (art[i]) {
            printf("\n\033[1;31m[ALERTE SPOF]\033[0m Noeud %d est un point critique de defaillance.", i);
            count++;
        }
    }

    if (count == 0) {
        printf("\n>> Statut : Reseau hautement resilient (aucun point critique).");
    } else {
        printf("\n>> Total : %d vulnerabilite(s) detectee(s).", count);
    }
    printf("\n");

    free(v); free(d); free(l); free(p); free(art);
}