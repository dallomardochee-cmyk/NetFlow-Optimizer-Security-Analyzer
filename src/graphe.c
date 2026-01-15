#include "graphe.h"

Graphe *creer_graphe(int capacite_initiale, int est_oriente)
{
  if (capacite_initiale <= 0)
    capacite_initiale = 10;

  Graphe *graphe = (Graphe *)malloc(sizeof(Graphe));
  if (!graphe)
  {
    perror("Erreur d'allocation du graphe");
    exit(EXIT_FAILURE);
  }

  graphe->nb_noeuds = 0;
  graphe->capacite_noeuds = capacite_initiale;
  graphe->est_oriente = est_oriente;
  graphe->noeuds = (Noeud *)calloc(capacite_initiale, sizeof(Noeud));

  // Initialisation de la matrice d'adjacence
  // Allocation dynamique d'un tableau de pointeurs
  graphe->matrice_adjacence =
      (float **)malloc(capacite_initiale * sizeof(float *));
  if (graphe->matrice_adjacence)
  {
    for (int i = 0; i < capacite_initiale; i++)
    {
      graphe->matrice_adjacence[i] =
          (float *)malloc(capacite_initiale * sizeof(float));
      // Init à -1 ou INF pour dire "pas d'arête"
      for (int j = 0; j < capacite_initiale; j++)
        graphe->matrice_adjacence[i][j] = -1.0;
    }
  }

  if (!graphe->noeuds)
  {
    perror("Erreur d'allocation des nœuds");
    free(graphe);
    exit(EXIT_FAILURE);
  }

  return graphe;
}

void liberer_graphe(Graphe *graphe)
{
  if (!graphe)
    return;

  // Libérer les arêtes pour chaque nœud
  for (int i = 0; i < graphe->nb_noeuds; i++)
  {
    Arete *courante = graphe->noeuds[i].aretes;
    while (courante)
    {
      Arete *a_supprimer = courante;
      courante = courante->suivant;
      free(a_supprimer);
    }
  }

  // Libérer le tableau de nœuds
  free(graphe->noeuds);

  // Libérer la matrice d'adjacence
  if (graphe->matrice_adjacence)
  {
    for (int i = 0; i < graphe->capacite_noeuds; i++)
    {
      free(graphe->matrice_adjacence[i]);
    }
    free(graphe->matrice_adjacence);
  }

  free(graphe);
}

void ajouter_noeud(Graphe *graphe, int id, const char *nom)
{
  if (!graphe)
    return;

  // Vérifier si redimensionnement nécessaire
  if (graphe->nb_noeuds >= graphe->capacite_noeuds)
  {
    int ancienne_cap = graphe->capacite_noeuds;
    int nouvelle_capacite = graphe->capacite_noeuds * 2;

    // 1. Realloc Noeuds
    Noeud *nouveaux_noeuds =
        (Noeud *)realloc(graphe->noeuds, nouvelle_capacite * sizeof(Noeud));
    if (!nouveaux_noeuds)
    {
      perror("Erreur de redimensionnement des nœuds");
      return;
    }
    graphe->noeuds = nouveaux_noeuds;

    // Init nouveaux noeuds
    for (int i = ancienne_cap; i < nouvelle_capacite; i++)
    {
      graphe->noeuds[i].id = -1;
      graphe->noeuds[i].aretes = NULL;
      strcpy(graphe->noeuds[i].nom, "");
    }

    // 2. Realloc Matrice
    if (graphe->matrice_adjacence)
    {
      float **nouvelle_matrice = (float **)realloc(
          graphe->matrice_adjacence, nouvelle_capacite * sizeof(float *));
      if (nouvelle_matrice)
      {
        graphe->matrice_adjacence = nouvelle_matrice;
        // Allouer les nouvelles lignes
        for (int i = ancienne_cap; i < nouvelle_capacite; i++)
        {
          graphe->matrice_adjacence[i] =
              (float *)malloc(nouvelle_capacite * sizeof(float));
          for (int j = 0; j < nouvelle_capacite; j++)
            graphe->matrice_adjacence[i][j] = -1.0;
        }
        // Realloc les anciennes lignes pour agrandir les colonnes
        for (int i = 0; i < ancienne_cap; i++)
        {
          float *nouvelle_ligne = (float *)realloc(
              graphe->matrice_adjacence[i], nouvelle_capacite * sizeof(float));
          if (nouvelle_ligne)
          {
            graphe->matrice_adjacence[i] = nouvelle_ligne;
            for (int j = ancienne_cap; j < nouvelle_capacite; j++)
              graphe->matrice_adjacence[i][j] = -1.0;
          }
        }
      }
    }

    graphe->capacite_noeuds = nouvelle_capacite;
  }

  // Vérifions si l'ID existe déjà pour éviter les doublons
  for (int i = 0; i < graphe->nb_noeuds; i++)
  {
    if (graphe->noeuds[i].id == id)
    {
      strncpy(graphe->noeuds[i].nom, nom, 49);
      graphe->noeuds[i].nom[49] = '\0';
      return;
    }
  }

  int index = graphe->nb_noeuds;
  graphe->noeuds[index].id = id;
  strncpy(graphe->noeuds[index].nom, nom, 49);
  graphe->noeuds[index].nom[49] = '\0';
  graphe->noeuds[index].aretes = NULL;

  graphe->nb_noeuds++;
}

void ajouter_arete(Graphe *graphe, int source, int destination, float latence,
                   float bande_passante, float cout, int securite)
{
  if (!graphe)
    return;

  // Trouver l'index du nœud source
  int index_source = -1;
  int index_dest = -1;
  for (int i = 0; i < graphe->nb_noeuds; i++)
  {
    if (graphe->noeuds[i].id == source)
      index_source = i;
    if (graphe->noeuds[i].id == destination)
      index_dest = i;
  }

  if (index_source == -1)
  {
    fprintf(stderr, "Erreur: Nœud source %d inexistant.\n", source);
    return;
  }

  // Création de l'arête (Liste Adjacence)
  Arete *nouvelle_arete = (Arete *)malloc(sizeof(Arete));
  if (!nouvelle_arete)
  {
    perror("Erreur allocation arête");
    return;
  }
  nouvelle_arete->destination = destination;
  nouvelle_arete->latence = latence;
  nouvelle_arete->bande_passante = bande_passante;
  nouvelle_arete->cout = cout;
  nouvelle_arete->securite = securite;

  nouvelle_arete->suivant = graphe->noeuds[index_source].aretes;
  graphe->noeuds[index_source].aretes = nouvelle_arete;

  // Mise à jour Matrice Adjacence (Stockage Latence par défaut dans la matrice
  // simple)
  if (graphe->matrice_adjacence && index_source < graphe->capacite_noeuds &&
      index_dest != -1 && index_dest < graphe->capacite_noeuds)
  {
    graphe->matrice_adjacence[index_source][index_dest] = latence;
  }

  // Si non orienté, ajouter l'arête inverse
  if (!graphe->est_oriente)
  {
    if (index_dest != -1)
    {
      Arete *arete_inverse = (Arete *)malloc(sizeof(Arete));
      if (arete_inverse)
      {
        arete_inverse->destination = source;
        arete_inverse->latence = latence;
        arete_inverse->bande_passante = bande_passante;
        arete_inverse->cout = cout;
        arete_inverse->securite = securite;
        arete_inverse->suivant = graphe->noeuds[index_dest].aretes;
        graphe->noeuds[index_dest].aretes = arete_inverse;

        // Matrice symétrique
        if (graphe->matrice_adjacence && index_dest < graphe->capacite_noeuds &&
            index_source < graphe->capacite_noeuds)
        {
          graphe->matrice_adjacence[index_dest][index_source] = latence;
        }
      }
    }
  }
}

/**
 * @brief Supprime une arête entre deux nœuds
 * @complexity O(V + deg(source))
 */
int supprimer_arete(Graphe *graphe, int source, int destination)
{
  if (!graphe)
    return 0;

  // Trouver l'index du nœud source
  int index_source = -1;
  int index_dest = -1;
  for (int i = 0; i < graphe->nb_noeuds; i++)
  {
    if (graphe->noeuds[i].id == source)
      index_source = i;
    if (graphe->noeuds[i].id == destination)
      index_dest = i;
  }

  if (index_source == -1)
    return 0;

  // Supprimer l'arête source -> destination
  Arete *prev = NULL;
  Arete *curr = graphe->noeuds[index_source].aretes;
  int trouve = 0;

  while (curr)
  {
    if (curr->destination == destination)
    {
      if (prev)
        prev->suivant = curr->suivant;
      else
        graphe->noeuds[index_source].aretes = curr->suivant;
      free(curr);
      trouve = 1;
      break;
    }
    prev = curr;
    curr = curr->suivant;
  }

  // Mise à jour matrice d'adjacence
  if (trouve && graphe->matrice_adjacence && index_dest != -1 &&
      index_source < graphe->capacite_noeuds &&
      index_dest < graphe->capacite_noeuds)
  {
    graphe->matrice_adjacence[index_source][index_dest] = -1.0;
  }

  // Si non orienté, supprimer aussi l'arête inverse
  if (!graphe->est_oriente && index_dest != -1)
  {
    prev = NULL;
    curr = graphe->noeuds[index_dest].aretes;
    while (curr)
    {
      if (curr->destination == source)
      {
        if (prev)
          prev->suivant = curr->suivant;
        else
          graphe->noeuds[index_dest].aretes = curr->suivant;
        free(curr);
        break;
      }
      prev = curr;
      curr = curr->suivant;
    }
    // Matrice symétrique
    if (graphe->matrice_adjacence)
    {
      graphe->matrice_adjacence[index_dest][index_source] = -1.0;
    }
  }

  return trouve;
}

/**
 * @brief Supprime un nœud du graphe et toutes ses arêtes
 * @complexity O(V + E)
 */
int supprimer_noeud(Graphe *graphe, int id)
{
  if (!graphe)
    return 0;

  // Trouver l'index du nœud à supprimer
  int index = -1;
  for (int i = 0; i < graphe->nb_noeuds; i++)
  {
    if (graphe->noeuds[i].id == id)
    {
      index = i;
      break;
    }
  }

  if (index == -1)
    return 0;

  // 1. Libérer toutes les arêtes sortantes du nœud
  Arete *courante = graphe->noeuds[index].aretes;
  while (courante)
  {
    Arete *a_supprimer = courante;
    courante = courante->suivant;
    free(a_supprimer);
  }

  // 2. Supprimer toutes les arêtes entrantes (des autres nœuds vers ce nœud)
  for (int i = 0; i < graphe->nb_noeuds; i++)
  {
    if (i == index)
      continue;

    Arete *prev = NULL;
    Arete *curr = graphe->noeuds[i].aretes;
    while (curr)
    {
      if (curr->destination == id)
      {
        Arete *a_supprimer = curr;
        if (prev)
          prev->suivant = curr->suivant;
        else
          graphe->noeuds[i].aretes = curr->suivant;
        curr = curr->suivant;
        free(a_supprimer);
      }
      else
      {
        prev = curr;
        curr = curr->suivant;
      }
    }
  }

  // 3. Mise à jour matrice d'adjacence (mettre ligne et colonne à -1)
  if (graphe->matrice_adjacence && index < graphe->capacite_noeuds)
  {
    for (int j = 0; j < graphe->capacite_noeuds; j++)
    {
      graphe->matrice_adjacence[index][j] = -1.0;
      graphe->matrice_adjacence[j][index] = -1.0;
    }
  }

  // 4. Décaler les nœuds suivants dans le tableau
  for (int i = index; i < graphe->nb_noeuds - 1; i++)
  {
    graphe->noeuds[i] = graphe->noeuds[i + 1];
  }

  // 5. Réinitialiser le dernier emplacement
  graphe->noeuds[graphe->nb_noeuds - 1].id = -1;
  graphe->noeuds[graphe->nb_noeuds - 1].aretes = NULL;
  strcpy(graphe->noeuds[graphe->nb_noeuds - 1].nom, "");

  graphe->nb_noeuds--;

  return 1;
}

void afficher_graphe(const Graphe *graphe)
{
  if (!graphe)
    return;
  printf("=== Affichage du Graphe (%d nœuds) ===\n", graphe->nb_noeuds);
  for (int i = 0; i < graphe->nb_noeuds; i++)
  {
    printf("Nœud %d (%s) :", graphe->noeuds[i].id, graphe->noeuds[i].nom);
    Arete *courante = graphe->noeuds[i].aretes;
    while (courante)
    {
      printf(" -> [Dest:%d | Lat:%.1f | BP:%.1f | Séc:%d]",
             courante->destination, courante->latence, courante->bande_passante,
             courante->securite);
      courante = courante->suivant;
    }
    printf("\n");
  }
  printf("========================================\n");
}
