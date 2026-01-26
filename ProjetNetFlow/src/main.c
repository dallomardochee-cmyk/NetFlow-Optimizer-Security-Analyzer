#include "backtracking.h"
#include "bellman_ford.h"
#include "dijkstra.h"
#include "graphe.h"
#include "k_chemins.h"
#include "liste_chainee.h"
#include "securite.h"
#include "simulation.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void afficher_menu() {
  printf("\n=== NetFlow Optimizer & Security Analyzer ===\n");
  printf("1. Charger un graphe (data/test_save.txt par défaut)\n");
  printf("2. Afficher le graphe\n");
  printf("3. Algorithme Dijkstra (Plus court chemin)\n");
  printf("4. Algorithme Bellman-Ford (Robuste)\n");
  printf("5. Backtracking (Chemin avec contraintes)\n");
  printf("6. Analyse Sécurité (Cycles, Points d'articulation, Ponts, SCC)\n");
  printf("7. Simulation de Trafic (Files d'attente)\n");
  printf("8. K-Plus Courts Chemins (Bonus)\n");
  printf("9. Exporter Visualisation (.dot)\n");
  printf("0. Quitter\n");
  printf("Votre choix : ");
}

int main() {
  Graphe *g = NULL;
  int choix = -1;
  char buffer[100];

  // Chargement par défaut au démarrage pour faciliter les tests
  g = charger_graphe("data/test_save.txt");
  // Si échec, on crée un graphe manuel
  if (!g) {
    printf("Fichier par défaut non trouvé, création d'un graphe simple.\n");
    g = creer_graphe(5, 0);
    ajouter_noeud(g, 0, "Paris");
    ajouter_noeud(g, 1, "Lyon");
    ajouter_noeud(g, 2, "Marseille");
    ajouter_arete(g, 0, 1, 15.0, 1000.0, 50.0, 9);
    ajouter_arete(g, 1, 2, 8.0, 1000.0, 30.0, 8);
    ajouter_arete(g, 0, 2, 25.0, 500.0, 70.0, 7);
  }

  while (choix != 0) {
    afficher_menu();
    if (scanf("%d", &choix) != 1) {
      // Vider buffer
      while (getchar() != '\n')
        ;
      continue;
    }

    switch (choix) {
    case 1: {
      printf("Chemin du fichier (ex: data/reseau.txt) : ");
      scanf("%99s", buffer);
      if (g)
        liberer_graphe(g);
      g = charger_graphe(buffer);
      if (g)
        printf("Graphe chargé !\n");
      else
        printf("Erreur chargement.\n");
      break;
    }
    case 2:
      if (g)
        afficher_graphe(g);
      else
        printf("Aucun graphe chargé.\n");
      break;
    case 3:
      if (g) {
        int src, dest;
        printf("ID Source : ");
        scanf("%d", &src);
        printf("ID Destination : ");
        scanf("%d", &dest);
        ResultatChemin *res = dijkstra(g, src);
        afficher_chemin(res, dest);
        liberer_resultat_chemin(res);
      }
      break;
    case 4:
      if (g) {
        int src, dest;
        printf("ID Source : ");
        scanf("%d", &src);
        printf("ID Destination : ");
        scanf("%d", &dest);
        ResultatChemin *res = bellman_ford(g, src);
        if (res) {
          afficher_chemin(res, dest);
          liberer_resultat_chemin(res);
        }
      }
      break;
    case 5:
      if (g) {
        int src, dest;
        float bp, cout;
        int sec, lat_max;
        printf("ID Source : ");
        scanf("%d", &src);
        printf("ID Destination : ");
        scanf("%d", &dest);
        printf("Min Bande Passante : ");
        scanf("%f", &bp);
        printf("Min Sécurité (0-10) : ");
        scanf("%d", &sec);
        printf("Max Coût : ");
        scanf("%f", &cout);
        printf("Max Latence : ");
        scanf("%d", &lat_max);

        int interdits[1] = {-1};
        int obligatoires[1] = {-1};
        printf("ID Noeud à ÉVITER (-1 si aucun) : ");
        scanf("%d", &interdits[0]);
        printf("ID Noeud OBLIGATOIRE (-1 si aucun) : ");
        scanf("%d", &obligatoires[0]);

        Contraintes c = {.min_bande_passante = bp,
                         .max_cout = cout,
                         .min_securite = sec,
                         .max_latence = lat_max,
                         .nb_interdits = (interdits[0] != -1) ? 1 : 0,
                         .noeuds_interdits = interdits,
                         .nb_obligatoires = (obligatoires[0] != -1) ? 1 : 0,
                         .noeuds_obligatoires = obligatoires};

        ResultatChemin *res = recherche_contrainte(g, src, dest, c);
        if (res) {
          printf("Chemin trouvé !\n");
          afficher_chemin(res, dest);
          liberer_resultat_chemin(res);
        } else {
          printf("Aucun chemin ne respecte ces critères.\n");
        }
      }
      break;
    case 6:
      if (g) {
        printf("\n--- Analyse Sécurité ---\n");
        if (detecter_cycle(g))
          printf("[ALERTE] Cycle détecté !\n");
        else
          printf("[OK] Pas de cycle.\n");

        afficher_points_articulation(g);
        afficher_ponts(g);
        afficher_scc_tarjan(g);
      }
      break;
    case 7: {
      int cycles, cap;
      float taux;
      printf("Nb Cycles (ex: 100) : ");
      scanf("%d", &cycles);
      printf("Capacité File (ex: 10) : ");
      scanf("%d", &cap);
      printf("Taux Arrivée (0.0-1.0) : ");
      scanf("%f", &taux);
      lancer_simulation_flux(cycles, cap, taux);
      break;
    }
    case 8:
      if (g) {
        int src, dest, k;
        printf("ID Source : ");
        scanf("%d", &src);
        printf("ID Destination : ");
        scanf("%d", &dest);
        printf("Combien de chemins (K) ? ");
        scanf("%d", &k);
        afficher_k_plus_courts_chemins(g, src, dest, k);
      }
      break;
    case 9:
      if (g) {
        char nom_fic[100];
        printf("Nom du fichier (.dot) : ");
        scanf("%99s", nom_fic);
        exporter_graphe_dot(g, nom_fic);
      }
      break;
    case 0:
      printf("Au revoir.\n");
      break;
    default:
      printf("Choix invalide.\n");
    }
  }

  if (g)
    liberer_graphe(g);
  return 0;
}
