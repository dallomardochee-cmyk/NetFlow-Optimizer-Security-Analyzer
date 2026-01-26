#include "liste_chainee.h"
#include "graphe.h"       
#include "simulation.h"
#include <stdlib.h>

Arete* creer_arete(int dest, float lat, float bp, float cout, int sec) {
// ========== CREATION D'ARETE ==========
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
// ========== INITIALISATION DE FILE ==========
}

FileAttente* initialiser_file(int capacite_max) {
    FileAttente *file = (FileAttente*)malloc(sizeof(FileAttente));
    file->tete = NULL;
    file->queue = NULL;
    file->taille_actuelle = 0;
    file->capacite_max = capacite_max;
    return file;
// ========== ENQUEUE: INSERTION AVEC PRIORITE ==========
}

// Opération : enqueue (Insertion avec priorité O(n))
int enqueue(FileAttente *file, int id, int priorite, float taille, int cycle) {
    if (file->taille_actuelle >= file->capacite_max) {
        return 0; // Échec : sera comptabilisé comme "perte" dans simulation.c
    }

    Paquet *nouveau = (Paquet*)malloc(sizeof(Paquet));
    nouveau->id = id;
    nouveau->priorite = priorite;
    nouveau->taille_Mo = taille;
    nouveau->cycle_arrivee = cycle;
    nouveau->suivant = NULL;
    nouveau->precedent = NULL;

    // Cas 1 : File vide
    if (file->tete == NULL) {
        file->tete = file->queue = nouveau;
    } 
    // Cas 2 : Insertion en tête (plus prioritaire que le premier)
    else if (priorite < file->tete->priorite) {
        nouveau->suivant = file->tete;
        file->tete->precedent = nouveau;
        file->tete = nouveau;
    } 
    // Cas 3 : Insertion au milieu ou à la fin (parcours O(n))
    else {
        Paquet *courant = file->tete;
        while (courant->suivant != NULL && courant->suivant->priorite <= priorite) {
            courant = courant->suivant;
        }
        nouveau->suivant = courant->suivant;
        nouveau->precedent = courant;
        
        if (courant->suivant != NULL) {
            courant->suivant->precedent = nouveau;
        } else {
            file->queue = nouveau; // Nouveau devient la nouvelle fin
        }
        courant->suivant = nouveau;
    }
// ========== DEQUEUE: EXTRACTION O(1) ==========

    file->taille_actuelle++;
    return 1;
}

// Opération : dequeue (Extraction du plus prioritaire O(1))
Paquet* dequeue(FileAttente *file) {
    if (file->tete == NULL) return NULL;

    Paquet *p = file->tete;
    file->tete = file->tete->suivant;

    if (file->tete != NULL) {
        file->tete->precedent = NULL;
// ========== PEEK: CONSULTATION SANS SUPPRESSION ==========
    } else {
        file->queue = NULL;
    }

// ========== INSERTION EN TETE DE LISTE ==========
    file->taille_actuelle--;
    return p;
}

// Opération : peek (Consultation sans suppression O(1))
Paquet* peek(FileAttente *file) {
    return file->tete;
// ========== LIBERATION DE LISTE ==========
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