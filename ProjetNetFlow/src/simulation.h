#ifndef SIMULATION_H
#define SIMULATION_H

typedef struct Paquet {
    int id;
    int priorite;
    float taille; // en Ko
    struct Paquet *prec, *suiv;
} Paquet;

typedef struct {
    Paquet* tete;
    Paquet* queue;
    int nb_paquets;
} FilePriorite;

FilePriorite* init_file();
void enqueue_paquet(FilePriorite* f, int id, int prio, float taille);
Paquet* dequeue_paquet(FilePriorite* f);
void simuler_flux(int nb_a_simuler);

#endif