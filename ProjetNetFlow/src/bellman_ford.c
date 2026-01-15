#include "bellman_ford.h"
#include <float.h>

ResultatChemin *bellman_ford(const Graphe *graphe, int source) {
  if (!graphe)
    return NULL;

  // Détermination taille max ID
  int max_id = -1;
  for (int i = 0; i < graphe->nb_noeuds; i++) {
    if (graphe->noeuds[i].id > max_id)
      max_id = graphe->noeuds[i].id;
  }
  int t = max_id + 1;

  // Allocation résultat
  ResultatChemin *res = (ResultatChemin *)malloc(sizeof(ResultatChemin));
  res->distances = (float *)malloc(t * sizeof(float));
  res->precesseurs = (int *)malloc(t * sizeof(int));
  res->nb_noeuds = t;
  res->source = source;

  // Initialisation
  for (int i = 0; i < t; i++) {
    res->distances[i] = FLT_MAX;
    res->precesseurs[i] = -1;
  }

  if (source < t)
    res->distances[source] = 0.0;

  // Relaxation V-1 fois
  // On itère sur toutes les arêtes
  // Graphe stocké en liste d'adjacence, donc on parcourt tous les nœuds u puis
  // leurs arêtes
  for (int i = 1; i < graphe->nb_noeuds; i++) {
    int changement = 0;
    for (int u_idx = 0; u_idx < graphe->nb_noeuds; u_idx++) {
      int u = graphe->noeuds[u_idx].id;

      // Si distance u est infinie, on ne peut pas relaxer ses voisins
      if (res->distances[u] == FLT_MAX)
        continue;

      Arete *arete = graphe->noeuds[u_idx].aretes;
      while (arete) {
        int v = arete->destination;
        float poids = arete->latence; // On optimise la latence

        if (v < t) {
          if (res->distances[u] + poids < res->distances[v]) {
            res->distances[v] = res->distances[u] + poids;
            res->precesseurs[v] = u;
            changement = 1;
          }
        }
        arete = arete->suivant;
      }
    }
    // Optimisation : si aucune modification dans un tour, on arrête
    if (!changement)
      break;
  }

  // Détection de cycle négatif (V-ième tour)
  for (int u_idx = 0; u_idx < graphe->nb_noeuds; u_idx++) {
    int u = graphe->noeuds[u_idx].id;
    if (res->distances[u] == FLT_MAX)
      continue;

    Arete *arete = graphe->noeuds[u_idx].aretes;
    while (arete) {
      int v = arete->destination;
      float poids = arete->latence;
      if (v < t) {
        if (res->distances[u] + poids < res->distances[v]) {
          printf("Cycle de poids négatif détecté !\n");
          liberer_resultat_chemin(res);
          return NULL;
        }
      }
      arete = arete->suivant;
    }
  }

  return res;
}
