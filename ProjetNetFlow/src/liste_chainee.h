#ifndef LISTE_CHAINEE_H
#define LISTE_CHAINEE_H

typedef struct Arete {
    int destination;
    float latence;
    float bande_passante;
    float cout;
    int securite;
    struct Arete* suivant; // Le pointeur vers l'élément suivant
} Arete;

// Prototypes pour manipuler la liste
Arete* creer_arete(int dest, float lat, float bp, float cout, int sec);
void inserer_en_tete(Arete** liste, Arete* nouvelle);
void liberer_liste(Arete* liste);

#endif