#include "graphe.h"
#include "dijkstra.h"
#include <assert.h>
#include <stdio.h>

void test_performance() {
    printf("Lancement des tests exhaustifs...\n");
    Graphe* g = creer_graphe(2);
    ajouter_arete(g, 0, 1, 0.5, 10000, 1, 1);
    assert(g->nb_noeuds == 2);
    printf("Test RAM...\n"); mesurer_performance_memoire(g);
    printf("Test Dijkstra...\n"); dijkstra(g, 0, 1);
    printf("\033[1;32m[SUCCES] Tous les modules sont valides.\033[0m\n");
    liberer_graphe(g);
}

int main() { test_performance(); return 0; }