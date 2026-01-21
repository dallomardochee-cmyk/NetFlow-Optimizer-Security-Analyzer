#include <windows.h>
#include <stdio.h>
#include "graphe.h"
#include "dijkstra.h"
#include "securite.h"
#include "utils.h"
#include "backtracking.h"
#include "simulation.h"

void analyser_connectivite(Graphe* g, int depart);
void lancer_dijkstra(Graphe* g, int s, int d);
void lancer_backtracking(Graphe* g, int src, int dest, float bp_min, float cout_max);
void afficher_graphe_complet(Graphe* g);
void generer_dot(Graphe* g, const char* nom_fichier);
void simuler_flux_avance(Graphe* g, int source, int destination, int nb_paquets);

int main() {
    int choix = 0;
    int src = 0, dest = 0; 
    int n_paquets = 0;
    float bp_min = 0.0, cout_max = 0.0;
    Graphe* g = NULL;
    char path[100]; 

    while (1) {
        menu();
        if (scanf("%d", &choix) != 1) break;
        if (choix == 11) break; // Modifié pour correspondre à une option de sortie propre

        switch (choix) {
            case 1:
                printf("Chemin du fichier (ex: data/reseau_reel.txt) : ");
                scanf("%s", path);
                g = charger_depuis_fichier(path);
                if (g) {
                    printf("\033[1;32m[SUCCES] Reseau charge.\033[0m\n");
                    printf("Nombre de noeuds : %d\n", g->nb_noeuds);
                }
                else printf("\033[1;31m[ERREUR] Impossible d'ouvrir %s\033[0m\n", path);
                break;

            case 2:
                afficher_graphe_complet(g);
                break;    

            case 3: 
                if (g) {
                    int confirmer;
                    printf("\n--- ANALYSE DE CONNECTIVITE ---\n");
                    printf("Entrez le noeud source (0 a %d) : ", g->nb_noeuds - 1);
                    
                    if (scanf("%d", &src) != 1) {
                        printf("Entree invalide.\n");
                        while(getchar() != '\n'); 
                        break;
                    }

                    if (src < 0 || src >= g->nb_noeuds) {
                        printf("\033[1;31m[ERREUR]\033[0m Le noeud %d n'existe pas.\n", src);
                    } else {
                        analyser_connectivite(g, src);
                        
                        do {
                            printf("\nSouhaitez-vous calculer un itineraire ? (1: Oui / 0: Non) : ");
                            
                            // Si l'utilisateur tape une lettre au lieu d'un chiffre
                            if (scanf("%d", &confirmer) != 1) {
                                printf("\033[1;31m[ERREUR]\033[0m Veuillez entrer un chiffre (1 ou 0).\n");
                                while(getchar() != '\n'); // Vide le buffer pour éviter la boucle infinie
                                confirmer = -1; // Force la répétition de la boucle
                                continue;
                            }

                            if (confirmer != 1 && confirmer != 0) {
                                printf("\033[1;33m[!] Choix invalide.\033[0m Merci d'entrer uniquement 1 pour OUI ou 0 pour NON.\n");
                            }

                        } while (confirmer != 1 && confirmer != 0);
                        if (confirmer == 1) {
                            printf("Entrez le noeud destination (0 a %d) : ", g->nb_noeuds - 1);
                            scanf("%d", &dest);
                            if (dest >= 0 && dest < g->nb_noeuds) {
                                lancer_dijkstra(g, src, dest); 
                            } 
                           } 
                       }
                   }
                else {
                    printf("Erreur : Veuillez charger le reseau d'abord.\n");
                }
                break;

            case 4:
                if (g) { printf("Source : "); scanf("%d", &src); bellman_ford(g, src); }
                else { printf("Erreur : Veuillez charger le reseau d'abord.\n"); }
                break;

            case 5:
                if (g) analyser_securite(g);
                else printf("Erreur : Veuillez charger le reseau d'abord.\n");
                break;

            case 6:
                if (g) {
                    printf("Source : "); scanf("%d", &src);
                    printf("Destination : "); scanf("%d", &dest);
                    printf("BP minimum : "); scanf("%f", &bp_min);
                    printf("Cout maximum : "); scanf("%f", &cout_max);
                    lancer_backtracking(g, src, dest, bp_min, cout_max);
                }
                else printf("Erreur : Veuillez charger le reseau d'abord.\n");
                break;

            case 7:
                if (g) {
                    printf("Noeud source : "); scanf("%d", &src);
                    printf("Noeud destination : "); scanf("%d", &dest);
                    printf("Nombre de paquets a envoyer : "); scanf("%d", &n_paquets);
                    simuler_flux_avance(g, src, dest, n_paquets);
                }
                else printf("Erreur : Veuillez charger le reseau d'abord.\n");
                break;

            case 8:
                if (g) { 
                    mesurer_performance_memoire(g); 
                    analyser_topologie(g); 
                }
                else { printf("Erreur : Veuillez charger le reseau d'abord.\n"); }
                break;
            
            case 9: 
                sauvegarder_graphe(g, "data/sauvegarde_topo.txt");
                printf("Graphe sauvegarde.\n"); 
                break; 
                
           case 10:
                if (g) {
                    char nom_fich[100];
                    char nom_complet[110];

                    printf("Entrez le nom du fichier de sortie (ex: mon_reseau) : ");
                    scanf("%s", nom_fich);

                    // On ajoute l'extension .dot automatiquement pour plus de sécurité
                    sprintf(nom_complet, "%s.dot", nom_fich);

                    generer_dot(g, nom_complet);
                    
                    printf("\n\033[1;32m[SUCCES]\033[0m Fichier genere : %s\n", nom_complet);
                    printf("Utilisez un visualiseur Graphviz pour ouvrir ce fichier.\n");
                } else {
                    printf("\033[1;31m[ERREUR]\033[0m Chargez un reseau d'abord (Option 1).\n");
                }
                break;
        }
    }

    if (g) liberer_graphe(g);
    return 0;
}