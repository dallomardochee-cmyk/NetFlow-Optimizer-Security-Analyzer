# Guide des Commandes - NetFlow Optimizer

Ce fichier récapitule toutes les commandes nécessaires pour compiler, exécuter et tester le projet.

## 1. Compilation

Avant de lancer le programme, il faut transformer le code source en exécutable.

* **Compiler le projet (Standard)** :

    ```bash
    make
    ```

    *Utilité :* Crée l'exécutable `./bin/netflow_optimizer`. À faire après chaque modification du code.

* **Nettoyer les fichiers temporaires** :

    ```bash
    make clean
    ```

    *Utilité :* Supprime les dossiers `obj/` et `bin/` pour repartir sur une base propre. Utile si la compilation semble bloquée ou buggée.

* **Mode Debug** :

    ```bash
    make debug
    ```

    *Utilité :* Compile avec des informations supplémentaires pour le débogage (utile avec `gdb` ou `valgrind`).

## 2. Exécution du Programme

Une fois compilé, voici comment lancer l'interface interactive.

* **Lancer l'application** :

    ```bash
    ./bin/netflow_optimizer
    ```

    *Fonctionnement :* Ouvre le menu principal interactif. Suivez les instructions à l'écran (tapez 1, 2, 3...).

## 3. Tests et Vérification

Pour prouver que le code est robuste et respecte le cahier des charges.

* **Lancer les Tests Unitaires** :

    ```bash
    make test
    ```

    *Utilité :* Compile et exécute automatiquement `tests/tests_unitaires.c`.
    *Résultat attendu :* Doit afficher "OK" pour chaque module et finir par "Tous les tests unitaires ont réussi".

* **Vérifier les fuites de mémoire (Valgrind)** :
    Si vous avez l'outil `valgrind` installé (sur Linux/Mac) :

    ```bash
    valgrind ./bin/netflow_optimizer
    ```

    *Utilité :* Analyse si le programme libère bien toute la mémoire utilisée.

## 4. Visualisation Graphique (Bonus)

Le programme permet d'exporter le graphe en fichier `.dot`.

1. Dans le menu principal, choisissez l'option **9** (Exporter).
2. Entrez un nom de fichier, par exemple `mon_graphe.dot`.
3. **Pour voir l'image** :
    * **Option A (En ligne)** : Ouvrez le fichier `.dot` avec un éditeur de texte, copiez le contenu, et collez-le sur [viz-js.com](http://viz-js.com).
    * **Option B (Ligne de commande)** : Si Graphviz est installé :

        ```bash
        dot -Tpng mon_graphe.dot -o mon_graphe.png
        open mon_graphe.png  # ou xdg-open sur Linux
        ```

---
**Résumé Rapide pour la Soutenance :**

1. `make clean`
2. `make`
3. `./bin/netflow_optimizer`
(Faire la démo dans le menu)
4. (Si question sur les tests) `make test`
