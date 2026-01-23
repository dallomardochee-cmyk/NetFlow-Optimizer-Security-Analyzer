#include <stdio.h>
#include <stdlib.h>
#include "graphe.h"

// Variables pour stocker la meilleure solution trouvée
float meilleure_latence = 1e9;
int meilleur_trajet[100];
int taille_meilleur = 0;
int trouve = 0;

void backtracking_recursive(Graphe* g, int u, int dest, float bp_min, float cout_max, 
                            float cout_acc, float lat_acc, int* visite, int* chemin, int etape) {
    
    // 1. ÉLAGAGE (Pruning) : Si la latence actuelle est déjà pire que le record, on arrête.
    // C'est ce qui rend l'algorithme "intelligent".
    if (lat_acc >= meilleure_latence || cout_acc > cout_max) return;

    // 2. CONDITION DE RÉUSSITE : On a atteint la destination
    if (u == dest) {
        trouve = 1;
        meilleure_latence = lat_acc;
        taille_meilleur = etape;
        for (int i = 0; i < etape; i++) meilleur_trajet[i] = chemin[i];
        return;
    }

    visite[u] = 1;

    // 3. EXPLORATION DES VOISINS
   for (Arete* a = g->noeuds[u].aretes; a; a = a->suivant) {
    int v = a->destination;

        // ÉLAGAGE AVANCÉ :
        // - Pas encore visité
        // - Bande passante suffisante
        // - Coût total respecté
        // - Nœud sécurisé (on exclut ici les niveaux de sécurité < 3)
        if (!visite[v] && a->bande_passante >= bp_min && (cout_acc + a->cout) <= cout_max && a->securite >= 3) {
            chemin[etape] = v;
            backtracking_recursive(g, v, dest, bp_min, cout_max, 
                                   cout_acc + a->cout, lat_acc + a->latence, 
                                   visite, chemin, etape + 1);
        }
    }

    // 4. BACKTRACK : On libère le nœud pour d'autres combinaisons
    visite[u] = 0;
}

void lancer_backtracking(Graphe* g, int src, int dest, float bp_min, float cout_max) {
    int* visite = calloc(g->nb_noeuds, sizeof(int));
    int* chemin = malloc(g->nb_noeuds * sizeof(int));
    
    // Reset des variables globales pour chaque nouvelle recherche
    trouve = 0;
    meilleure_latence = 1e9;
    taille_meilleur = 0;
    chemin[0] = src;

    printf("\n\033[1;36m[BACKTRACKING] Recherche du chemin optimal sous contraintes...\033[0m\n");

    backtracking_recursive(g, src, dest, bp_min, cout_max, 0.0, 0.0, visite, chemin, 1);

    if (trouve) {
        printf("\n\033[1;32m[SUCCES] Meilleur chemin trouve :\033[0m\n");
        printf("   Trajet : ");
        for (int i = 0; i < taille_meilleur; i++) {
            printf("%d%s", meilleur_trajet[i], (i == taille_meilleur - 1 ? "" : " -> "));
        }
        printf("\n   Latence Totale : %.2f ms | Cout : %.2f\n", meilleure_latence, cout_max);
    } else {
        printf("\n\033[1;31m[ECHEC]\033[0m Aucun chemin ne satisfait les contraintes (BP >= %.1f, Secu >= 3).\n", bp_min);
    }

    free(visite);
    free(chemin);
}