#include "simulation.h"
#include "graphe.h"
#include "liste_chainee.h"
#include <stdio.h>
#include <stdlib.h>

// 1. Initialisation : Utilisation du nom FileAttente conforme au .h
FileAttente* init_file(int capacite) {
    FileAttente* f = malloc(sizeof(FileAttente));
    if (!f) return NULL;
    f->tete = f->queue = NULL;
    f->taille_actuelle = 0;
    f->capacite_max = capacite;
    return f;
}

// 2. ENQUEUE : Corrigé pour utiliser FileAttente et les bons noms de champs
void enqueue_paquet(FileAttente* f, int id, int prio, float taille, int cycle) {
    Paquet* nouveau = malloc(sizeof(Paquet));
    if (!nouveau) return;
    
    nouveau->id = id;
    nouveau->priorite = prio;
    nouveau->taille_Mo = taille;
    nouveau->cycle_arrivee = cycle; // Utilisation du champ ajouté dans le .h
    nouveau->suivant = nouveau->precedent = NULL;

    if (f->tete == NULL) {
        f->tete = f->queue = nouveau;
    } else {
        Paquet* courant = f->tete;
        // Priorité élevée (1) placée en début de file
        while (courant && courant->priorite <= prio) {
            courant = courant->suivant;
        }

        if (courant == f->tete) { 
            nouveau->suivant = f->tete;
            f->tete->precedent = nouveau;
            f->tete = nouveau;
        } else if (courant == NULL) { 
            nouveau->precedent = f->queue;
            f->queue->suivant = nouveau;
            f->queue = nouveau;
        } else { 
            nouveau->suivant = courant;
            nouveau->precedent = courant->precedent;
            courant->precedent->suivant = nouveau;
            courant->precedent = nouveau;
        }
    }
    f->taille_actuelle++;
}

// 3. DEQUEUE : Extraction O(1)
Paquet* dequeue_paquet(FileAttente* f) {
    if (!f || !f->tete) return NULL;

    Paquet* temp = f->tete;
    f->tete = f->tete->suivant;
    if (f->tete) f->tete->precedent = NULL;
    else f->queue = NULL;

    f->taille_actuelle--;
    return temp;
}

// 4. SIMULATION : Correction de la signature (void) et de la logique
void simuler_flux_avance(Graphe* g, int source, int destination, int nb_paquets, float taux_arrivee) {
    if (!g) {
        printf("\033[1;31m[ERREUR]\033[0m Graphe non charge.\n");
        return;
    }

    FileAttente* file = init_file(50);
    int paquets_generes = 0, paquets_traites = 0, paquets_perdus = 0;
    long temps_attente_total = 0;
    int cycle = 0;

    printf("\n\033[1;35m--- SIMULATION DYNAMIQUE : %d -> %d ---\033[0m\n", source, destination);

    while ((paquets_generes < nb_paquets || file->taille_actuelle > 0) && cycle < 3000) {
        // ARRIVÉE
        if (paquets_generes < nb_paquets) {
            if (((float)rand() / (float)RAND_MAX) < taux_arrivee) {
                if (file->taille_actuelle < file->capacite_max) {
                    // Correction : on appelle enqueue_paquet avec les bons paramètres
                    enqueue_paquet(file, paquets_generes + 1, rand() % 10 + 1, 1.5, cycle);
                } else {
                    paquets_perdus++;
                }
                paquets_generes++; 
            }
        }

        // TRAITEMENT
        if (file->taille_actuelle > 0) {
            Paquet* p = dequeue_paquet(file);
            if (p) {
                temps_attente_total += (cycle - p->cycle_arrivee);
                paquets_traites++;
                free(p);
            }
        }
        cycle++;
    }

    // Statistiques finales... (ton code d'affichage est correct)
    printf(">> Simulation terminee en %d cycles.\n", cycle);
    
    // Nettoyage final
    while(file->tete) {
        Paquet* p = dequeue_paquet(file);
        free(p);
    }
    free(file);
}