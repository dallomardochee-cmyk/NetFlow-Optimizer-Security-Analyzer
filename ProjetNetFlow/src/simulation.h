#ifndef SIMULATION_H
#define SIMULATION_H

#include "liste_chainee.h"

/**
 * @brief Lance une simulation de flux de paquets
 * Génère des paquets aléatoires, les enfile, et les traite.
 * Affiche des statistiques à la fin.
 *
 * @param nb_cycles Nombre d'itérations de la simulation
 * @param capacite_file Capacité maximale de la file d'attente
 * @param taux_arrivee Probabilité d'arrivée d'un paquet par cycle (0.0 - 1.0)
 */
void lancer_simulation_flux(int nb_cycles, int capacite_file,
                            float taux_arrivee);

#endif // SIMULATION_H
