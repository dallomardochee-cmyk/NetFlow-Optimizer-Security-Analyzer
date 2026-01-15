#ifndef BELLMAN_FORD_H
#define BELLMAN_FORD_H

#include "dijkstra.h" // Pour réutiliser ResultatChemin
#include "graphe.h"

/**
 * @brief Exécute l'algorithme de Bellman-Ford
 * @param graphe Le graphe à parcourir
 * @param source ID du nœud de départ
 * @return Pointeur vers ResultatChemin, ou NULL si cycle négatif détecté
 */
ResultatChemin *bellman_ford(const Graphe *graphe, int source);

#endif // BELLMAN_FORD_H
