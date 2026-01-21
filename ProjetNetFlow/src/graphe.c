#include "graphe.h"
#include <stdlib.h>
#include <string.h>


Graphe* creer_graphe(int nb_noeuds) {
    Graphe* g = (Graphe*)malloc(sizeof(Graphe));
    g->nb_noeuds = nb_noeuds;
    g->tab_noeuds = (Noeud*)malloc(nb_noeuds * sizeof(Noeud));
    for (int i = 0; i < nb_noeuds; i++) {
        g->tab_noeuds[i].liste_adjacence = NULL;
    }
    return g;
}

void ajouter_arete(Graphe* g, int src, int dest, float lat, float bp, float cout, int sec) {
    // 1. On crée le lien Aller (ex: 40 -> 99)
    Arete* nouvelle_aller = creer_arete(dest, lat, bp, cout, sec);
    inserer_en_tete(&(g->tab_noeuds[src].liste_adjacence), nouvelle_aller);

    // 2. On crée le lien Retour (ex: 99 -> 40) systématiquement
    Arete* nouvelle_retour = creer_arete(src, lat, bp, cout, sec);
    inserer_en_tete(&(g->tab_noeuds[dest].liste_adjacence), nouvelle_retour);
}

Graphe* charger_depuis_fichier(const char* nom_fichier) {
    FILE* f = fopen(nom_fichier, "r");
    if (!f) return NULL;

    char ligne[256];
    int nb_noeuds = 0;
    Graphe* g = NULL;

    while (fgets(ligne, sizeof(ligne), f)) {
        if (ligne[0] == '#' || ligne[0] == '\n') continue; // Ignore commentaires

        if (strncmp(ligne, "NB_NOEUDS", 9) == 0) {
            sscanf(ligne, "NB_NOEUDS %d", &nb_noeuds);
            g = creer_graphe(nb_noeuds);
        } 
        else if (strncmp(ligne, "ARETE", 5) == 0) {
            int s, d, sec;
            float lat, bp, ct;
            sscanf(ligne, "ARETE %d %d %f %f %f %d", &s, &d, &lat, &bp, &ct, &sec);
            if (g) ajouter_arete(g, s, d, lat, bp, ct, sec);
        }
        // On ignore les lignes "NOEUD" car ton graphe utilise les indices (0-99)
    }

    fclose(f);
    return g;
}

void afficher_graphe_complet(Graphe* g) {
    if (!g) {
        printf("\n[ERREUR] Aucun graphe a afficher.\n");
        return;
    }

    printf("\n==============================================================\n");
    printf("          AFFICHAGE INTEGRAL DE LA TOPOLOGIE RESEAU           \n");
    printf("==============================================================\n");

    for (int i = 0; i < g->nb_noeuds; i++) {
        printf("Noeud [%02d] | Adjacences: ", i);
        
        Arete* a = g->tab_noeuds[i].liste_adjacence;
        if (a == NULL) {
            printf("(Aucune connexion)");
        } else {
            while (a) {
                printf("[%d](L:%.1f, BP:%.0f)", a->destination, a->latence, a->bande_passante);
                if (a->suivant) printf(" -> ");
                a = a->suivant;
            }
        }
        printf("\n");
        
        if ((i + 1) % 20 == 0 && i != g->nb_noeuds - 1) {
            printf("\n--- Appuyez sur Entree pour la suite ---");
            getchar(); getchar(); 
        }
    }
}

void analyser_connectivite(Graphe* g, int depart) {
    if (!g || depart < 0 || depart >= g->nb_noeuds) {
        printf("\033[1;31mErreur : Parametres d'analyse invalides.\033[0m\n");
        return;
    }
    
    int* visite = (int*)calloc(g->nb_noeuds, sizeof(int));
    int* file = (int*)malloc(g->nb_noeuds * sizeof(int));
    int tete = 0, queue = 0;

    // Exploration BFS
    visite[depart] = 1;
    file[queue++] = depart;

    while (tete < queue) {
        int actuel = file[tete++];
        Arete* a = g->tab_noeuds[actuel].liste_adjacence;
        while (a) {
            if (!visite[a->destination]) {
                visite[a->destination] = 1;
                file[queue++] = a->destination;
            }
            a = a->suivant;
        }
    }

    // --- AFFICHAGE DES RÉSULTATS ---
    int nb_atteints = queue;
    printf("\n\033[1;36m================================================\033[0m");
    printf("\n   \033[1mRESULTAT DE L'EXPLORATION DEPUIS LE NOEUD %d\033[0m", depart);
    printf("\n\033[1;36m================================================\033[0m\n");

    // 1. Affichage des Noeuds ACCESSIBLES
    printf("\033[1;32m[+] NOEUDS ACCESSIBLES (%d/%d) :\033[0m\n", nb_atteints, g->nb_noeuds);
    int col = 0;
    for (int i = 0; i < g->nb_noeuds; i++) {
        if (visite[i]) {
            printf("%2d ", i);
            col++;
            if (col % 15 == 0) printf("\n"); // Retour à la ligne pour la lisibilité
        }
    }
    printf("\n");

    // 2. Affichage des Noeuds INACCESSIBLES (si le réseau est coupé)
    if (nb_atteints < g->nb_noeuds) {
        printf("\n\033[1;31m[-] NOEUDS INACCESSIBLES (%d/%d) :\033[0m\n", g->nb_noeuds - nb_atteints, g->nb_noeuds);
        col = 0;
        for (int i = 0; i < g->nb_noeuds; i++) {
            if (!visite[i]) {
                printf("%2d ", i);
                col++;
                if (col % 15 == 0) printf("\n");
            }
        }
        printf("\n\n\033[1;33m[!] ATTENTION : Le reseau est fragmente.\033[0m\n");
    } else {
        printf("\n\033[1;32m[OK] Connectivite totale : Tous les noeuds sont joignables.\033[0m\n");
    }
    printf("\033[1;36m================================================\033[0m\n");

    free(visite);
    free(file);
}
void analyser_topologie(Graphe* g) {
    float lat_totale = 0;
    int nb_links = 0;
    for (int i = 0; i < g->nb_noeuds; i++) {
        Arete* a = g->tab_noeuds[i].liste_adjacence;
        while (a) {
            lat_totale += a->latence;
            nb_links++;
            a = a->suivant;
        }
    }
    float moyenne = (nb_links > 0) ? lat_totale / nb_links : 0;
    printf("\n\033[1;34m--- ANALYSE TOPOLOGIQUE ---\033[0m\n");
    if (moyenne < 1.0) printf("Detection : DATACENTER\n");
    else printf("Detection : WAN / IoT\n");
}

void mesurer_performance_memoire(Graphe* g) {
    size_t sz = sizeof(Graphe) + (g->nb_noeuds * sizeof(Noeud));
    for (int i = 0; i < g->nb_noeuds; i++) {
        Arete* a = g->tab_noeuds[i].liste_adjacence;
        while (a) { sz += sizeof(Arete); a = a->suivant; }
    }
    printf("\n\033[1;35m--- PERFORMANCE RAM ---\033[0m\n");
    printf("Occupation : %.2f Ko\n", (float)sz / 1024);
}

void liberer_graphe(Graphe* g) {
    for (int i = 0; i < g->nb_noeuds; i++) {
        liberer_liste(g->tab_noeuds[i].liste_adjacence);
    }
    free(g->tab_noeuds);
    free(g);
}

void sauvegarder_graphe(Graphe* g, const char* nom_fichier) {
    FILE* f = fopen(nom_fichier, "w");
    if (!f) { printf("Erreur sauvegarde.\n"); return; }
    fprintf(f, "%d\n", g->nb_noeuds);
    for (int i = 0; i < g->nb_noeuds; i++) {
        Arete* a = g->tab_noeuds[i].liste_adjacence;
        while (a) {
            fprintf(f, "%d %d %.2f %.2f %.2f %d\n", i, a->destination, a->latence, a->bande_passante, a->cout, a->securite);
            a = a->suivant;
        }
    }
    fclose(f);
    printf("\n\033[1;32m[SUCCES]\033[0m Topologie sauvegardee dans %s\n", nom_fichier);
}

void generer_dot(Graphe* g, const char* nom_fichier) {
    if (!g) return;
    FILE* f = fopen(nom_fichier, "w");
    if (!f) return;

    fprintf(f, "graph G {\n    node [shape=circle];\n");
    for (int i = 0; i < g->nb_noeuds; i++) {
        Arete* a = g->tab_noeuds[i].liste_adjacence;
        while (a) {
            if (i < a->destination) { // Pour ne pas doubler les liens
                fprintf(f, "    %d -- %d [label=\"%.1f\"];\n", i, a->destination, a->latence);
            }
            a = a->suivant;
        }
    }
    fprintf(f, "}\n");
    fclose(f);
    printf("\033[1;32m[OK]\033[0m Fichier %s genere.\n", nom_fichier);
}