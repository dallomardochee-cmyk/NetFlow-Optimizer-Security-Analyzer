#include "liste_chainee.h"
#include <stdlib.h>

Arete* creer_arete(int dest, float lat, float bp, float cout, int sec) {
    Arete* n = (Arete*)malloc(sizeof(Arete));
    if (n) {
        n->destination = dest;
        n->latence = lat;
        n->bande_passante = bp;
        n->cout = cout;
        n->securite = sec;
        n->suivant = NULL;
    }
    return n;
}

void inserer_en_tete(Arete** liste, Arete* nouvelle) {
    if (nouvelle) {
        nouvelle->suivant = *liste;
        *liste = nouvelle;
    }
}

void liberer_liste(Arete* liste) {
    Arete* courant = liste;
    while (courant) {
        Arete* a_supprimer = courant;
        courant = courant->suivant;
        free(a_supprimer);
    }
}