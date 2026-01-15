#ifndef DIJKSTRA_H
#define DIJKSTRA_H

#include "graphe.h"
#include "liste_chainee.h"
#include <float.h> // Pour FLT_MAX

/**
 * @brief Structure pour représenter le résultat d'un calcul de chemin
 */
typedef struct ResultatChemin {
  int *precesseurs; // Tableau des prédécesseurs pour reconstruire le chemin
  float *distances; // Tableau des distances minimales depuis la source
  int nb_noeuds;    // Taille des tableaux
  int source;       // Nœud source
} ResultatChemin;

/**
 * @brief Exécute l'algorithme de Dijkstra
 * @param graphe Le graphe à parcourir
 * @param source ID du nœud de départ
 * @return Pointeur vers ResultatChemin contenant distances et prédécesseurs
 */
ResultatChemin *dijkstra(const Graphe *graphe, int source);

/**
 * @brief Affiche le chemin le plus court vers une destination
 * @param resultat Résultat de l'algo Dijkstra
 * @param destination ID du nœud destination
 */
void afficher_chemin(const ResultatChemin *resultat, int destination);

/**
 * @brief Libère la mémoire du résultat
 */
void liberer_resultat_chemin(ResultatChemin *resultat);

#endif // DIJKSTRA_H
