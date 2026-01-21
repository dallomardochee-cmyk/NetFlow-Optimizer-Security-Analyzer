#include <stdio.h>
#include <stdlib.h>
#include "graphe.h"

// Variable pour savoir si on a trouve au moins une solution
int trouve = 0;

// Fonction de recherche récursive
void backtracking_recursive(Graphe* g, int u, int dest, float bp_min, float cout_max, 
                            float cout_accumule, int* visite, int* chemin, int etape) {
    
    // 1. SECURITÉ : Si on a déjà trouvé un chemin ou si c'est trop profond, on arrête
    if (trouve || etape >= g->nb_noeuds) return;

    // 2. CONDITION DE RÉUSSITE : On est arrivé au noeud destination
    if (u == dest) {
        trouve = 1;
        printf("\n\033[1;32m[SUCCES] Chemin trouve sous contraintes :\033[0m\n");
        printf("   Trajet : ");
        for (int i = 0; i < etape; i++) {
            printf("%d%s", chemin[i], (i == etape - 1 ? "" : " -> "));
        }
        printf("\n   Cout total : %.2f | BP min respectee : %.2f\n", cout_accumule, bp_min);
        return;
    }

    // 3. MARQUAGE : On marque le noeud actuel comme visité pour ce chemin
    visite[u] = 1;

    // 4. EXPLORATION des voisins
    for (Arete* a = g->tab_noeuds[u].liste_adjacence; a; a = a->suivant) {
        int v = a->destination;

        // ÉLAGAGE : On n'y va que si :
        // - Pas encore visité (évite les boucles infinies)
        // - La BP est suffisante
        // - Le coût cumulé ne dépasse pas la limite
        if (!visite[v] && a->bande_passante >= bp_min && (cout_accumule + a->cout) <= cout_max) {
            chemin[etape] = v;
            backtracking_recursive(g, v, dest, bp_min, cout_max, cout_accumule + a->cout, visite, chemin, etape + 1);
        }
    }

    // 5. BACKTRACKING : On libère le noeud pour permettre d'autres explorations
    visite[u] = 0;
}

// Fonction principale appelée par le main
void lancer_backtracking(Graphe* g, int src, int dest, float bp_min, float cout_max) {
    int* visite = calloc(g->nb_noeuds, sizeof(int));
    int* chemin = malloc(g->nb_noeuds * sizeof(int));
    
    trouve = 0; // Reset de la variable globale
    chemin[0] = src; // Le premier pas est la source

    backtracking_recursive(g, src, dest, bp_min, cout_max, 0.0, visite, chemin, 1);

    if (!trouve) {
        printf("\n\033[1;31m[ECHEC]\033[0m Aucun chemin ne respecte ces contraintes (BP >= %.1f, Cout <= %.1f).\n", bp_min, cout_max);
    }

    free(visite);
    free(chemin);
}