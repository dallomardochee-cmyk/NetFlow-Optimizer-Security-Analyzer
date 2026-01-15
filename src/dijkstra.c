#include "dijkstra.h"

ResultatChemin *dijkstra(const Graphe *graphe, int source) {
  if (!graphe)
    return NULL;

  if (!graphe)
    return NULL;

  // On utilise la capacité max car les IDs peuvent aller jusqu'à max
  // Pour être sûr, on va allouer en fonction de l'ID max trouvé dans le graphe
  // + 1.

  int max_id = -1;
  for (int i = 0; i < graphe->nb_noeuds; i++) {
    if (graphe->noeuds[i].id > max_id)
      max_id = graphe->noeuds[i].id;
  }
  int t = max_id + 1;

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

  // Vérifier si la source existe (ou est dans les bornes)
  if (source >= t) {
    fprintf(stderr, "Erreur Dijkstra: Source %d hors limites\n", source);
    return res; // Retourne tableau init à INF
  }

  res->distances[source] = 0.0;

  // File de priorité
  ElementDijkstra *file = NULL;
  file = enfiler_dijkstra(file, source, 0.0);

  while (file) {
    int u;
    float dist_u;
    file = defiler_dijkstra(file, &u, &dist_u);

    // Si on a trouvé un chemin plus court ailleurs entre temps
    if (dist_u > res->distances[u])
      continue;

    // Trouver le nœud u dans le graphe pour avoir ses arêtes
    // Attention: graphe->noeuds est un tableau, on doit trouver l'index
    // correspondant à l'ID u. Optimisation: si graphe->noeuds[i].id == i, c'est
    // direct. Sinon recherche linéaire. Dans utils.c on a supposé ID
    // quelconque. On va faire une recherche linéaire. (Pourrait être optimisé
    // par une hashmap ou indexation directe si contrainte respectée).

    int index_u = -1;
    for (int i = 0; i < graphe->nb_noeuds; i++) {
      if (graphe->noeuds[i].id == u) {
        index_u = i;
        break;
      }
    }

    if (index_u == -1)
      continue; // Nœud pas dans la liste des nœuds actifs?

    Arete *arete = graphe->noeuds[index_u].aretes;
    while (arete) {
      int v = arete->destination;
      float p = arete->latence; // On optimise la Latence par défaut selon
                                // l'énoncé principal

      // Vérifier bornes v
      if (v < t) {
        if (res->distances[u] + p < res->distances[v]) {
          res->distances[v] = res->distances[u] + p;
          res->precesseurs[v] = u;
          file = enfiler_dijkstra(file, v, res->distances[v]);
        }
      }
      arete = arete->suivant;
    }
  }

  return res;
}

void afficher_chemin(const ResultatChemin *res, int destination) {
  if (!res)
    return;
  if (destination >= res->nb_noeuds || res->distances[destination] == FLT_MAX) {
    printf("Pas de chemin trouvé vers le nœud %d\n", destination);
    return;
  }

  printf("Chemin vers %d (Latence: %.2f ms) : ", destination,
         res->distances[destination]);

  // Reconstruction du chemin (backtracking des prédécesseurs)
  // On peut utiliser une récursion ou une pile temporaire
  // Utilisons un tableau temporaire car la profondeur est bornée par nb_noeuds
  int *chemin = (int *)malloc(res->nb_noeuds * sizeof(int));
  int count = 0;
  int curr = destination;

  while (curr != -1) {
    chemin[count++] = curr;
    if (curr == res->source)
      break;
    curr = res->precesseurs[curr];

    // Sécurité boucle infinie
    if (count > res->nb_noeuds) {
      printf(" (Erreur boucle dans chemin) ");
      break;
    }
  }

  for (int i = count - 1; i >= 0; i--) {
    printf("%d", chemin[i]);
    if (i > 0)
      printf(" -> ");
  }
  printf("\n");
  free(chemin);
}

void liberer_resultat_chemin(ResultatChemin *res) {
  if (!res)
    return;
  free(res->distances);
  free(res->precesseurs);
  free(res);
}
