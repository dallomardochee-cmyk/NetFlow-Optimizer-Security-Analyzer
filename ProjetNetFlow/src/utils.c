#include "utils.h"
#include <stdio.h>

// ========== MENU PRINCIPAL ==========
void menu() {
    printf("\n\033[1;36m================================================\033[0m");
    printf("\n   \033[1mNETFLOW OPTIMIZER - INNOVATION SYSTEM\033[0m");
    printf("\n\033[1;36m================================================\033[0m\n");
    printf("1.  Charger Topologie (.txt)\n");
    printf("2.  Afficher la Topologie Integrale\n");
    printf("3.  Analyse Connectivite & Itineraire (BFS et Dijkstra)\n");
    printf("4.  Bellman-Ford (Verification & Cycles)\n");
    printf("5.  Audit de Fiabilite (SPOF via DFS)\n");
    printf("6.  Backtracking (Optimisation sous contraintes)\n");
    printf("7.  Simulation de Flux (QoS & Priorites)\n");
    printf("8.  Rapport de Performance Exhaustif\n");
    printf("9.  Sauvegarder l'etat du Graphe\n");
    printf("10. Exporter Cartographie (.dot)\n");
    printf("\033[1;33m--- GESTION DYNAMIQUE ---\033[0m\n");
    printf("11. Supprimer un Noeud (Panne totale)\n");
    printf("12. Supprimer une Arete (Coupure de lien)\n");
    printf("13. Ajouter un Noeud (Expansion)\n");
    printf("14. Ajouter une Arete (Nouvelle route)\n");
    printf("15. Creer un reseau geant\n");
    printf("0.  Quitter\n");
    printf("\033[1;36m================================================\033[0m\n");
    printf("\033[1;32mAction > \033[0m");
}