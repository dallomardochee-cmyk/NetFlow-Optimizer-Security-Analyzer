#ifndef LISTE_CHAINEE_H
#define LISTE_CHAINEE_H

#include <stdio.h>
#include <stdlib.h>

// Définition unique du Paquet
typedef struct Paquet {
    int id;
    int priorite;
    float taille_Mo;
    int source;
    int destination;
    int cycle_arrivee; 
    struct Paquet* precedent;
    struct Paquet* suivant;
} Paquet;

typedef struct {
    Paquet *tete;
    Paquet *queue;
    int taille_actuelle;
    int capacite_max;
} FileAttente;

FileAttente* initialiser_file(int capacite_max);
int enqueue(FileAttente *file, int id, int priorite, float taille, int cycle);
Paquet* dequeue(FileAttente *file);
Paquet* peek(FileAttente *file);
void liberer_file(FileAttente *file);

#endif