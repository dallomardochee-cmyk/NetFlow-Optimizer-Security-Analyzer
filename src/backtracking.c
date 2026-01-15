#include "backtracking.h"
#include <float.h>

// Variables globales pour stocker le meilleur chemin trouvé pendant la
// récursion
static float meilleur_latence_trouvee = FLT_MAX;
static int *meilleur_chemin = NULL;
static int meilleure_longueur = 0;

static int *chemin_actuel = NULL;
static int *visite = NULL;

void copier_meilleur_chemin(int longueur) {
  if (meilleur_chemin)
    free(meilleur_chemin);
  meilleur_chemin = (int *)malloc(longueur * sizeof(int));
  for (int i = 0; i < longueur; i++)
    meilleur_chemin[i] = chemin_actuel[i];
  meilleure_longueur = longueur;
}

void dfs_backtracking(const Graphe *g, int u, int dest, Contraintes c,
                      float latence_acc, float cout_acc, int profondeur) {
  // Élagage (Pruning)
  if (latence_acc >= meilleur_latence_trouvee)
    return;
  if (c.max_latence > 0 && latence_acc > c.max_latence)
    return;
  if (cout_acc > c.max_cout)
    return;

  // Verification Noeuds Interdits (Si le nœud actuel est interdit)
  for (int i = 0; i < c.nb_interdits; i++) {
    if (u == c.noeuds_interdits[i])
      return;
  }

  // init chemin actuel
  chemin_actuel[profondeur - 1] = u;
  visite[u] = 1;

  // Si destination atteinte
  if (u == dest) {
    // Vérifier si tous les obligatoires sont passés
    int tout_vu = 1;
    for (int i = 0; i < c.nb_obligatoires; i++) {
      int obl = c.noeuds_obligatoires[i];
      int vu = 0;
      for (int j = 0; j < profondeur; j++) {
        if (chemin_actuel[j] == obl) {
          vu = 1;
          break;
        }
      }
      if (!vu) {
        tout_vu = 0;
        break;
      }
    }

    if (tout_vu && latence_acc < meilleur_latence_trouvee) {
      meilleur_latence_trouvee = latence_acc;
      copier_meilleur_chemin(profondeur);
    }
    visite[u] = 0; // Backtrack important ici aussi
    return;
  }

  // Trouver index u
  int index_u = -1;
  for (int i = 0; i < g->nb_noeuds; i++)
    if (g->noeuds[i].id == u) {
      index_u = i;
      break;
    }

  if (index_u != -1) {
    Arete *arete = g->noeuds[index_u].aretes;
    while (arete) {
      int v = arete->destination;

      // Vérifier contraintes locales (BP, Secu)
      if (!visite[v] && arete->bande_passante >= c.min_bande_passante &&
          arete->securite >= c.min_securite) {

        dfs_backtracking(g, v, dest, c, latence_acc + arete->latence,
                         cout_acc + arete->cout, profondeur + 1);
      }
      arete = arete->suivant;
    }
  }

  visite[u] = 0; // Backtrack
}

ResultatChemin *recherche_contrainte(const Graphe *graphe, int source,
                                     int destination, Contraintes contraintes) {
  if (!graphe)
    return NULL;

  // Reset globales
  meilleur_latence_trouvee = FLT_MAX;
  if (meilleur_chemin) {
    free(meilleur_chemin);
    meilleur_chemin = NULL;
  }
  meilleure_longueur = 0;

  // Allocation buffers temporaires
  // Taille max chemin = nb_noeuds. Attention ID vs index.
  // Supposons IDs petits pour ce tableau visite, sinon map.
  int max_id = 0;
  for (int i = 0; i < graphe->nb_noeuds; i++)
    if (graphe->noeuds[i].id > max_id)
      max_id = graphe->noeuds[i].id;
  int taille = max_id + 1;

  visite = (int *)calloc(taille, sizeof(int));
  chemin_actuel = (int *)malloc(taille * sizeof(int)); // Au pire tout le graphe

  // Lancer DFS
  chemin_actuel[0] = source;
  dfs_backtracking(graphe, source, destination, contraintes, 0.0, 0.0, 1);

  // Nettoyage tmp
  free(visite);
  free(chemin_actuel);

  if (meilleure_longueur == 0)
    return NULL; // Pas de chemin trouvé

  // Convertir en ResultatChemin
  // ResultatChemin stocke prédecesseurs. C'est un format un peu inadapté pour
  // un simple chemin linéaire stocké ainsi Mais on va adapter : precesseur[v] =
  // u.
  ResultatChemin *res = (ResultatChemin *)malloc(sizeof(ResultatChemin));
  res->nb_noeuds = taille;
  res->distances = (float *)malloc(taille * sizeof(float));
  res->precesseurs = (int *)malloc(taille * sizeof(int));
  res->source = source;

  for (int i = 0; i < taille; i++) {
    res->distances[i] = FLT_MAX;
    res->precesseurs[i] = -1;
  }

  // Remplir distance finale
  res->distances[destination] = meilleur_latence_trouvee;

  // Remplir prédecesseurs
  for (int i = 0; i < meilleure_longueur - 1; i++) {
    int u = meilleur_chemin[i];
    int v = meilleur_chemin[i + 1];
    res->precesseurs[v] = u;
  }

  // Le chemin est stocké dans meilleur_chemin, qui est linear.
  // afficher_chemin utilise les prédecesseurs. Donc ça marche.

  // free meilleur_chemin car on a copié dans prédecesseurs ? non,
  // meilleur_chemin est statique ou duplicata? meilleur_chemin a été malloué
  // dans copier_meilleur_chemin. Il faut le free.
  free(meilleur_chemin);
  meilleur_chemin = NULL; // important pour leak

  return res;
}
