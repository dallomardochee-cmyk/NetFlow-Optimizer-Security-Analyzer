#ifndef GRAPHE_H
#define GRAPHE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Définition des structures comme demandé

/**
 * @brief Structure représentant une arête du graphe
 */
typedef struct Arete
{
    int destination;       // ID du nœud destination
    float latence;         // Latence en ms
    float bande_passante;  // Bande passante en Mbps
    float cout;            // Coût en unités monétaires
    int securite;          // Niveau de sécurité (0-10)
    struct Arete *suivant; // Pointeur vers l'arête suivante (liste chaînée)
} Arete;

/**
 * @brief Structure représentant un nœud du graphe
 */
typedef struct Noeud
{
    int id;        // Identifiant unique du nœud
    char nom[50];  // Nom du nœud (ex: "Routeur_A")
    Arete *aretes; // Liste d'adjacence des arêtes partantes
} Noeud;

/**
 * @brief Structure principale du graphe
 */
typedef struct Graphe
{
    int nb_noeuds;             // Nombre total de nœuds
    int capacite_noeuds;       // Capacité actuelle du tableau de nœuds
    Noeud *noeuds;             // Tableau dynamique de nœuds
    float **matrice_adjacence; // Matrice d'adjacence (optionnelle, pour graphes denses)
    int est_oriente;           // 1 si orienté, 0 sinon
} Graphe;

// Prototypes des fonctions

/**
 * @brief Crée un nouveau graphe vide
 * @param capacite_initiale Nombre de nœuds initial à allouer
 * @param est_oriente 1 pour un graphe orienté, 0 pour non-orienté
 * @return Pointeur vers le Graphe créé
 */
Graphe *creer_graphe(int capacite_initiale, int est_oriente);

/**
 * @brief Libère toute la mémoire associée au graphe
 * @param graphe Le graphe à détruire
 */
void liberer_graphe(Graphe *graphe);

/**
 * @brief Ajoute un nœud au graphe
 * @param graphe Le graphe cible
 * @param id Identifiant du nœud
 * @param nom Nom du nœud
 */
void ajouter_noeud(Graphe *graphe, int id, const char *nom);

/**
 * @brief Ajoute une arête entre deux nœuds
 * @param graphe Le graphe cible
 * @param source ID du nœud source
 * @param destination ID du nœud destination
 * @param latence Latence de la connexion
 * @param bande_passante Bande passante disponible
 * @param cout Coût de la liaison
 * @param securite Niveau de sécurité
 */
void ajouter_arete(Graphe *graphe, int source, int destination, float latence, float bande_passante, float cout, int securite);

/**
 * @brief Supprime un nœud du graphe (et toutes ses arêtes)
 * @param graphe Le graphe cible
 * @param id ID du nœud à supprimer
 * @return 1 si succès, 0 si nœud non trouvé
 * @complexity O(V + E) - parcourt tous les nœuds et arêtes
 */
int supprimer_noeud(Graphe *graphe, int id);

/**
 * @brief Supprime une arête entre deux nœuds
 * @param graphe Le graphe cible
 * @param source ID du nœud source
 * @param destination ID du nœud destination
 * @return 1 si succès, 0 si arête non trouvée
 * @complexity O(V + deg(source)) - recherche linéaire
 */
int supprimer_arete(Graphe *graphe, int source, int destination);

/**
 * @brief Affiche le graphe (pour le débogage)
 * @param graphe Le graphe à afficher
 */
void afficher_graphe(const Graphe *graphe);

#endif // GRAPHE_H
