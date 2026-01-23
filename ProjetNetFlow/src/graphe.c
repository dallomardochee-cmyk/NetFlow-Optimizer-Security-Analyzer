#include "graphe.h"
#include <stdlib.h>
#include <string.h>


Graphe* creer_graphe(int nb_noeuds) {
    // 1. Allocation de la structure principale
    Graphe* g = (Graphe*)malloc(sizeof(Graphe));
    if (!g) return NULL;

    g->nb_noeuds = nb_noeuds;

    // 2. Allocation du tableau de Noeuds (conformément au typedef : Noeud* noeuds)
    g->noeuds = (Noeud*)malloc(nb_noeuds * sizeof(Noeud));
    if (!g->noeuds) {
        free(g);
        return NULL;
    }
    
    // 3. Allocation de la matrice d'adjacence
    g->matrice_adjacence = (float**)malloc(nb_noeuds * sizeof(float*));
    
    for (int i = 0; i < nb_noeuds; i++) {
        // Initialisation du Noeud
        g->noeuds[i].id = i;
        sprintf(g->noeuds[i].nom, "Routeur_%d", i);
        g->noeuds[i].aretes = NULL; // Remplace liste_adjacence par aretes

        // Allocation de la ligne de la matrice
        g->matrice_adjacence[i] = (float*)malloc(nb_noeuds * sizeof(float));
        for (int j = 0; j < nb_noeuds; j++) {
            // Utilisation de -1.0 ou une constante INFINI pour Dijkstra
            g->matrice_adjacence[i][j] = -1.0; 
        }
    }
    
    return g;
}

void ajouter_arete(Graphe* g, int src, int dest, float lat, float bp, float cout, int sec) {
    if (!g || src < 0 || src >= g->nb_noeuds || dest < 0 || dest >= g->nb_noeuds) {
        return;
    }

    // 1. Mise à jour de la LISTE D'ADJACENCE (Aller)
    // On utilise g->noeuds[src].aretes conformément à ton typedef
    Arete* nouvelle_aller = creer_arete(dest, lat, bp, cout, sec);
    nouvelle_aller->suivant = g->noeuds[src].aretes;
    g->noeuds[src].aretes = nouvelle_aller;

    // 2. Mise à jour de la MATRICE D'ADJACENCE (Aller)
    g->matrice_adjacence[src][dest] = lat;

    // 3. Cas du graphe non-orienté (Retour)
    // On crée l'arête symétrique pour que la communication soit bidirectionnelle
    Arete* nouvelle_retour = creer_arete(src, lat, bp, cout, sec);
    nouvelle_retour->suivant = g->noeuds[dest].aretes;
    g->noeuds[dest].aretes = nouvelle_retour;

    // Symétrie dans la matrice
    g->matrice_adjacence[dest][src] = lat;
}

void agrandir_graphe(Graphe* g, int nouvelle_capacite) {
    if (!g || nouvelle_capacite <= g->nb_noeuds) return;

    // 1. Redimensionner le tableau de nœuds (Utilisation de 'noeuds')
    Noeud* temp_noeuds = realloc(g->noeuds, nouvelle_capacite * sizeof(Noeud));
    if (temp_noeuds == NULL) return; // Échec de réallocation
    g->noeuds = temp_noeuds;

    // 2. Redimensionner le tableau de pointeurs de la matrice
    float** temp_matrice = realloc(g->matrice_adjacence, nouvelle_capacite * sizeof(float*));
    if (temp_matrice == NULL) return;
    g->matrice_adjacence = temp_matrice;
    
    for (int i = 0; i < nouvelle_capacite; i++) {
        if (i >= g->nb_noeuds) {
            // Pour les nouveaux nœuds : Initialisation complète
            g->noeuds[i].id = i;
            sprintf(g->noeuds[i].nom, "Routeur_%d", i);
            g->noeuds[i].aretes = NULL; // Utilisation de 'aretes'

            // Nouvelle ligne dans la matrice
            g->matrice_adjacence[i] = malloc(nouvelle_capacite * sizeof(float));
            for (int j = 0; j < nouvelle_capacite; j++) {
                g->matrice_adjacence[i][j] = -1.0;
            }
        } else {
            // Pour les nœuds existants : Agrandir la ligne et initialiser les nouvelles colonnes
            float* temp_ligne = realloc(g->matrice_adjacence[i], nouvelle_capacite * sizeof(float));
            if (temp_ligne) {
                g->matrice_adjacence[i] = temp_ligne;
                for (int j = g->nb_noeuds; j < nouvelle_capacite; j++) {
                    g->matrice_adjacence[i][j] = -1.0;
                }
            }
        }
    }
    
    g->nb_noeuds = nouvelle_capacite;
    printf("[DEBUG] Graphe agrandi a %d noeuds.\n", nouvelle_capacite);
}

void supprimer_arete(Graphe* g, int src, int dest) {
    if (!g || src < 0 || src >= g->nb_noeuds || dest < 0 || dest >= g->nb_noeuds) return;

    // --- 1. Nettoyage dans la Liste d'Adjacence ---
    Arete* curr = g->noeuds[src].aretes;
    Arete* prev = NULL;

    while (curr != NULL) {
        if (curr->destination == dest) {
            if (prev == NULL) g->noeuds[src].aretes = curr->suivant;
            else prev->suivant = curr->suivant;
            free(curr);
            break; // Sortir dès qu'on a trouvé et supprimé
        }
        prev = curr;
        curr = curr->suivant;
    }

    // --- 2. Nettoyage dans la Matrice ---
    if (g->matrice_adjacence && g->matrice_adjacence[src]) {
        g->matrice_adjacence[src][dest] = -1.0;
    }
}

void supprimer_noeud(Graphe* g, int id_noeud) {
    // 1. VERIFICATION DE SECURITE (Indispensable !)
    if (!g || id_noeud < 0 || id_noeud >= g->nb_noeuds) {
        printf("\033[1;31m[ERREUR]\033[0m ID %d invalide (Max: %d).\n", id_noeud, g->nb_noeuds - 1);
        return;
    }

    // 2. Supprimer les arêtes SORTANTES
    Arete* curr = g->noeuds[id_noeud].aretes;
    while (curr) {
        Arete* temp = curr;
        curr = curr->suivant;
        free(temp);
    }
    g->noeuds[id_noeud].aretes = NULL; // On marque le noeud comme vide

    // 3. Supprimer les arêtes ENTRANTES (venant des autres nœuds)
    // On nettoie la matrice ET les listes d'adjacence des voisins
    for (int i = 0; i < g->nb_noeuds; i++) {
        if (i == id_noeud) continue;
        
        // Supprime le lien dans la liste d'adjacence du voisin i vers id_noeud
        supprimer_arete(g, i, id_noeud);
        
        // Nettoie la matrice
        g->matrice_adjacence[id_noeud][i] = -1.0;
        g->matrice_adjacence[i][id_noeud] = -1.0;
    }
    
    printf("\033[1;32m[SUCCES]\033[0m Le noeud %d a ete neutralise.\n", id_noeud);
}
Graphe* charger_depuis_fichier(const char* nom_fichier) {
    FILE* f = fopen(nom_fichier, "r");
    if (!f) return NULL;

    char ligne[256];
    int nb_noeuds = 0;
    Graphe* g = NULL;

    while (fgets(ligne, sizeof(ligne), f)) {
        if (ligne[0] == '#' || ligne[0] == '\n') continue; // Ignore commentaires

        if (strncmp(ligne, "NB_NOEUDS", 9) == 0) {
            sscanf(ligne, "NB_NOEUDS %d", &nb_noeuds);
            g = creer_graphe(nb_noeuds);
        } 
        else if (strncmp(ligne, "ARETE", 5) == 0) {
            int s, d, sec;
            float lat, bp, ct;
            sscanf(ligne, "ARETE %d %d %f %f %f %d", &s, &d, &lat, &bp, &ct, &sec);
            if (g) ajouter_arete(g, s, d, lat, bp, ct, sec);
        }
        // On ignore les lignes "NOEUD" car ton graphe utilise les indices (0-99)
    }

    fclose(f);
    return g;
}

void afficher_graphe_complet(Graphe* g) {
    if (!g) {
        printf("\n\033[1;31m[ERREUR]\033[0m Aucun graphe a afficher.\n");
        return;
    }

    printf("\n\033[1;34m============================================================================\033[0m\n");
    printf("           AFFICHAGE INTEGRAL DE LA TOPOLOGIE RESEAU (Full Metrics)           \n");
    printf("\033[1;34m============================================================================\033[0m\n");

    for (int i = 0; i < g->nb_noeuds; i++) {
        printf("\033[1;33mNoeud [%02d]\033[0m | ", i);
        
        Arete* a = g->noeuds[i].aretes; 
        
        if (a == NULL) {
            printf("\033[0;90m(Aucune connexion active)\033[0m");
        } else {
            while (a) {
                // Formatage enrichi : Dest (Latence | BP | Coût | Sécurité)
                // On utilise des couleurs différentes pour chaque métrique pour la lisibilité
                printf("\033[1;32m-> [%d]\033[0m(\033[0;36mL:%.1f\033[0m, \033[0;35mBP:%.0f\033[0m, \033[0;33mC:%.1f\033[0m, \033[0;31mS:%d\033[0m)", 
                        a->destination, 
                        a->latence, 
                        a->bande_passante, 
                        a->cout, 
                        a->securite);
                
                if (a->suivant) printf(" | ");
                a = a->suivant;
            }
        }
        printf("\n");
        
        // Pagination
        if ((i + 1) % 20 == 0 && i != g->nb_noeuds - 1) {
            printf("\n\033[0;90m--- Appuyez sur Entree pour voir la suite (%d/%d) ---\033[0m", i+1, g->nb_noeuds);
            getchar(); 
            getchar(); 
        }
    }
    printf("\033[1;34m============================================================================\033[0m\n\n");
}

void analyser_connectivite(Graphe* g, int source) {
    if (!g || source < 0 || source >= g->nb_noeuds) return;

    // 1. Initialisation du tableau de visite
    int* visite = calloc(g->nb_noeuds, sizeof(int));
    int* file = malloc(g->nb_noeuds * sizeof(int));
    int tete = 0, queue = 0;

    // 2. Algorithme BFS
    visite[source] = 1;
    file[queue++] = source;

    while (tete < queue) {
        int actuel = file[tete++]; // 'actuel' est maintenant bien utilisé
        
        // Correction : tab_noeuds -> noeuds | liste_adjacence -> aretes
        Arete* a = g->noeuds[actuel].aretes; 
        
        while (a) {
            if (!visite[a->destination]) {
                visite[a->destination] = 1;
                file[queue++] = a->destination;
            }
            a = a->suivant;
        }
    }

    // 3. Vérification du résultat
    int nb_atteints = queue;
    printf("\n--- ANALYSE DE CONNECTIVITE (Source: %d) ---\n", source);
    printf("Noeuds accessibles : %d / %d\n", nb_atteints, g->nb_noeuds);

    if (nb_atteints == g->nb_noeuds) {
        printf("Statut : \033[1;32mRESEAU FORTEMENT CONNECTE\033[0m\n");
    } else {
        printf("Statut : \033[1;31mRESEAU FRAGMENTE\033[0m (%d noeuds isoles)\n", g->nb_noeuds - nb_atteints);
    }

    free(visite);
    free(file);
}
void analyser_topologie(Graphe* g) {
    if (!g) return;

    float lat_totale = 0;
    int nb_links = 0;

    for (int i = 0; i < g->nb_noeuds; i++) {
        // Correction : tab_noeuds -> noeuds | liste_adjacence -> aretes
        Arete* a = g->noeuds[i].aretes;
        while (a) {
            lat_totale += a->latence;
            nb_links++;
            a = a->suivant;
        }
    }

    float moyenne = (nb_links > 0) ? lat_totale / nb_links : 0;
    
    // Calcul de la densité : (nb_liens / nb_liens_possibles)
    // Pour un graphe non-orienté, nb_liens_max = n*(n-1)/2
    // Ici on divise nb_links par 2 car chaque arête est comptée deux fois (Aller/Retour)
    float densite = 0;
    if (g->nb_noeuds > 1) {
        densite = (float)(nb_links / 2.0) / ((g->nb_noeuds * (g->nb_noeuds - 1)) / 2.0);
    }

    printf("\n\033[1;34m--- ANALYSE TOPOLOGIQUE ---\033[0m\n");
    printf("Nombre de noeuds : %d\n", g->nb_noeuds);
    printf("Nombre de liaisons : %d\n", nb_links / 2); // /2 car bidirectionnel
    printf("Latence moyenne : %.2f ms\n", moyenne);
    printf("Densite du reseau : %.2f%%\n", densite * 100);

    // Diagnostic intelligent
    if (moyenne < 5.0 && densite > 0.5) {
        printf("Detection : \033[1;32mDATACENTER / LAN HAUTE DENSITE\033[0m\n");
    } else if (moyenne > 20.0) {
        printf("Detection : \033[1;33mWAN (Internet / Large Echelle)\033[0m\n");
    } else {
        printf("Detection : \033[1;36mTOPOLOGIE MIXTE / IoT\033[0m\n");
    }
    printf("---------------------------\n");
}

void mesurer_performance_memoire(Graphe* g) {
    if (!g) return;

    // 1. Taille de la structure de base + tableau de Noeuds
    size_t sz = sizeof(Graphe) + (g->nb_noeuds * sizeof(Noeud));

    // 2. Taille de la Matrice d'Adjacence (Module 1)
    // Elle prend (N * sizeof(float*)) + (N * N * sizeof(float))
    sz += (g->nb_noeuds * sizeof(float*)); 
    sz += (g->nb_noeuds * g->nb_noeuds * sizeof(float));

    // 3. Taille des listes d'adjacence
    for (int i = 0; i < g->nb_noeuds; i++) {
        // Correction : tab_noeuds -> noeuds | liste_adjacence -> aretes
        Arete* a = g->noeuds[i].aretes;
        while (a) { 
            sz += sizeof(Arete); 
            a = a->suivant; 
        }
    }

    printf("\n\033[1;35m--- PERFORMANCE RAM ---\033[0m\n");
    printf("Occupation totale : %.2f Ko\n", (float)sz / 1024);
    
    // Un petit plus pour ton rapport :
    if (sz > 1024 * 1024) {
        printf("Statut : \033[1;31mCONSOMMATION ELEVEE (%.2f Mo)\033[0m\n", (float)sz / (1024 * 1024));
    } else {
        printf("Statut : \033[1;32mOPTIMISE\033[0m\n");
    }
}

void liberer_graphe(Graphe* g) {
    if (!g) return;

    for (int i = 0; i < g->nb_noeuds; i++) {
        // Libération manuelle de la liste d'arêtes (évite l'erreur liberer_liste)
        Arete* curr = g->noeuds[i].aretes;
        while (curr) {
            Arete* temp = curr;
            curr = curr->suivant;
            free(temp);
        }
        
        // Libération des lignes de la matrice
        if (g->matrice_adjacence[i]) {
            free(g->matrice_adjacence[i]);
        }
    }

    free(g->matrice_adjacence);
    free(g->noeuds);
    free(g);
    printf("\033[1;32m[CLEAN]\033[0m Memoire du graphe liberee.\n");
}

void sauvegarder_graphe(Graphe* g, const char* nom_fichier) {
    if (!g) return;
    FILE* f = fopen(nom_fichier, "w");
    if (!f) { printf("Erreur sauvegarde.\n"); return; }

    fprintf(f, "%d\n", g->nb_noeuds);
    for (int i = 0; i < g->nb_noeuds; i++) {
        Arete* a = g->noeuds[i].aretes;
        while (a) {
            // Sauvegarde au format : src dest latence cout bp securite
            // On ne sauvegarde que i < a->destination pour éviter les doublons dans le fichier
            if (i < a->destination) {
                fprintf(f, "%d %d %.2f %.2f %.2f %d\n", 
                        i, a->destination, a->latence, a->cout, a->bande_passante, a->securite);
            }
            a = a->suivant;
        }
    }
    fclose(f);
    printf("\n\033[1;32m[SUCCES]\033[0m Topologie sauvegardee dans %s\n", nom_fichier);
}

void generer_dot(Graphe* g, const char* nom_fichier) {
    if (!g) return;
    FILE* f = fopen(nom_fichier, "w");
    if (!f) return;

    fprintf(f, "graph G {\n    node [shape=circle, style=filled, fillcolor=lightblue];\n");
    for (int i = 0; i < g->nb_noeuds; i++) {
        Arete* a = g->noeuds[i].aretes;
        while (a) {
            if (i < a->destination) { // Pour un graphe non-orienté, une seule ligne suffit
                fprintf(f, "    %d -- %d [label=\"%.1f ms\"];\n", i, a->destination, a->latence);
            }
            a = a->suivant;
        }
    }

    for (int i = 0; i < g->nb_noeuds; i++) {
        // Si le noeud n'a plus aucune arête, on le met en évidence
        if (g->noeuds[i].aretes == NULL) {
            fprintf(f, "    %d [fillcolor=tomato, label=\"Routeur %d (OFF)\"];\n", i, i);
        } else {
            fprintf(f, "    %d [label=\"Routeur %d\"];\n", i, i);
        }
        
        Arete* a = g->noeuds[i].aretes;
        while (a) {
            if (i < a->destination) {
                fprintf(f, "    %d -- %d [label=\"%.1f ms\", len=2.0];\n", i, a->destination, a->latence);
            }
            a = a->suivant;
        }
    }
    fprintf(f, "}\n");
    fclose(f);
    printf("\033[1;32m[OK]\033[0m Fichier %s genere. Utilisez 'dot -Tpng %s -o reseau.png' pour visualiser.\n", nom_fichier, nom_fichier);
}