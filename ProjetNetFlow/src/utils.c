#include "utils.h"
#include <ctype.h>

/*
Format de fichier attendu (.txt) :
---
NB_NOEUDS 3
ORIENTE 0 (ou 1)
NOEUD <id> <nom>
ARETE <src> <dest> <latence> <bypass> <cout> <secu>
...
---
*/

Graphe *charger_graphe(const char *chemin_fichier) {
  FILE *fichier = fopen(chemin_fichier, "r");
  if (!fichier) {
    perror("Erreur d'ouverture du fichier");
    return NULL;
  }

  char ligne[MAX_LINE_LENGTH];
  Graphe *graphe = NULL;
  int est_oriente = 0; // Par défaut non orienté

  while (fgets(ligne, sizeof(ligne), fichier)) {
    // Ignorer commentaires (#) et lignes vides
    if (ligne[0] == '#' || ligne[0] == '\n' || ligne[0] == '\r')
      continue;

    char type[20];
    if (sscanf(ligne, "%s", type) != 1)
      continue;

    if (strcmp(type, "NB_NOEUDS") == 0) {
      // Optionnel si on redimensionne dynamiquement, mais utile pour init
      // On l'ignore ici car on crée le graphe au premier NOEUD ou on attend.
      // Mais pour faire propre, on peut lire ORIENTE avant.
    } else if (strcmp(type, "ORIENTE") == 0) {
      int val;
      sscanf(ligne, "%*s %d", &val);
      est_oriente = val;
      // On initialise le graphe ici si pas encore fait
      if (!graphe)
        graphe = creer_graphe(10, est_oriente);
    } else if (strcmp(type, "NOEUD") == 0) {
      if (!graphe)
        graphe = creer_graphe(10, est_oriente);
      int id;
      char nom[50];
      if (sscanf(ligne, "%*s %d %49s", &id, nom) == 2) {
        ajouter_noeud(graphe, id, nom);
      }
    } else if (strcmp(type, "ARETE") == 0) {
      if (!graphe)
        continue; // Doit avoir des nœuds avant
      int src, dest, secu;
      float lat, bp, cout;
      if (sscanf(ligne, "%*s %d %d %f %f %f %d", &src, &dest, &lat, &bp, &cout,
                 &secu) == 6) {
        ajouter_arete(graphe, src, dest, lat, bp, cout, secu);
      }
    }
  }

  if (!graphe) {
    // Si le fichier était vide ou sans instructions valides
    // On crée un graphe vide par défaut pour éviter de retourner NULL sans
    // raison grave
    graphe = creer_graphe(5, 0);
  }

  fclose(fichier);
  return graphe;
}

int sauvegarder_graphe(const Graphe *graphe, const char *chemin_fichier) {
  if (!graphe)
    return 0;

  FILE *fichier = fopen(chemin_fichier, "w");
  if (!fichier) {
    perror("Erreur création fichier sauvegarde");
    return 0;
  }

  fprintf(fichier, "# Sauvegarde Graphe NetFlow\n");
  fprintf(fichier, "NB_NOEUDS %d\n", graphe->nb_noeuds);
  fprintf(fichier, "ORIENTE %d\n", graphe->est_oriente);

  // Sauvegarde des nœuds
  for (int i = 0; i < graphe->nb_noeuds; i++) {
    fprintf(fichier, "NOEUD %d %s\n", graphe->noeuds[i].id,
            graphe->noeuds[i].nom);
  }

  // Sauvegarde des arêtes
  // Attention : pour un graphe non orienté, on risque de sauvegarder A->B et
  // B->A (doublons) Pour simplifier, on sauvegarde tout, le chargement gérera
  // (ou recréera des doublons si graphe orienté) Si non-orienté, ajouter_arete
  // ajoute les deux sens. Si on recharge tout, on aura 4 arêtes... Idéalement
  // il faut filtrer. Solution simple : On sauvegarde tout. Lors du chargement,
  // si on est en "non orienté", 'ajouter_arete' ajoute A->B et B->A. Si le
  // fichier contient ARETE A B et ARETE B A, on aura des doublons si on ne fait
  // pas gaffe. Mais 'ajouter_arete' ajoute simplement en tête de liste.
  // => Risque de multiplication des arêtes.
  // Pour l'instant, faisons simple : on sauvegarde tout tel que stocké.
  // L'utilisateur devra être conscient que le rechargement d'un graphe
  // non-orienté sauvegardé tel quel pourrait doubler les liens si la fonction
  // `ajouter_arete` est naïve. Mieux : Si non-orienté, on sauvegarde l'arête
  // seulement si src < dest pour éviter doublons

  for (int i = 0; i < graphe->nb_noeuds; i++) {
    Arete *courante = graphe->noeuds[i].aretes;
    while (courante) {
      if (graphe->est_oriente ||
          (graphe->noeuds[i].id < courante->destination)) {
        fprintf(fichier, "ARETE %d %d %.2f %.2f %.2f %d\n",
                graphe->noeuds[i].id, courante->destination, courante->latence,
                courante->bande_passante, courante->cout, courante->securite);
      }
      courante = courante->suivant;
    }
  }

  fclose(fichier);
  return 1;
}

void exporter_graphe_dot(const Graphe *graphe, const char *nom_fichier) {
  if (!graphe || !nom_fichier)
    return;

  FILE *f = fopen(nom_fichier, "w");
  if (!f) {
    perror("Erreur création fichier DOT");
    return;
  }

  if (graphe->est_oriente)
    fprintf(f, "digraph G {\n");
  else
    fprintf(f, "graph G {\n");

  // Noeuds
  for (int i = 0; i < graphe->nb_noeuds; i++) {
    // Nettoyage nom pour affichage (retrait potentiel retours ligne)
    fprintf(f, "    N%d [label=\"%s (ID:%d)\"];\n", graphe->noeuds[i].id,
            graphe->noeuds[i].nom, graphe->noeuds[i].id);
  }

  // Arêtes
  for (int i = 0; i < graphe->nb_noeuds; i++) {
    Arete *a = graphe->noeuds[i].aretes;
    while (a) {
      // Pour graphes non orientés, on évite les doublons A->B et B->A
      if (!graphe->est_oriente && a->destination < graphe->noeuds[i].id) {
        a = a->suivant;
        continue;
      }

      fprintf(f, "    N%d -> N%d [label=\"L:%.0f BP:%.0f\"];\n",
              graphe->noeuds[i].id, a->destination, a->latence,
              a->bande_passante);
      a = a->suivant;
    }
  }

  fprintf(f, "}\n");
  fclose(f);
  printf("Graphe exporté dans '%s'. Utilisez 'dot -Tpng %s -o graphe.png' pour "
         "visualiser.\n",
         nom_fichier, nom_fichier);
}
