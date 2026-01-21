#ifndef GRAPHE_H
#define GRAPHE_H

#include <stdio.h>
#include "liste_chainee.h"

typedef struct {
    char type[30];
    Arete* liste_adjacence;
} Noeud;

typedef struct {
    int nb_noeuds;
    Noeud* tab_noeuds;
} Graphe;

Graphe* creer_graphe(int nb_noeuds);
void ajouter_arete(Graphe* g, int src, int dest, float lat, float bp, float cout, int sec);
Graphe* charger_depuis_fichier(const char* nom_fichier);
void analyser_connectivite(Graphe* g, int depart);
void liberer_graphe(Graphe* g);
void sauvegarder_graphe(Graphe* g, const char* nom_fichier);

void analyser_topologie(Graphe* g);
void mesurer_performance_memoire(Graphe* g); 

#endif