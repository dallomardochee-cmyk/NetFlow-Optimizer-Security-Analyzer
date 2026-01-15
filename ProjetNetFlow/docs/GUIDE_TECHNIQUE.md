# GUIDE TECHNIQUE : NetFlow Optimizer & Security Analyzer

## 1. Introduction

Ce document détaille l'architecture technique, les algorithmes implémentés et le fonctionnement du projet **NetFlow Optimizer & Security Analyzer**. Il sert de référence pour comprendre le code source et la logique de conception utilisée pour répondre aux exigences du projet ALC2101.

---

## 2. Architecture Logicielle

Le projet est structuré de manière modulaire en langage C, favorisant la séparation des responsabilités.

### 2.1 Arborescence

- **`src/`** : Contient tout le code source (.c et .h).
  - **`graphe.*`** : Module central définissant le graphe, les nœuds et les arêtes.
  - **`liste_chainee.*`** : Structures de données auxiliaires (Files de priorité, Listes).
  - **`dijkstra.*`** : Implémentation de l'algorithme de routage le plus court.
  - **`bellman_ford.*`** : Algorithme de routage robuste (poids négatifs).
  - **`backtracking.*`** : Algorithme de recherche de chemins avec contraintes multiples (récursif).
  - **`securite.*`** : Modules d'analyse de graphe (Cycles, Points d'articulation).
  - **`utils.*`** : Gestion des entrées/sorties fichiers (persistance).
  - **`main.c`** : Point d'entrée et tests d'intégration.
- **`data/`** : Fichiers de données (topologies réseau).
- **`docs/`** : Documentation projet (Journal de bord, ce guide).
- **`Makefile`** : Système de compilation automatisé.

---

## 3. Structures de Données (Détails)

### 3.1 Graphe (`src/graphe.h`)

Nous avons choisi une représentation par **Listes d'Adjacence** car les réseaux informatiques sont souvent des graphes creux (peu d'arêtes par rapport au nombre max théorique d'arêtes).

- **`Struct Noeud`** : Contient un ID, un nom, et un pointeur vers la tête de la liste chaînée de ses arêtes sortantes.
- **`Struct Arete`** : Élément d'une liste chaînée. Contient la destination, et tous les attributs pondérés : `latence` (float), `bande_passante` (float), `cout` (float), `securite` (int).

### 3.2 File de Priorité (`src/liste_chainee.h`)

Essentielle pour l'efficacité de Dijkstra.

- **Implémentation** : Liste chaînée triée à l'insertion.
- **Complexité** : Insertion en O(N), Extraction en O(1).
- **Structure** : `ElementDijkstra` contient l'ID du nœud et sa `distance` (clé de tri).

---

## 4. Algorithmes Implémentés

### 4.1 Dijkstra (`src/dijkstra.c`)

- **Objectif** : Trouver le plus court chemin (minimisant la latence) depuis une source vers tous les autres nœuds.
- **Fonctionnement** :
  1. Initialisation des distances à INF, sauf source à 0.
  2. Utilisation d'une file de priorité pour explorer toujours le nœud le plus proche non visité.
  3. Relâchement (Relaxation) des arêtes adjacentes.
- **Complexité** : O(E + V log V) avec un tas binaire, ici O(E + V^2) avec notre liste simple (acceptable pour N < 1000).

### 4.2 Bellman-Ford (`src/bellman_ford.c`)

- **Objectif** : Idem Dijkstra, mais capable de détecter des cycles de poids négatifs (impossible physiquement pour la latence, mais requis pédagogiquement).
- **Fonctionnement** : Relâchement de toutes les arêtes V-1 fois.
- **Complexité** : O(V * E). Plus lent que Dijkstra mais plus robuste.

### 4.3 Backtracking avec Contraintes (`src/backtracking.c`)

- **Objectif** : Trouver un chemin qui respecte des seuils stricts (Bande Passante Min, Sécurité Min, Coût Max) tout en minimisant la latence.
- **Fonctionnement** :
  - Parcours DFS (Profondeur) récursif.
  - **Élagage (Pruning)** : On coupe la branche si :
    - La latence actuelle dépasse déjà le meilleur chemin trouvé complet.
    - Le coût dépasse le max autorisé.
    - La latence dépasse un max absolu.
- **Avantage** : Trouve des solutions que Dijkstra ne peut pas voir (car Dijkstra est glouton sur un seul critère).

### 4.4 Analyse Sécurité (`src/securite.c`)

- **Détection de Cycle** : DFS avec coloration des nœuds (Blanc, Gris, Noir). Si on rencontre un nœud Gris, c'est un cycle.
- **Points d'Articulation** : Variate de DFS (Algo de Tarjan). Un nœud est critique si son retrait augmente le nombre de composantes connexes (déconnecte le réseau). On utilise les dates de découverte (`disc`) et le lien le plus bas accessible (`low`).

---

## 5. Guide d'Utilisation

### 5.1 Compilation

Assurez-vous d'avoir `gcc` et `make`.

```bash
make
```

Cela génère l'exécutable `bin/netflow_optimizer`.

### 5.2 Exécution des Tests

```bash
./bin/netflow_optimizer
```

Le programme lancera une batterie de tests :

1. Création d'un mini graphe (Paris, Lyon, Marseille).
2. Vérification des structures de file.
3. Sauvegarde/Chargement fichier.
4. Exécution de Dijkstra, Bellman-Ford et Backtracking sur ce graphe.
5. Analyse de sécurité.

### 5.3 Format de Fichier Données

Les fichiers `.txt` dans `data/` doivent suivre ce format :

```
NB_NOEUDS 3
ORIENTE 0
NOEUD 0 Paris
NOEUD 1 Lyon
ARETE 0 1 15.5 1000 10 9
...
```

(ARETE source dest latence bp cout securite)

---

## 6. Conclusion

Le projet couvre l'ensemble des exigences fonctionnelles. L'architecture est prête à être étendue (ex: ajout d'une GUI, parallélisation) sans casser le noyau logique.
