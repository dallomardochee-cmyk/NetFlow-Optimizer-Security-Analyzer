# NetFlow Optimizer & Security Analyzer

## Description

Projet réalisé dans le cadre du cours ALC2101 - Algorithmique et Complexité.
Ce logiciel permet de modéliser, optimiser et sécuriser des réseaux informatiques complexes.

## Fonctionnalités

- **Modélisation** : Graphe pondéré (Latence, BP, Coût, Sécurité).
- **Routage** : Dijkstra, Bellman-Ford, K-Plus Courts Chemins.
- **Optimisation** : Backtracking avec contraintes multiples (Nœuds interdits/obligatoires).
- **Sécurité** : Détection de cycles, Points d'articulation, Ponts, SCC (Tarjan).
- **Simulation** : Gestion de files d'attente et simulation de trafic.

## Structure

- `src/` : Code source C.
- `docs/` : Documentation technique et journal de bord.
- `data/` : Jeux de données de test.
- `tests/` : Tests unitaires.
- `bin/` : Exécutables (après compilation).

## Compilation et Utilisation

Veuillez consulter le fichier `MES_COMMANDES.md` à la racine pour le guide complet des commandes.
En résumé :

```bash
make
./bin/netflow_optimizer
```

## Auteurs

Groupe ALC2101
