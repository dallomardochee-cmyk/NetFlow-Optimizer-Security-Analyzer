#ifndef GRAPHE_H
#define GRAPHE_H

#include <stdio.h>
#include "liste_chainee.h"

typedef struct Arete {
    int destination;
    float latence;
    float bande_passante;
    float cout;
    int securite;
    struct Arete* suivant;
} Arete;

typedef struct {
    int id;
    char nom[50];
    Arete* aretes; // Liste d'adjacence
} Noeud;

typedef struct {
    int nb_noeuds;
    int capacite;
    int est_oriente;
    Noeud* noeuds;
    float** matrice_adjacence; // Matrice pour accès rapide O(1)
} Graphe;

Arete* creer_arete(int dest, float lat, float bp, float cout, int sec);
void inserer_en_tete(Arete** liste, Arete* nouvelle);
Graphe* creer_graphe(int nb_noeuds);
void ajouter_noeud(Graphe* g);
void ajouter_arete(Graphe* g, int src, int dest, float lat, float bp, float cout, int sec);
void supprimer_noeud(Graphe* g, int id_noeud);
void supprimer_arete(Graphe* g, int src, int dest);
void agrandir_graphe(Graphe* g, int nouvelle_capacite);
Graphe* charger_depuis_fichier(const char* nom_fichier);
void analyser_connectivite(Graphe* g, int depart);
void liberer_graphe(Graphe* g);
void sauvegarder_graphe(Graphe* g, const char* nom_fichier);

void analyser_topologie(Graphe* g);
void mesurer_performance_memoire(Graphe* g); 

#endif