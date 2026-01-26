#ifndef LISTE_CHAINEE_H
#define LISTE_CHAINEE_H

#include <stdio.h>
#include <stdlib.h>

// ============================================================================
// STRUCTURE PAQUET ET FILE D'ATTENTE PRIORITAIRE
// ============================================================================

/**
 * @brief Structure représentant un paquet de données
 */
typedef struct Paquet
{
  int id;                   // Identifiant unique du paquet
  int priorite;             // Priorité (plus haute valeur = plus prioritaire)
  float taille_Mo;          // Taille en Mo
  int source;               // ID du nœud source
  int destination;          // ID du nœud destination
  struct Paquet *precedent; // Pointeur vers le paquet précédent
  struct Paquet *suivant;   // Pointeur vers le paquet suivant
} Paquet;

/**
 * @brief Structure représentant une file d'attente (implémentée via liste
 * doublement chaînée)
 */
typedef struct FileAttente
{
  Paquet *tete;        // Début de la file (plus haute priorité)
  Paquet *queue;       // Fin de la file
  int taille_actuelle; // Nombre de paquets dans la file
  int capacite_max;    // Capacité maximale de la file (si limitée)
} FileAttente;

// Prototypes des fonctions

/**
 * @brief Initialise une nouvelle file d'attente
 * @param capacite_max Capacité maximale de la file
 * @return Pointeur vers la FileAttente créée
 * @complexity O(1)
 */
FileAttente *initialiser_file(int capacite_max);

/**
 * @brief Ajoute un paquet à la file en respectant l'ordre de priorité
 * @param file La file cible
 * @param id Identifiant du paquet
 * @param priorite Priorité du paquet
 * @param taille Taille du paquet
 * @param source Source
 * @param destination Destination
 * @return 1 si succès, 0 si échec (file pleine)
 * @complexity O(n) - insertion triée
 */
int enfiler_paquet(FileAttente *file, int id, int priorite, float taille,
                   int source, int destination);

/**
 * @brief Retire et retourne le paquet le plus prioritaire (tête de file)
 * @param file La file cible
 * @return Pointeur vers le paquet extrait (doit être libéré par l'appelant), ou
 * NULL si vide
 * @complexity O(1)
 */
Paquet *defiler_paquet(FileAttente *file);

/**
 * @brief Consulte le paquet prioritaire sans le retirer
 * @param file La file cible
 * @return Pointeur vers le paquet en tête, ou NULL si vide
 * @complexity O(1)
 */
Paquet *consulter_sommet(const FileAttente *file);

/**
 * @brief Affiche le contenu de la file
 * @param file La file à afficher
 */
void afficher_file(const FileAttente *file);

/**
 * @brief Libère la file et tous ses paquets
 * @param file La file à libérer
 */
void liberer_file(FileAttente *file);

// ============================================================================
// STRUCTURE PILE GÉNÉRIQUE (pour DFS et Backtracking)
// ============================================================================

/**
 * @brief Élément de la pile générique
 */
typedef struct ElementPile
{
  int valeur;
  struct ElementPile *suivant;
} ElementPile;

/**
 * @brief Structure Pile (LIFO)
 */
typedef struct Pile
{
  ElementPile *sommet;
  int taille;
} Pile;

/**
 * @brief Crée une nouvelle pile vide
 * @return Pointeur vers la pile créée
 * @complexity O(1)
 */
Pile *creer_pile(void);

/**
 * @brief Empile un élément
 * @param pile La pile
 * @param valeur Valeur à empiler
 * @complexity O(1)
 */
void empiler(Pile *pile, int valeur);

/**
 * @brief Dépile un élément
 * @param pile La pile
 * @return Valeur dépilée, ou -1 si pile vide
 * @complexity O(1)
 */
int depiler(Pile *pile);

/**
 * @brief Consulte le sommet sans dépiler
 * @param pile La pile
 * @return Valeur au sommet, ou -1 si pile vide
 * @complexity O(1)
 */
int sommet_pile(const Pile *pile);

/**
 * @brief Vérifie si la pile est vide
 * @param pile La pile
 * @return 1 si vide, 0 sinon
 * @complexity O(1)
 */
int pile_est_vide(const Pile *pile);

/**
 * @brief Libère la pile
 * @param pile La pile à libérer
 */
void liberer_pile(Pile *pile);

// ============================================================================
// STRUCTURE FILE FIFO (pour BFS)
// ============================================================================

/**
 * @brief Élément de la file FIFO
 */
typedef struct ElementFileFIFO
{
  int valeur;
  struct ElementFileFIFO *suivant;
} ElementFileFIFO;

/**
 * @brief Structure File FIFO
 */
typedef struct FileFIFO
{
  ElementFileFIFO *tete;
  ElementFileFIFO *queue;
  int taille;
} FileFIFO;

/**
 * @brief Crée une nouvelle file FIFO vide
 * @return Pointeur vers la file créée
 * @complexity O(1)
 */
FileFIFO *creer_file_fifo(void);

/**
 * @brief Enfile un élément (en queue)
 * @param file La file
 * @param valeur Valeur à enfiler
 * @complexity O(1)
 */
void enfiler_fifo(FileFIFO *file, int valeur);

/**
 * @brief Défile un élément (depuis la tête)
 * @param file La file
 * @return Valeur défilée, ou -1 si file vide
 * @complexity O(1)
 */
int defiler_fifo(FileFIFO *file);

/**
 * @brief Consulte la tête sans défiler
 * @param file La file
 * @return Valeur en tête, ou -1 si vide
 * @complexity O(1)
 */
int tete_file_fifo(const FileFIFO *file);

/**
 * @brief Vérifie si la file est vide
 * @param file La file
 * @return 1 si vide, 0 sinon
 * @complexity O(1)
 */
int file_fifo_est_vide(const FileFIFO *file);

/**
 * @brief Libère la file
 * @param file La file à libérer
 */
void liberer_file_fifo(FileFIFO *file);

// ============================================================================
// FILE DE PRIORITÉ POUR DIJKSTRA
// ============================================================================

/**
 * @brief Structure pour un nœud dans la file de priorité de Dijkstra (Min-Heap
 * simulé par liste triée)
 */
typedef struct ElementDijkstra
{
  int id_noeud;
  float distance; // La priorité est la distance (plus petite = prioritaire)
  struct ElementDijkstra *suivant;
} ElementDijkstra;

// --- Fonctions Spécifiques Dijkstra (File de priorité Min avec Float) ---

/**
 * @brief Ajoute un élément dans une liste triée par distance croissante (pour
 * Dijkstra)
 * @param tete Pointeur vers la tête de liste
 * @param id_noeud ID du nœud
 * @param distance Distance accumulée (priorité)
 * @return Nouvelle tête de liste
 * @complexity O(n)
 */
ElementDijkstra *enfiler_dijkstra(ElementDijkstra *tete, int id_noeud,
                                  float distance);

/**
 * @brief Retire l'élément avec la plus petite distance (tête de liste)
 * @param tete Pointeur vers la tête de liste
 * @param id_out Pointeur pour stocker l'ID du nœud extrait
 * @param dist_out Pointeur pour stocker la distance extraite
 * @return Nouvelle tête de liste
 * @complexity O(1)
 */
ElementDijkstra *defiler_dijkstra(ElementDijkstra *tete, int *id_out,
                                  float *dist_out);

/**
 * @brief Libère la liste de priorité Dijkstra
 */
void liberer_liste_dijkstra(ElementDijkstra *tete);

#endif // LISTE_CHAINEE_H
