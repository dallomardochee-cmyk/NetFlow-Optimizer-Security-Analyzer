#include <stdio.h>
#include "graphe.h"
#include "dijkstra.h"
#include "securite.h"
#include "utils.h"
#include "backtracking.h"
#include "simulation.h"

// Prototypes
void analyser_connectivite(Graphe* g, int depart);
void lancer_dijkstra(Graphe* g, int s, int d);
void afficher_graphe_complet(Graphe* g);
void generer_dot(Graphe* g, const char* nom_fichier);

int main() {
    // ========== DECLARATIONS GLOBALES ET VARIABLES ==========
    int choix = 0;
    int src = 0, dest = 0 , sec = 0; 
    float bp_min = 0.0, cout_max = 0.0;
    Graphe* g = NULL;
    char path[100];

    // ========== BOUCLE PRINCIPALE DU MENU ==========
    while (1) {
        menu();
        if (scanf("%d", &choix) != 1) break;
        if (choix == 0) break; // Sortie propre du programme

        switch (choix) {
            // ========== CASE 1: CHARGEMENT DU RESEAU ==========
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

            // ========== CASE 2: AFFICHAGE DU RESEAU ==========
            case 2:
                if (g) afficher_graphe_complet(g);
                else printf("Erreur : Chargez un reseau.\n");
                break;    

            // ========== CASE 3: ANALYSE CONNECTIVITE ET DIJKSTRA ==========
            case 3: 
                if (g) {
                    int confirmer;
                    printf("\n--- ANALYSE DE CONNECTIVITE ---\n");
                    printf("Entrez le noeud source (0 a %d) : ", g->nb_noeuds - 1);
                    
                    if (scanf("%d", &src) != 1) {
                        while(getchar() != '\n'); 
                        break;
                    }

                    if (src < 0 || src >= g->nb_noeuds) {
                        printf("\033[1;31m[ERREUR]\033[0m Noeud inexistant.\n");
                    } else {
                        // 1. On vérifie d'abord la portée (Connectivité)
                        analyser_connectivite(g, src);

                        // 2. On demande si on veut le détail de l'itinéraire
                        do {
                            printf("\nCalculer un itineraire ? (1: Oui / 0: Non) : ");
                            if (scanf("%d", &confirmer) != 1) {
                                while(getchar() != '\n');
                                confirmer = -1;
                                continue;
                            }
                        } while (confirmer != 1 && confirmer != 0);
                        
                        if (confirmer == 1) {
                            printf("Destination (0 a %d) : ", g->nb_noeuds - 1);
                            if (scanf("%d", &dest) != 1) {
                                printf("\033[1;31m[ERREUR]\033[0m Saisie invalide.\n");
                                while(getchar() != '\n');
                            } else if (dest >= 0 && dest < g->nb_noeuds) {
                                lancer_dijkstra(g, src, dest); 
                            } else {
                                printf("\033[1;31m[ERREUR]\033[0m Destination hors limites.\n");
                            }
                        }
                    } 
                } else {
                    printf("\033[1;31m[ERREUR]\033[0m Chargez un reseau avant l'analyse.\n");
                }
                break;

            // ========== CASE 4: BELLMAN-FORD ==========
            case 4:
                if (g) {
                    printf("Source : "); scanf("%d", &src);
                    printf("Destination : "); scanf("%d", &dest); // On a besoin de la cible !
                    bellman_ford(g, src, dest); 
                } else {
                    printf("Erreur : Chargez un reseau.\n");
                }
                break;
            // ========== CASE 5: AUDIT DE SECURITE ==========
            case 5:
                if (g) analyser_securite(g);
                else printf("Erreur : Chargez un reseau.\n");
                break;

            // ========== CASE 6: BACKTRACKING OPTIMISE ==========
            case 6:
                if (g) {
                    // Ajout d'un espace avant % pour "nettoyer" le buffer
                    printf("Source : ");           scanf(" %d", &src);
                    printf("Destination : ");      scanf(" %d", &dest);
                    printf("BP minimum : ");       scanf(" %f", &bp_min);
                    printf("Cout maximum : ");     scanf(" %f", &cout_max);
                    printf("Securité minimale : "); scanf(" %d", &sec);
                    
                    lancer_backtracking(g, src, dest, bp_min, cout_max, sec);
                } else {
                    printf("Erreur : Chargez un reseau.\n");
                }
                break;
            // ========== CASE 7: SIMULATION DE FLUX ==========
            case 7: // Simulation corrigée
                if (g) {
                    float taux;
                    int n_pqts;
                    printf("Noeud Source : "); scanf("%d", &src);
                    printf("Noeud Destination : "); scanf("%d", &dest);
                    printf("Taux d'arrivee (0.1 a 1.0) : "); scanf("%f", &taux);
                    printf("Nombre de paquets : "); scanf("%d", &n_pqts);
                    simuler_flux_avance(g, src, dest, n_pqts, taux);
                } else printf("Erreur : Chargez un reseau.\n");
                break;

            // ========== CASE 8: RAPPORT DE PERFORMANCE ==========
            case 8:
                if (g) { 
                    mesurer_performance_memoire(g); 
                    analyser_topologie(g); 
                } else printf("Erreur : Chargez un reseau.\n");
                break;
            
            // ========== CASE 9: SAUVEGARDE ==========
            case 9: 
                if (g) {
                    sauvegarder_graphe(g, "data/sauvegarde_topo.txt");
                    printf("Graphe sauvegarde.\n"); 
                } break; 
                
            // ========== CASE 10: EXPORT DOT ==========
            case 10:
                if (g) {
                    char nom_fich[100], nom_complet[110];
                    printf("Nom du fichier DOT (sans extension) : ");
                    scanf("%s", nom_fich);
                    sprintf(nom_complet, "%s.dot", nom_fich);
                    generer_dot(g, nom_complet);
                    printf("\033[1;32m[OK]\033[0m Genere : %s\n", nom_complet);
                } else printf("Erreur : Chargez un reseau.\n");
                break;

            // ========== CASE 11: SUPPRESSION NOEUD ==========
            case 11: // Suppression Noeud
                if (g) {
                    int id;
                    printf("ID du noeud a supprimer : ");
                    scanf("%d", &id);
                    
                    if (id < 0 || id >= g->nb_noeuds) {
                        printf("\033[1;31m[ERREUR]\033[0m Noeud inexistant.\n");
                                        } else {
                        // Afficher les liens avant suppression
                        printf("\033[1;33m[SYSTEM]\033[0m Liens isoles du noeud %d :\n", id);
                        
                        // Afficher les arêtes sortantes
                        Arete* aretes_sortantes = g->noeuds[id].aretes;
                        while (aretes_sortantes != NULL) {
                            printf("  - %d -> %d (Latence: %.2f, BP: %.2f, Cout: %.2f)\n", 
                                id, aretes_sortantes->destination, aretes_sortantes->latence, 
                                aretes_sortantes->bande_passante, aretes_sortantes->cout);
                            aretes_sortantes = aretes_sortantes->suivant;
                        }
                        
                        // Afficher les arêtes entrantes
                        for (int i = 0; i < g->nb_noeuds; i++) {
                            if (i != id) {
                                Arete* aretes_entrantes = g->noeuds[i].aretes;
                                while (aretes_entrantes != NULL) {
                                    if (aretes_entrantes->destination == id) {
                                        printf("  - %d -> %d (Latence: %.2f, BP: %.2f, Cout: %.2f)\n", 
                                            i, id, aretes_entrantes->latence, 
                                            aretes_entrantes->bande_passante, aretes_entrantes->cout);
                                    }
                                    aretes_entrantes = aretes_entrantes->suivant;
                                }
                            }
                        }
                        
                        supprimer_noeud(g, id);
                        printf("\033[1;32m[OK]\033[0m Noeud %d supprime avec ses liens.\n", id);
                    }
                } else printf("Erreur : Chargez un reseau.\n");
                break;

            // ========== CASE 12: SUPPRESSION ARETE ==========
            case 12: // Suppression Arete
                if (g) {
                    int s, d;
                    printf("Source du lien : "); scanf("%d", &s);
                    printf("Destination du lien : "); scanf("%d", &d);
                    supprimer_arete(g, s, d);
                    supprimer_arete(g, d, s);
                    printf("\033[1;32m[OK]\033[0m Liaison %d <-> %d supprimee.\n", s, d);
                } else printf("Erreur : Chargez un reseau.\n");
                break;

            // ========== CASE 13: AJOUT NOEUD ==========
            case 13: // Ajout Noeud
                if (g) {
                    int nouveau_id = g->nb_noeuds;
                    agrandir_graphe(g, g->nb_noeuds + 1);
                    printf("\033[1;32m[OK]\033[0m Noeud %d cree.\n", nouveau_id);
                } else {
                    g = creer_graphe(1); // Si aucun graphe n'existe
                    printf("\033[1;32m[OK]\033[0m Premier noeud (ID: 0) cree.\n");
                }
                break;

            // ========== CASE 14: AJOUT ARETE ==========
            case 14: // Ajout Arete
                if (g) {
                    int s, d, sec;
                    float lat, bp, c;
                    printf("Source : "); scanf("%d", &s);
                    printf("Destination : "); scanf("%d", &d);
                    printf("Latence : "); scanf("%f", &lat);
                    printf("BP : "); scanf("%f", &bp);
                    printf("Cout : "); scanf("%f", &c);
                    printf("Securite (1-10) : "); scanf("%d", &sec);
                    ajouter_arete(g, s, d, lat, bp, c, sec);
                } else printf("Erreur : Chargez un reseau.\n");
                break;
            // Generer un reseau geant
            case 15:
                generer_reseau_geant();
                break;
        }
    }

    if (g) liberer_graphe(g);
    return 0;
}