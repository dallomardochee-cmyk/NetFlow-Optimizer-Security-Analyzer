#ifndef BACKTRACKING_H
#define BACKTRACKING_H

#include "graphe.h"

/**
 * Lance la recherche du chemin optimal avec élagage (Backtracking).
 * Paramètres : Graphe, Source, Destination, BP min, Coût max, Sécurité min.
 */
void lancer_backtracking(Graphe* g, int src, int dest, float bp_min, float cout_max, int sec_min);

#endif