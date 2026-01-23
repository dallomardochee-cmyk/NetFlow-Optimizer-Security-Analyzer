# NetFlow Sim : Simulateur de Routage et Audit Réseau

Projet Universitaire 2025-2026 | UVCI

Présentation

NetFlow Sim est une application en langage C permettant de modéliser, simuler et auditer des topologies réseaux complexes. L'outil combine des algorithmes de théorie des graphes et des structures de données dynamiques pour optimiser le transport de paquets de données sous contraintes.

Fonctionnalités principales

Gestion de Topologie (Module 1)
Double Représentation : Utilisation simultanée de listes d'adjacence (graphes creux) et de matrices d'adjacence (graphes denses).

Évolutivité : Fonctions d'ajout/suppression de nœuds et d'arêtes en temps réel.

Algorithmes de Routage (Module 2)
Dijkstra : Recherche du chemin le plus rapide (latence minimale).

Bellman-Ford : Détection de cycles négatifs et instabilités réseau.

Backtracking : Recherche sous contraintes multiples (Bande passante min, Coût max).

Audit & Sécurité (Module 3)
Algorithme de Tarjan : Identification des points d'articulation (SPOF) et des ponts critiques.

Analyse de Connectivité : Vérification de l'intégrité du réseau après une panne.

Simulation de Flux (Module 4)
File de Priorité : Gestion du trafic par priorité de paquets (Liste doublement chaînée).

Statistiques : Calcul du taux de perte de paquets et de la latence moyenne.

Installation et Compilation

Prérequis
Compilateur gcc

Outil make

Optionnel : Graphviz (pour la visualisation des fichiers .dot)

Compilation
Ouvrez un terminal dans le dossier racine et lancez :

make

./NetFlow_Sim.exe

Structure du Projet
src/ : Fichiers sources (.c) et headers (.h).

reseau_test.txt : Fichier de configuration du réseau (format : src dest latence cout bp secu).

Makefile : Script de compilation automatisé.

nomfichier.dot : Fichier généré pour exporter le graphe.

Exemple de fichier de données
Le fichier de configuration suit ce format :

Plaintext
7 (Nombre de noeuds)
0 1 10.5 5.0 100.0 5  (Source Destination Latence Coût BP Sécurité).
