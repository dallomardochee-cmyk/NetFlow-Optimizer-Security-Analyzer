#ifndef BACKTRACKING_H
#define BACKTRACKING_H

#include "dijkstra.h" // Pour ResultatChemin
#include "graphe.h"

/**
 * @brief Structure définissant les contraintes de recherche
 */
typedef struct Contraintes {
  float min_bande_passante;
  float max_cout;
  int min_securite;
  int max_latence;
  int *noeuds_obligatoires;
  int nb_obligatoires;
  int *noeuds_interdits;
  int nb_interdits;
} Contraintes;

/**
 * @brief Trouve le meilleur chemin respectant les contraintes (minimisant la
 * latence)
 * @param graphe Le graphe
 * @param source ID source
 * @param destination ID destination
 * @param contraintes Struct Contraintes
 * @return Pointeur vers ResultatChemin (contenant uniquement le chemin trouvé),
 * NULL si aucun
 */
ResultatChemin *recherche_contrainte(const Graphe *graphe, int source,
                                     int destination, Contraintes contraintes);

#endif // BACKTRACKING_H
