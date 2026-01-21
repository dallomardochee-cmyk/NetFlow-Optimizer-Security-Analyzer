#ifndef BACKTRACKING_H
#define BACKTRACKING_H

#include "graphe.h"

// Doit correspondre exactement à la définition dans le .c
void trouver_chemins(Graphe* g, int s, int d, float bp_min, float cost_max);

#endif