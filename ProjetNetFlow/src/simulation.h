#ifndef SIMULATION_H
#define SIMULATION_H

// INDISPENSABLE : Le compilateur doit savoir ce qu'est un 'Graphe'
#include "graphe.h" 
#include "liste_chainee.h"

// Prototypes des fonctions de file (utilisant FileAttente de liste_chainee.h)
FileAttente* init_file(int capacite);
void enqueue_paquet(FileAttente* f, int id, int prio, float taille, int cycle);
Paquet* dequeue_paquet(FileAttente* f);

// Signature de la simulation (Graphe est maintenant reconnu grâce à l'include plus haut)
void simuler_flux_avance(Graphe* g, int source, int destination, int nb_paquets, float taux_arrivee);

#endif