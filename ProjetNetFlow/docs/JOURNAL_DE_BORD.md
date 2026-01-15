# Journal de Bord - Projet NetFlow Optimizer

Ce fichier consigne chronologiquement toutes les actions, décisions et modifications apportées au projet. Il sert de référence pour assurer la traçabilité et la logique du développement.

| Date | Heure | Acteur | Action | Descriptions / Détails | Fichiers Impactés |
|------|-------|--------|--------|------------------------|-------------------|
| 2026-01-09 | 09:50 | Assistant | Initialisation | Création de l'arborescence du projet et des fichiers de documentation. | /, src/, data/, tests/, docs/ |
| 2026-01-09 | 09:50 | Assistant | Planification | Établissement du plan de mise en œuvre détaillé. | implementation_plan.md |
| 2026-01-09 | 09:55 | Assistant | Démarrage Phase 1 | Création du Makefile et des structures de Graphe. | Makefile, src/graphe.* |
| 2026-01-09 | 10:00 | Assistant | Implémentation Files | Création des structures Liste Chaînée et File de Priorité. | src/liste_chainee.* |
| 2026-01-09 | 10:05 | Assistant | Implémentation Utils | Création des fonctions de chargement/sauvegarde de graphe. | src/utils.*, src/main.c |
| 2026-01-09 | 10:10 | Assistant | Vérification Phase 1 | Compilation et exécution réussie des tests unitaires (Graphe, File, I/O). | bin/netflow_optimizer |
| 2026-01-09 | 10:15 | Assistant | Démarrage Phase 2 | Extension des files de priorité pour Dijkstra et implémentation de l'algo. | src/liste_chainee.*, src/dijkstra.* |
| 2026-01-09 | 10:20 | Assistant | Vérification Dijkstra | Tests unitaires concluants pour le calcul du plus court chemin. | src/main.c |
| 2026-01-09 | 10:25 | Assistant | Implémentation Bellman-Ford | Création de l'algorithme robuste aux poids négatifs. | src/bellman_ford.* |
| 2026-01-09 | 10:30 | Assistant | Implémentation Backtracking | Développement de l'algo récursif avec contraintes multiples. | src/backtracking.* |
| 2026-01-09 | 10:35 | Assistant | Démarrage Phase 3 | Implémentation des outils d'analyse de sécurité (Cycles, Point d'art.). | src/securite.* |
| 2026-01-09 | 10:45 | Assistant | Integration & Validation | Assemblage final et tests de tous les modules dans main.c. | src/main.c |
| 2026-01-09 | 10:50 | Assistant | Documentation | Rédaction du Guide Technique détaillé. | docs/GUIDE_TECHNIQUE.md |
| 2026-01-09 | 11:00 | Assistant | Extensions | Ajout SCC (Tarjan), Simulation Trafic, et Interface Interactive. | src/* |
| 2026-01-09 | 11:30 | Assistant | Conformité 100% | Ajout Matrice Adjacence, Ponts, K-Chemins, Backtracking avancé. | src/* |
| 2026-01-09 | 11:35 | Assistant | Vérification | Exécution réussie de make test et validation unitaires. | tests/ |
| 2026-01-09 | 11:45 | Assistant | Innovation | Ajout export Graphviz (.dot) pour visualisation externe. | src/utils.c |
| 2026-01-09 | 12:00 | Assistant | Clôture | Projet prêt. Conformité 100%. Validation finale. | - |

## Référence Rapide

- **Objectif :** Système de routage et sécurité réseau en C.
- **Règles :** C11, commentaires français, pas d'emojis, gestion mémoire stricte.
