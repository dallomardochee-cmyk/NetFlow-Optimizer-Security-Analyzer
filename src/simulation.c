#include "simulation.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void lancer_simulation_flux(int nb_cycles, int capacite_file,
                            float taux_arrivee) {
  printf("--- Démarrage Simulation de Flux ---\n");
  printf("Cycles: %d, Capacité: %d, Taux Arrivée: %.2f\n", nb_cycles,
         capacite_file, taux_arrivee);

  FileAttente *file = initialiser_file(capacite_file);
  int paquets_generes = 0;
  int paquets_traites = 0;
  int paquets_perdus = 0;
  long temps_attente_total = 0;

  // Pour simuler le temps, on utilise le numéro du cycle comme timestamp
  // d'arrivée La structure Paquet n'a pas de champ timestamp, on va utiliser
  // 'taille_Mo' pour stocker le cycle d'arrivée (astuce temporaire) Ou mieux,
  // on suppose juste que le paquet est traité immédiatement si possible.

  srand(time(NULL));

  for (int cycle = 1; cycle <= nb_cycles; cycle++) {
    // 1. Arrivée potentielle d'un paquet
    float r = (float)rand() / (float)RAND_MAX;
    if (r < taux_arrivee) {
      // Génération paquet
      int id = 1000 + paquets_generes;
      int prio = rand() % 10; // 0-9
      // On stocke le cycle creation dans 'source' pour calculer l'attente plus
      // tard (astuce)
      if (enfiler_paquet(file, id, prio, 1.0, cycle, 0)) {
        paquets_generes++;
        // printf("[Cycle %d] Paquet %d (Prio %d) ajouté.\n", cycle, id, prio);
      } else {
        paquets_perdus++;
        // printf("[Cycle %d] PERTE : File pleine !\n", cycle);
      }
    }

    // 2. Traitement (Simulé : on traite 1 paquet par cycle max)
    Paquet *p = defiler_paquet(file);
    if (p) {
      int cycle_creation = p->source; // On récupère notre timestamp caché
      long attente = cycle - cycle_creation;
      temps_attente_total += attente;
      paquets_traites++;
      // printf("[Cycle %d] Paquet %d traité (Attente: %ld cycles).\n", cycle,
      // p->id, attente);
      free(p);
    }
  }

  // Résultats
  printf("\n=== Résultats Simulation ===\n");
  printf("Total Paquets Générés : %d\n", paquets_generes + paquets_perdus);
  printf("Paquets Traités       : %d\n", paquets_traites);
  printf("Paquets Perdus        : %d\n", paquets_perdus);
  printf("Reste en file         : %d\n", file->taille_actuelle);

  float taux_perte = 0.0;
  if ((paquets_generes + paquets_perdus) > 0)
    taux_perte = (float)paquets_perdus /
                 (float)(paquets_generes + paquets_perdus) * 100.0;

  float attente_moyenne = 0.0;
  if (paquets_traites > 0)
    attente_moyenne = (float)temps_attente_total / (float)paquets_traites;

  printf("Taux de Perte         : %.2f %%\n", taux_perte);
  printf("Temps Attente Moyen   : %.2f cycles\n\n", attente_moyenne);

  liberer_file(file);
}
