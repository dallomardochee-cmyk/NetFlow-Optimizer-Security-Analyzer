#include "k_chemins.h"
#include <stdio.h>
#include <stdlib.h>

// Structure pour stocker un chemin complet
typedef struct {
  int *noeuds;
  int nb_noeuds;
  float latence_totale;
} CheminComplet;

// Liste globale de tous les chemins trouvés (pour trier ensuite)
static CheminComplet *tous_chemins = NULL;
static int nb_chemins_trouves = 0;
static int capacite_chemins = 0;

static int *chemin_temp = NULL;
static int *visite = NULL;

static void ajouter_chemin(int longueur, float latence) {
  if (nb_chemins_trouves >= capacite_chemins) {
    capacite_chemins = (capacite_chemins == 0) ? 10 : capacite_chemins * 2;
    tous_chemins = (CheminComplet *)realloc(
        tous_chemins, capacite_chemins * sizeof(CheminComplet));
  }

  CheminComplet *c = &tous_chemins[nb_chemins_trouves];
  c->nb_noeuds = longueur;
  c->latence_totale = latence;
  c->noeuds = (int *)malloc(longueur * sizeof(int));
  for (int i = 0; i < longueur; i++)
    c->noeuds[i] = chemin_temp[i];

  nb_chemins_trouves++;
}

static void dfs_k(const Graphe *g, int u, int dest, float latence_acc,
                  int profondeur) {
  chemin_temp[profondeur - 1] = u;
  visite[u] = 1;

  if (u == dest) {
    ajouter_chemin(profondeur, latence_acc);
    visite[u] = 0;
    return;
  }

  int idx = -1;
  for (int i = 0; i < g->nb_noeuds; i++)
    if (g->noeuds[i].id == u) {
      idx = i;
      break;
    }

  if (idx != -1) {
    Arete *a = g->noeuds[idx].aretes;
    while (a) {
      int v = a->destination;
      if (!visite[v]) {
        dfs_k(g, v, dest, latence_acc + a->latence, profondeur + 1);
      }
      a = a->suivant;
    }
  }

  visite[u] = 0;
}

// Fonction de comparaison pour qsort
static int comparer_chemins(const void *a, const void *b) {
  CheminComplet *c1 = (CheminComplet *)a;
  CheminComplet *c2 = (CheminComplet *)b;
  if (c1->latence_totale < c2->latence_totale)
    return -1;
  if (c1->latence_totale > c2->latence_totale)
    return 1;
  return 0;
}

void afficher_k_plus_courts_chemins(const Graphe *graphe, int source,
                                    int destination, int k) {
  if (!graphe)
    return;

  // init global
  nb_chemins_trouves = 0;
  capacite_chemins = 0;
  if (tous_chemins)
    free(tous_chemins);
  tous_chemins = NULL;

  int max_id = 0;
  for (int i = 0; i < graphe->nb_noeuds; i++)
    if (graphe->noeuds[i].id > max_id)
      max_id = graphe->noeuds[i].id;
  int t = max_id + 1;

  chemin_temp = (int *)malloc(t * sizeof(int));
  visite = (int *)calloc(t, sizeof(int));

  // Lancer recherche exhaustive (attention, exponentiel si graphe énorme, mais
  // OK pour projet étudiant) Pour "Diviser pour Régner" pur (Yen), c'est plus
  // complexe. Ici on fait une énumération (Backtracking) + Tri (Sort).
  dfs_k(graphe, source, destination, 0.0, 1);

  // Trier
  qsort(tous_chemins, nb_chemins_trouves, sizeof(CheminComplet),
        comparer_chemins);

  // Afficher Top K
  printf("--- Les %d Plus Courts Chemins (sur %d trouvés) ---\n", k,
         nb_chemins_trouves);
  int limit = (k < nb_chemins_trouves) ? k : nb_chemins_trouves;

  for (int i = 0; i < limit; i++) {
    printf("%d) Latence %.2f : ", i + 1, tous_chemins[i].latence_totale);
    for (int j = 0; j < tous_chemins[i].nb_noeuds; j++) {
      printf("%d", tous_chemins[i].noeuds[j]);
      if (j < tous_chemins[i].nb_noeuds - 1)
        printf(" -> ");
    }
    printf("\n");
  }

  // Nettoyage
  for (int i = 0; i < nb_chemins_trouves; i++)
    free(tous_chemins[i].noeuds);
  free(tous_chemins);
  tous_chemins = NULL;
  free(chemin_temp);
  free(visite);
}
