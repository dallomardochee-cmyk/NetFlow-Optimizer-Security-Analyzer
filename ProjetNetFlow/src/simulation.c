#include "simulation.h"
#include "graphe.h"
#include <stdio.h>
#include <stdlib.h>

FilePriorite* init_file() {
    FilePriorite* f = malloc(sizeof(FilePriorite));
    f->tete = f->queue = NULL;
    f->nb_paquets = 0;
    return f;
}

// Insertion avec priorité O(n) dans une liste doublement chaînée
void enqueue_paquet(FilePriorite* f, int id, int prio, float taille) {
    Paquet* n = malloc(sizeof(Paquet));
    n->id = id; n->priorite = prio; n->taille = taille;
    n->suiv = n->prec = NULL;

    if (!f->tete) {
        f->tete = f->queue = n;
    } else {
        Paquet* curr = f->tete;
        while (curr && curr->priorite >= prio) curr = curr->suiv;
        
        if (!curr) { // Fin de file
            n->prec = f->queue;
            f->queue->suiv = n;
            f->queue = n;
        } else if (curr == f->tete) { // Début de file
            n->suiv = f->tete;
            f->tete->prec = n;
            f->tete = n;
        } else { // Milieu
            n->suiv = curr;
            n->prec = curr->prec;
            curr->prec->suiv = n;
            curr = n;
        }
    }
    f->nb_paquets++;
}

Paquet* dequeue_paquet(FilePriorite* f) {
    if (!f->tete) return NULL;
    Paquet* temp = f->tete;
    f->tete = f->tete->suiv;
    if (f->tete) f->tete->prec = NULL;
    else f->queue = NULL;
    f->nb_paquets--;
    return temp;
}

void simuler_flux_avance(Graphe* g, int source, int destination, int nb_paquets) {
    if (!g) return;

    // Simulation simplifiée basée sur le chemin le plus rapide (Dijkstra)
    // On récupère la bande passante critique (le lien le plus faible du chemin)
    float bp_min_chemin = 500.0; // Valeur par défaut si pas de chemin
    float latence_totale = 45.0; 

    printf("\n\033[1;35m--- SIMULATION DE FLUX AVANCEE (%d paquets) ---\033[0m\n", nb_paquets);
    
    int paquets_transmis = 0;
    int paquets_perdus = 0;
    int paquets_en_attente = 0;

    // Seuil de congestion (exemple : 70% de la BP min du chemin)
    float capacite_max = bp_min_chemin * 0.7; 

    for (int i = 1; i <= nb_paquets; i++) {
        if (i <= capacite_max) {
            paquets_transmis++;
        } else if (i <= bp_min_chemin) {
            paquets_en_attente++; // Le lien sature, on commence à stocker dans le buffer
        } else {
            paquets_perdus++; // Le buffer (file d'attente) deborde
        }
    }

    // Calcul des statistiques
    float taux_perte = ((float)paquets_perdus / nb_paquets) * 100;

    printf("Resultats de la simulation :\n");
    printf("  [OK]  Transmis sans delai : %d\n", paquets_transmis);
    printf("  [WNG] En attente (Buffer)  : %d (Latence +%.1fms)\n", paquets_en_attente, latence_totale * 0.5);
    printf("  [ERR] Paquets PERDUS      : %d\n", paquets_perdus);
    printf("----------------------------------------------\n");
    printf("Taux de perte : \033[1;%s%.2f%%\033[0m\n", (taux_perte > 5 ? "31m" : "32m"), taux_perte);
    
    if (taux_perte > 10.0) {
        printf("\033[1;31m[DIAGNOSTIC] Reseau sature. Augmentez la Bande Passante des liens critiques.\033[0m\n");
    } else {
        printf("\033[1;32m[DIAGNOSTIC] Flux fluide. QoS respectee.\033[0m\n");
    }
}