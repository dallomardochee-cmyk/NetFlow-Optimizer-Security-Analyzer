#include "liste_chainee.h"

FileAttente *initialiser_file(int capacite_max)
{
  FileAttente *file = (FileAttente *)malloc(sizeof(FileAttente));
  if (!file)
  {
    perror("Erreur allocation file");
    exit(EXIT_FAILURE);
  }
  file->tete = NULL;
  file->queue = NULL;
  file->taille_actuelle = 0;
  file->capacite_max = capacite_max;
  return file;
}

int enfiler_paquet(FileAttente *file, int id, int priorite, float taille,
                   int source, int destination)
{
  if (!file)
    return 0;
  if (file->capacite_max > 0 && file->taille_actuelle >= file->capacite_max)
  {
    fprintf(stderr, "File pleine, impossible d'ajouter le paquet %d\n", id);
    return 0;
  }

  Paquet *nouveau = (Paquet *)malloc(sizeof(Paquet));
  if (!nouveau)
  {
    perror("Erreur allocation paquet");
    return 0;
  }
  nouveau->id = id;
  nouveau->priorite = priorite;
  nouveau->taille_Mo = taille;
  nouveau->source = source;
  nouveau->destination = destination;
  nouveau->suivant = NULL;
  nouveau->precedent = NULL;

  // Cas 1 : File vide
  if (file->tete == NULL)
  {
    file->tete = nouveau;
    file->queue = nouveau;
  }
  else
  {
    // Insertion triée par priorité décroissante (plus haute priorité en tête)

    // Si le nouveau est plus prioritaire que la tête
    if (priorite > file->tete->priorite)
    {
      nouveau->suivant = file->tete;
      file->tete->precedent = nouveau;
      file->tete = nouveau;
    }
    else
    {
      // Parcours pour trouver la position
      Paquet *courant = file->tete;
      while (courant->suivant != NULL &&
             courant->suivant->priorite >= priorite)
      {
        courant = courant->suivant;
      }

      // Insertion après courant
      nouveau->suivant = courant->suivant;
      if (courant->suivant != NULL)
      {
        courant->suivant->precedent = nouveau;
      }
      else
      {
        file->queue = nouveau; // Insertion en fin de file
      }
      courant->suivant = nouveau;
      nouveau->precedent = courant;
    }
  }

  file->taille_actuelle++;
  return 1;
}

Paquet *defiler_paquet(FileAttente *file)
{
  if (!file || file->tete == NULL)
    return NULL;

  Paquet *a_retirer = file->tete;

  file->tete = a_retirer->suivant;
  if (file->tete)
  {
    file->tete->precedent = NULL;
  }
  else
  {
    file->queue = NULL; // La file devient vide
  }

  // Isoler le paquet retiré
  a_retirer->suivant = NULL;
  a_retirer->precedent = NULL;

  file->taille_actuelle--;
  return a_retirer;
}

Paquet *consulter_sommet(const FileAttente *file)
{
  if (!file)
    return NULL;
  return file->tete;
}

void afficher_file(const FileAttente *file)
{
  if (!file)
    return;
  printf("=== État de la File (%d/%d) ===\n", file->taille_actuelle,
         file->capacite_max);
  Paquet *courant = file->tete;
  while (courant)
  {
    printf("[ID:%d | Prio:%d | Src:%d -> Dst:%d]\n", courant->id,
           courant->priorite, courant->source, courant->destination);
    courant = courant->suivant;
  }
  printf("==============================\n");
}

void liberer_file(FileAttente *file)
{
  if (!file)
    return;

  Paquet *courant = file->tete;
  while (courant)
  {
    Paquet *a_supprimer = courant;
    courant = courant->suivant;
    free(a_supprimer);
  }
  free(file);
}

// --- Implémentation Dijkstra ---

ElementDijkstra *enfiler_dijkstra(ElementDijkstra *tete, int id_noeud,
                                  float distance)
{
  ElementDijkstra *nouveau = (ElementDijkstra *)malloc(sizeof(ElementDijkstra));
  if (!nouveau)
  {
    perror("Allocation ElementDijkstra");
    return tete;
  }
  nouveau->id_noeud = id_noeud;
  nouveau->distance = distance;
  nouveau->suivant = NULL;

  // Insertion en tête si liste vide ou distance plus petite (Min-Priority)
  if (!tete || distance < tete->distance)
  {
    nouveau->suivant = tete;
    return nouveau;
  }

  // Insertion triée
  ElementDijkstra *courant = tete;
  while (courant->suivant && courant->suivant->distance <= distance)
  {
    courant = courant->suivant;
  }

  nouveau->suivant = courant->suivant;
  courant->suivant = nouveau;

  return tete;
}

ElementDijkstra *defiler_dijkstra(ElementDijkstra *tete, int *id_out,
                                  float *dist_out)
{
  if (!tete)
    return NULL;

  if (id_out)
    *id_out = tete->id_noeud;
  if (dist_out)
    *dist_out = tete->distance;

  ElementDijkstra *suivant = tete->suivant;
  free(tete);
  return suivant;
}

void liberer_liste_dijkstra(ElementDijkstra *tete)
{
  while (tete)
  {
    ElementDijkstra *a_supprimer = tete;
    tete = tete->suivant;
    free(a_supprimer);
  }
}

// ============================================================================
// IMPLÉMENTATION PILE GÉNÉRIQUE (LIFO)
// ============================================================================

Pile *creer_pile(void)
{
  Pile *pile = (Pile *)malloc(sizeof(Pile));
  if (!pile)
  {
    perror("Erreur allocation pile");
    exit(EXIT_FAILURE);
  }
  pile->sommet = NULL;
  pile->taille = 0;
  return pile;
}

void empiler(Pile *pile, int valeur)
{
  if (!pile)
    return;

  ElementPile *nouveau = (ElementPile *)malloc(sizeof(ElementPile));
  if (!nouveau)
  {
    perror("Erreur allocation élément pile");
    return;
  }
  nouveau->valeur = valeur;
  nouveau->suivant = pile->sommet;
  pile->sommet = nouveau;
  pile->taille++;
}

int depiler(Pile *pile)
{
  if (!pile || !pile->sommet)
    return -1;

  ElementPile *a_supprimer = pile->sommet;
  int valeur = a_supprimer->valeur;
  pile->sommet = a_supprimer->suivant;
  free(a_supprimer);
  pile->taille--;
  return valeur;
}

int sommet_pile(const Pile *pile)
{
  if (!pile || !pile->sommet)
    return -1;
  return pile->sommet->valeur;
}

int pile_est_vide(const Pile *pile)
{
  if (!pile)
    return 1;
  return (pile->sommet == NULL);
}

void liberer_pile(Pile *pile)
{
  if (!pile)
    return;

  while (pile->sommet)
  {
    ElementPile *a_supprimer = pile->sommet;
    pile->sommet = a_supprimer->suivant;
    free(a_supprimer);
  }
  free(pile);
}

// ============================================================================
// IMPLÉMENTATION FILE FIFO (pour BFS)
// ============================================================================

FileFIFO *creer_file_fifo(void)
{
  FileFIFO *file = (FileFIFO *)malloc(sizeof(FileFIFO));
  if (!file)
  {
    perror("Erreur allocation file FIFO");
    exit(EXIT_FAILURE);
  }
  file->tete = NULL;
  file->queue = NULL;
  file->taille = 0;
  return file;
}

void enfiler_fifo(FileFIFO *file, int valeur)
{
  if (!file)
    return;

  ElementFileFIFO *nouveau = (ElementFileFIFO *)malloc(sizeof(ElementFileFIFO));
  if (!nouveau)
  {
    perror("Erreur allocation élément file FIFO");
    return;
  }
  nouveau->valeur = valeur;
  nouveau->suivant = NULL;

  if (file->queue == NULL)
  {
    // File vide
    file->tete = nouveau;
    file->queue = nouveau;
  }
  else
  {
    file->queue->suivant = nouveau;
    file->queue = nouveau;
  }
  file->taille++;
}

int defiler_fifo(FileFIFO *file)
{
  if (!file || !file->tete)
    return -1;

  ElementFileFIFO *a_supprimer = file->tete;
  int valeur = a_supprimer->valeur;
  file->tete = a_supprimer->suivant;

  if (file->tete == NULL)
  {
    file->queue = NULL;
  }

  free(a_supprimer);
  file->taille--;
  return valeur;
}

int tete_file_fifo(const FileFIFO *file)
{
  if (!file || !file->tete)
    return -1;
  return file->tete->valeur;
}

int file_fifo_est_vide(const FileFIFO *file)
{
  if (!file)
    return 1;
  return (file->tete == NULL);
}

void liberer_file_fifo(FileFIFO *file)
{
  if (!file)
    return;

  while (file->tete)
  {
    ElementFileFIFO *a_supprimer = file->tete;
    file->tete = a_supprimer->suivant;
    free(a_supprimer);
  }
  free(file);
}
