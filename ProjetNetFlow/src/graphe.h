#ifndef GRAPHE_H
#define GRAPHE_H

// 1. L'Arête : C'est le lien (le câble) entre deux routeurs
typedef struct Arete {
    int destination;        // Vers quel routeur le câble va
    float latence;          // Le retard (ms)
    float bande_passante;   // La vitesse (Mbps)
    int securite;           // Note de 0 à 10
    struct Arete* suivant;  // Le lien vers le voisin suivant (liste chaînée)
} Arete;

// 2. Le Noeud : C'est le routeur lui-même
typedef struct Noeud {
    int id;                 // Son numéro (0, 1, 2...)
    char nom[50];           // Son nom (ex: "Routeur_A")
    Arete* liste_voisins;   // La liste des câbles qui partent de lui
} Noeud;

// 3. Le Graphe : C'est la carte complète du réseau
typedef struct Graphe {
    int nb_noeuds;          // Combien de routeurs au total
    Noeud* tab_noeuds;      // Le tableau qui contient tous les routeurs
} Graphe;

#endif