#include "utils.h"
#include <stdio.h>

void simulation_flux(float go) {
    printf("\n\033[1;36m--- INNOVATION : SIMULATION DATACENTER (%.2f Go) ---\033[0m\n", go);
    float temps = (go * 1024 * 8) / 10000.0; // 10 Gbps
    printf("Temps de transfert estimé : \033[1;33m%.2f secondes\033[0m\n", temps);
}
void menu() {
    printf("\n\033[1;36m================================================\033[0m");
    printf("\n   \033[1mNETFLOW OPTIMIZER - INNOVATION SYSTEM\033[0m");
    printf("\n\033[1;36m================================================\033[0m\n");
    printf("1.  Charger Topologie (Saisie Manuelle / Fichier)\n");
    printf("2.  Afficher la Topologie Integrale \n");
    printf("3.  Analyse Connectivite & Itineraire (BFS + Dijkstra)\n");
    printf("4.  Bellman-Ford (Verification + CPU)\n");
    printf("5.  Audit de Fiabilite (SPOF via DFS)\n");
    printf("6.  Backtracking (Contraintes Bande Passante/Coût)\n");
    printf("7.  Simulation de Flux (QoS & Priorites)\n");
    printf("8.  Rapport de Performance Exhaustif (RAM/Topologie)\n");
    printf("9.  Sauvegarder l'etat du Graphe\n");
    printf("10. Exporter Cartographie (.dot)\n");
    printf("11. Quitter\n");
    printf("\033[1;36m================================================\033[0m\n");
    printf("\033[1;32mAction > \033[0m");
}