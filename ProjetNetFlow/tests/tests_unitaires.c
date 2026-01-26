#include "../src/backtracking.h"
#include "../src/bellman_ford.h"
#include "../src/dijkstra.h"
#include "../src/graphe.h"
#include "../src/k_chemins.h"
#include "../src/liste_chainee.h"
#include "../src/securite.h"
#include "../src/utils.h"
#include <assert.h>
#include <float.h>
#include <stdio.h>

// ============================================================================
// TESTS GRAPHE
// ============================================================================

void test_graphe()
{
  printf("[TEST] Structure Graphe... ");
  Graphe *g = creer_graphe(10, 1);
  assert(g != NULL);
  assert(g->nb_noeuds == 0);
  assert(g->est_oriente == 1);
  assert(g->matrice_adjacence != NULL);

  ajouter_noeud(g, 0, "Source");
  ajouter_noeud(g, 1, "Dest"); // Ajout du noeud destination avant !

  assert(g->nb_noeuds == 2);

  ajouter_arete(g, 0, 1, 10.0, 100.0, 0.0, 5);

  // Vérif liste
  assert(g->noeuds[0].aretes != NULL);
  assert(g->noeuds[0].aretes->destination == 1);

  // Vérif matrice
  // Note: ajouter_arete stocke la latence (10.0) dans la matrice
  if (g->matrice_adjacence[0][1] == 10.0)
  {
    // OK
  }
  else
  {
    printf("Matrice mismatch: found %f expected 10.0\n",
           g->matrice_adjacence[0][1]);
    assert(0);
  }

  printf("OK\n");
  liberer_graphe(g);
}

void test_suppression_arete()
{
  printf("[TEST] Suppression arête... ");
  Graphe *g = creer_graphe(10, 1);
  ajouter_noeud(g, 0, "A");
  ajouter_noeud(g, 1, "B");
  ajouter_noeud(g, 2, "C");
  ajouter_arete(g, 0, 1, 10.0, 100.0, 5.0, 5);
  ajouter_arete(g, 0, 2, 20.0, 100.0, 5.0, 5);

  assert(supprimer_arete(g, 0, 1) == 1);
  // Vérifier que l'arête 0->1 n'existe plus
  Arete *a = g->noeuds[0].aretes;
  while (a)
  {
    assert(a->destination != 1);
    a = a->suivant;
  }

  printf("OK\n");
  liberer_graphe(g);
}

void test_suppression_noeud()
{
  printf("[TEST] Suppression nœud... ");
  Graphe *g = creer_graphe(10, 1);
  ajouter_noeud(g, 0, "A");
  ajouter_noeud(g, 1, "B");
  ajouter_noeud(g, 2, "C");
  ajouter_arete(g, 0, 1, 10.0, 100.0, 5.0, 5);
  ajouter_arete(g, 1, 2, 15.0, 100.0, 5.0, 5);

  assert(g->nb_noeuds == 3);
  assert(supprimer_noeud(g, 1) == 1);
  assert(g->nb_noeuds == 2);

  // Vérifier que le nœud 1 n'existe plus
  for (int i = 0; i < g->nb_noeuds; i++)
  {
    assert(g->noeuds[i].id != 1);
  }

  printf("OK\n");
  liberer_graphe(g);
}

// ============================================================================
// TESTS FILE DE PRIORITÉ
// ============================================================================

void test_file()
{
  printf("[TEST] File de priorité... ");
  FileAttente *f = initialiser_file(10);
  enfiler_paquet(f, 1, 10, 1.0, 0, 1);
  enfiler_paquet(f, 2, 20, 1.0, 0, 1);

  Paquet *p = defiler_paquet(f);
  assert(p->id == 2);
  free(p);

  Paquet *p2 = defiler_paquet(f);
  if (p2)
    free(p2);
  liberer_file(f);
  printf("OK\n");
}

// ============================================================================
// TESTS PILE
// ============================================================================

void test_pile()
{
  printf("[TEST] Pile générique... ");
  Pile *pile = creer_pile();
  assert(pile_est_vide(pile) == 1);

  empiler(pile, 10);
  empiler(pile, 20);
  empiler(pile, 30);

  assert(pile_est_vide(pile) == 0);
  assert(sommet_pile(pile) == 30);
  assert(depiler(pile) == 30);
  assert(depiler(pile) == 20);
  assert(depiler(pile) == 10);
  assert(pile_est_vide(pile) == 1);
  assert(depiler(pile) == -1); // Pile vide

  liberer_pile(pile);
  printf("OK\n");
}

// ============================================================================
// TESTS FILE FIFO
// ============================================================================

void test_file_fifo()
{
  printf("[TEST] File FIFO... ");
  FileFIFO *file = creer_file_fifo();
  assert(file_fifo_est_vide(file) == 1);

  enfiler_fifo(file, 10);
  enfiler_fifo(file, 20);
  enfiler_fifo(file, 30);

  assert(file_fifo_est_vide(file) == 0);
  assert(tete_file_fifo(file) == 10);
  assert(defiler_fifo(file) == 10);
  assert(defiler_fifo(file) == 20);
  assert(defiler_fifo(file) == 30);
  assert(file_fifo_est_vide(file) == 1);

  liberer_file_fifo(file);
  printf("OK\n");
}

// ============================================================================
// TESTS DIJKSTRA
// ============================================================================

void test_dijkstra()
{
  printf("[TEST] Dijkstra... ");
  Graphe *g = creer_graphe(10, 1);
  ajouter_noeud(g, 0, "A");
  ajouter_noeud(g, 1, "B");
  ajouter_noeud(g, 2, "C");
  ajouter_noeud(g, 3, "D");

  // A -> B (5), A -> C (10), B -> C (2), B -> D (1), C -> D (3)
  ajouter_arete(g, 0, 1, 5.0, 100.0, 5.0, 5);
  ajouter_arete(g, 0, 2, 10.0, 100.0, 5.0, 5);
  ajouter_arete(g, 1, 2, 2.0, 100.0, 5.0, 5);
  ajouter_arete(g, 1, 3, 1.0, 100.0, 5.0, 5);
  ajouter_arete(g, 2, 3, 3.0, 100.0, 5.0, 5);

  ResultatChemin *res = dijkstra(g, 0);
  assert(res != NULL);

  // Distance A->A = 0
  assert(res->distances[0] == 0.0);
  // Distance A->B = 5
  assert(res->distances[1] == 5.0);
  // Distance A->D = 5 + 1 = 6 (via B)
  assert(res->distances[3] == 6.0);
  // Distance A->C = 5 + 2 = 7 (via B)
  assert(res->distances[2] == 7.0);

  liberer_resultat_chemin(res);
  liberer_graphe(g);
  printf("OK\n");
}

// ============================================================================
// TESTS BELLMAN-FORD
// ============================================================================

void test_bellman_ford()
{
  printf("[TEST] Bellman-Ford... ");
  Graphe *g = creer_graphe(10, 1);
  ajouter_noeud(g, 0, "A");
  ajouter_noeud(g, 1, "B");
  ajouter_noeud(g, 2, "C");

  ajouter_arete(g, 0, 1, 5.0, 100.0, 5.0, 5);
  ajouter_arete(g, 1, 2, 3.0, 100.0, 5.0, 5);
  ajouter_arete(g, 0, 2, 10.0, 100.0, 5.0, 5);

  ResultatChemin *res = bellman_ford(g, 0);
  assert(res != NULL);

  assert(res->distances[0] == 0.0);
  assert(res->distances[1] == 5.0);
  assert(res->distances[2] == 8.0); // 5 + 3 via B

  liberer_resultat_chemin(res);
  liberer_graphe(g);
  printf("OK\n");
}

// ============================================================================
// TESTS DÉTECTION DE CYCLE
// ============================================================================

void test_detection_cycle()
{
  printf("[TEST] Détection cycle DFS... ");

  // Graphe SANS cycle
  Graphe *g1 = creer_graphe(10, 1);
  ajouter_noeud(g1, 0, "A");
  ajouter_noeud(g1, 1, "B");
  ajouter_noeud(g1, 2, "C");
  ajouter_arete(g1, 0, 1, 5.0, 100.0, 5.0, 5);
  ajouter_arete(g1, 1, 2, 3.0, 100.0, 5.0, 5);

  assert(detecter_cycle(g1) == 0);
  liberer_graphe(g1);

  // Graphe AVEC cycle (A -> B -> C -> A)
  Graphe *g2 = creer_graphe(10, 1);
  ajouter_noeud(g2, 0, "A");
  ajouter_noeud(g2, 1, "B");
  ajouter_noeud(g2, 2, "C");
  ajouter_arete(g2, 0, 1, 5.0, 100.0, 5.0, 5);
  ajouter_arete(g2, 1, 2, 3.0, 100.0, 5.0, 5);
  ajouter_arete(g2, 2, 0, 2.0, 100.0, 5.0, 5); // Cycle!

  assert(detecter_cycle(g2) == 1);
  liberer_graphe(g2);

  printf("OK\n");
}

void test_detection_cycle_bfs()
{
  printf("[TEST] Détection cycle BFS (Kahn)... ");

  // Graphe SANS cycle
  Graphe *g1 = creer_graphe(10, 1);
  ajouter_noeud(g1, 0, "A");
  ajouter_noeud(g1, 1, "B");
  ajouter_noeud(g1, 2, "C");
  ajouter_arete(g1, 0, 1, 5.0, 100.0, 5.0, 5);
  ajouter_arete(g1, 1, 2, 3.0, 100.0, 5.0, 5);

  assert(detecter_cycle_bfs(g1) == 0);
  liberer_graphe(g1);

  // Graphe AVEC cycle
  Graphe *g2 = creer_graphe(10, 1);
  ajouter_noeud(g2, 0, "A");
  ajouter_noeud(g2, 1, "B");
  ajouter_noeud(g2, 2, "C");
  ajouter_arete(g2, 0, 1, 5.0, 100.0, 5.0, 5);
  ajouter_arete(g2, 1, 2, 3.0, 100.0, 5.0, 5);
  ajouter_arete(g2, 2, 0, 2.0, 100.0, 5.0, 5);

  assert(detecter_cycle_bfs(g2) == 1);
  liberer_graphe(g2);

  printf("OK\n");
}

// ============================================================================
// TESTS BACKTRACKING
// ============================================================================

void test_backtracking()
{
  printf("[TEST] Backtracking avec contraintes... ");

  Graphe *g = creer_graphe(10, 1);
  ajouter_noeud(g, 0, "A");
  ajouter_noeud(g, 1, "B");
  ajouter_noeud(g, 2, "C");
  ajouter_noeud(g, 3, "D");

  // Chemin 1: A -> B -> D (latence 5+1=6, BP 100, sécu 5)
  // Chemin 2: A -> C -> D (latence 10+3=13, BP 200, sécu 8)
  ajouter_arete(g, 0, 1, 5.0, 100.0, 5.0, 5);
  ajouter_arete(g, 0, 2, 10.0, 200.0, 3.0, 8);
  ajouter_arete(g, 1, 3, 1.0, 100.0, 2.0, 5);
  ajouter_arete(g, 2, 3, 3.0, 200.0, 1.0, 8);

  // Sans contraintes particulières, le chemin le plus court est A->B->D
  Contraintes c1 = {.min_bande_passante = 0,
                    .max_cout = 1000,
                    .min_securite = 0,
                    .max_latence = 1000,
                    .noeuds_obligatoires = NULL,
                    .nb_obligatoires = 0,
                    .noeuds_interdits = NULL,
                    .nb_interdits = 0};

  ResultatChemin *res1 = recherche_contrainte(g, 0, 3, c1);
  assert(res1 != NULL);
  assert(res1->distances[3] == 6.0); // A->B->D
  liberer_resultat_chemin(res1);

  // Avec contrainte BP >= 150, doit passer par C
  Contraintes c2 = {.min_bande_passante = 150,
                    .max_cout = 1000,
                    .min_securite = 0,
                    .max_latence = 1000,
                    .noeuds_obligatoires = NULL,
                    .nb_obligatoires = 0,
                    .noeuds_interdits = NULL,
                    .nb_interdits = 0};

  ResultatChemin *res2 = recherche_contrainte(g, 0, 3, c2);
  assert(res2 != NULL);
  assert(res2->distances[3] == 13.0); // A->C->D
  liberer_resultat_chemin(res2);

  liberer_graphe(g);
  printf("OK\n");
}

// ============================================================================
// TESTS CHARGEMENT/SAUVEGARDE
// ============================================================================

void test_chargement_sauvegarde()
{
  printf("[TEST] Chargement/Sauvegarde graphe... ");

  Graphe *g = creer_graphe(10, 0);
  ajouter_noeud(g, 0, "Test1");
  ajouter_noeud(g, 1, "Test2");
  ajouter_arete(g, 0, 1, 15.0, 500.0, 10.0, 7);

  // Sauvegarder
  assert(sauvegarder_graphe(g, "data/test_unitaire_tmp.txt") == 1);

  // Recharger
  Graphe *g2 = charger_graphe("data/test_unitaire_tmp.txt");
  assert(g2 != NULL);
  assert(g2->nb_noeuds == 2);

  liberer_graphe(g);
  liberer_graphe(g2);

  // Nettoyer le fichier temporaire
  remove("data/test_unitaire_tmp.txt");

  printf("OK\n");
}

// ============================================================================
// MAIN
// ============================================================================

int main()
{
  printf("=== Démarrage des Tests Unitaires Automatisés ===\n\n");

  // Tests structures de base
  test_graphe();
  test_suppression_arete();
  test_suppression_noeud();
  test_file();
  test_pile();
  test_file_fifo();

  // Tests algorithmes
  test_dijkstra();
  test_bellman_ford();
  test_detection_cycle();
  test_detection_cycle_bfs();
  test_backtracking();

  // Tests utilitaires
  test_chargement_sauvegarde();

  printf("\n=== Tous les tests unitaires ont réussi ! ===\n");
  return 0;
}
