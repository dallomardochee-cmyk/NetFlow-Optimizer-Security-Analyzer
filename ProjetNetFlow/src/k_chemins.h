#ifndef K_CHEMINS_H
#define K_CHEMINS_H

#include "dijkstra.h"
#include "graphe.h"

/**
 * @brief Trouve les K plus courts chemins entre source et destination.
 * Affiche directement les K chemins trouvés.
 * Utilise une approche simplifiée de type DFS exhaustif trié ou Yen simplifié.
 *
 * @param graphe Le graphe
 * @param source ID source
 * @param destination ID destination
 * @param k Nombre de chemins à trouver
 */
void afficher_k_plus_courts_chemins(const Graphe *graphe, int source,
                                    int destination, int k);

#endif // K_CHEMINS_H
