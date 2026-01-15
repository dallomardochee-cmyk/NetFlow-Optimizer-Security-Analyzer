#ifndef SECURITE_H
#define SECURITE_H

#include "graphe.h"
#include "liste_chainee.h"

/**
 * @brief Détecte la présence de cycles dans le graphe (DFS)
 * @param graphe Le graphe à analyser
 * @return 1 si cycle détecté, 0 sinon
 * @complexity O(V + E)
 */
int detecter_cycle(const Graphe *graphe);

/**
 * @brief Détecte la présence de cycles dans le graphe via BFS (Kahn)
 * Utilise le tri topologique (Kahn's algorithm) pour détecter les cycles.
 * @param graphe Le graphe à analyser
 * @return 1 si cycle détecté, 0 sinon
 * @complexity O(V + E)
 */
int detecter_cycle_bfs(const Graphe *graphe);

/**
 * @brief Parcours en largeur (BFS) depuis un nœud source
 * Affiche l'ordre de visite des nœuds.
 * @param graphe Le graphe à parcourir
 * @param source ID du nœud de départ
 * @complexity O(V + E)
 */
void parcours_bfs(const Graphe *graphe, int source);

/**
 * @brief Parcours en profondeur (DFS) depuis un nœud source
 * Affiche l'ordre de visite des nœuds (version itérative avec pile).
 * @param graphe Le graphe à parcourir
 * @param source ID du nœud de départ
 * @complexity O(V + E)
 */
void parcours_dfs(const Graphe *graphe, int source);

/**
 * @brief Affiche les points d'articulation (nœuds critiques) du graphe
 * Utilise l'algo DFS avec temps de découverte et low-link value.
 * @param graphe Le graphe
 * @complexity O(V + E)
 */
void afficher_points_articulation(const Graphe *graphe);

/**
 * @brief Affiche les Composantes Fortement Connexes (SCC) via l'algorithme de
 * Tarjan Permet d'identifier les sous-réseaux isolés ou fortement liés.
 * @param graphe Le graphe
 * @complexity O(V + E)
 */
void afficher_scc_tarjan(const Graphe *graphe);

/**
 * @brief Affiche les Ponts (Arêtes critiques) du graphe.
 * @param graphe Le graphe
 * @complexity O(V + E)
 */
void afficher_ponts(const Graphe *graphe);

#endif // SECURITE_H
