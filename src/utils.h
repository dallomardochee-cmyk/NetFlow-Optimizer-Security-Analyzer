#ifndef UTILS_H
#define UTILS_H

#include "graphe.h"

// Define max line length for file reading
#define MAX_LINE_LENGTH 256

/**
 * @brief Charge un graphe depuis un fichier texte
 * @param chemin_fichier Chemin vers le fichier de définition du graphe
 * @return Pointeur vers le graphe créé, ou NULL en cas d'erreur
 */
Graphe *charger_graphe(const char *chemin_fichier);

/**
 * @brief Sauvegarde le graphe dans un fichier texte
 * @param graphe Le graphe à sauvegarder
 * @param chemin_fichier Chemin de destination
 * @return 1 si succès, 0 si échec
 */
int sauvegarder_graphe(const Graphe *graphe, const char *chemin_fichier);

/**
 * @brief Exporte le graphe au format Graphviz (.dot) pour visualisation
 * @param graphe Le graphe à exporter
 * @param nom_fichier Chemin du fichier de sortie
 */
void exporter_graphe_dot(const Graphe *graphe, const char *nom_fichier);

#endif // UTILS_H
