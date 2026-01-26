#include <stdio.h>
#include <stdlib.h>
#include "graphe.h"
#include "backtracking.h"

// Variables pour stocker la meilleure solution trouvée
// ========== VARIABLES GLOBALES ==========
float meilleure_latence = 1e9;
int meilleur_trajet[100];
int taille_meilleur = 0;
int trouve = 0;

// Ajout d'une variable globale pour le rapport
long nb_iterations = 0;
float meilleur_cout = 0; // Pour afficher le vrai coût final

void backtracking_recursive(Graphe* g, int u, int dest, float bp_min, float cout_max, 
                            int sec_min, float cout_acc, float lat_acc, 
                            int* visite, int* chemin, int etape) {
    nb_iterations++;

    // 1. ÉLAGAGE (Pruning)
    // On coupe si latence déjà moins bonne OU si on dépasse le budget
    if (lat_acc >= meilleure_latence || cout_acc > cout_max) return;

    // 2. CONDITION DE RÉUSSITE
    if (u == dest) {
        trouve = 1;
        meilleure_latence = lat_acc;
        meilleur_cout = cout_acc; // On stocke le coût réel
        taille_meilleur = etape;
        for (int i = 0; i < etape; i++) meilleur_trajet[i] = chemin[i];
        return;
    }

    visite[u] = 1;

    // 3. EXPLORATION
    for (Arete* a = g->noeuds[u].aretes; a; a = a->suivant) {
        int v = a->destination;

        // On remplace le '3' fixe par 'sec_min'
        if (!visite[v] && a->bande_passante >= bp_min && 
            (cout_acc + a->cout) <= cout_max && a->securite >= sec_min) {
            
            chemin[etape] = v;
            backtracking_recursive(g, v, dest, bp_min, cout_max, sec_min,
                                   cout_acc + a->cout, lat_acc + a->latence, 
                                   visite, chemin, etape + 1);
        }
    }

    // 4. BACKTRACK
    visite[u] = 0;
}

// ========== FONCTION PRINCIPALE DE BACKTRACKING ==========
void lancer_backtracking(Graphe* g, int src, int dest, float bp_min, float cout_max, int sec_min) {
    if (g == NULL || src < 0 || src >= g->nb_noeuds) return;

    int* visite = calloc(g->nb_noeuds, sizeof(int));
    int* chemin = malloc(g->nb_noeuds * sizeof(int));
    
    nb_iterations = 0;
    trouve = 0;
    meilleure_latence = 1e9;
    chemin[0] = src;

    backtracking_recursive(g, src, dest, bp_min, cout_max, sec_min, 0.0, 0.0, visite, chemin, 1);

    if (trouve) {
        printf("[SUCCES] Itérations : %ld\n", nb_iterations);
        printf("Trajet : ");
        for (int i = 0; i < taille_meilleur; i++) 
            printf("%d%s", meilleur_trajet[i], (i == taille_meilleur-1 ? "" : " -> "));
        printf("\nLatence : %.2f ms | Coût Réel : %.2f (Budget : %.2f)\n", meilleure_latence, meilleur_cout, cout_max);
    } else {
       printf("[ECHEC] Aucun chemin (Secu >= %d, BP >= %.1f, Budget <= %.1f)\n", sec_min, bp_min, cout_max);
    }

    free(visite);
    free(chemin);
}