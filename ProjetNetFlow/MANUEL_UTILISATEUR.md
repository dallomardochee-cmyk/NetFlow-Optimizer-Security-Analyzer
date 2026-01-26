# Manuel Utilisateur - NetFlow Optimizer

Ce guide détaille chaque fonctionnalité de l'application, explique les termes techniques (Source, Destination) et comment interpréter les résultats.

## 1. Concepts de Base

* **Source** : Le point de départ (l'ordinateur ou le serveur qui envoie les données). Dans notre graphe par défaut, `0` est Paris.
* **Destination** : Le point d'arrivée. Dans notre graphe par défaut, `2` est Marseille.
* **Latence (ms)** : Le temps que met l'information pour voyager (mieux = plus petit).
* **Bande Passante (Mbps)** : La capacité du tuyau (mieux = plus grand).
* **Coût (€)** : Le prix de la liaison.
* **Sécurité (0-10)** : Niveau de fiabilité (10 = très sûr).

## 2. Guide des Options du Menu

### Option 3 : Algorithme Dijkstra (Plus court chemin)

* **But** : Trouver le chemin le plus RAPIDE (latence minimale) sans aucune autre contrainte.
* **Entrée** : ID Source, ID Destination.
* **Interprétation Résultat** :
  * `Chemin : 0 -> 1 -> 2` : Il faut passer par le nœud 1 pour aller de 0 à 2.
  * `Latence : 23.00 ms` : Le temps total du trajet.

### Option 4 : Algorithme Bellman-Ford

* **But** : Comme Dijkstra, mais fonctionne même si certains coûts sont négatifs (rare en réseau physique, utile en finance/théorie).
* **Interprétation** : Si le résultat est identique à Dijkstra, tout va bien. Si Bellman-Ford détecte un "Cycle Négatif", cela signifie qu'il y a une erreur dans les données du réseau (le temps remonte vers le passé !).

### Option 5 : Backtracking (Chemin avec Contraintes)

* **But** : Trouver un chemin "sur mesure". Par exemple : "Je veux aller vite, mais je refuse de payer plus de 100€ et je veux une sécurité minimum de 8/10".
* **Entrées** :
  * *Min Bande Passante* : Filtre les câbles trop petits.
  * *Min Sécurité* : Filtre les liaisons dangereuses.
  * *Max Coût* : Filtre les chemins trop chers.
  * *Max Latence* : Temps maximum acceptable.
  * *Nœuds Interdits* : "Je ne veux PAS passer par Lyon (1)".
  * *Nœuds Obligatoires* : "Je DOIS passer par Bordeaux (3)".
* **Résultat** : Affiche le meilleur chemin qui respecte TOUTES ces règles. Si aucun chemin n'est possible, il affichera "Aucun chemin ne respecte ces critères".

### Option 6 : Analyse Sécurité

Lance trois analyses automatiques pour diagnostiquer la santé du réseau :

1. **Détection de Cycle** : Vérifie si des données peuvent tourner en rond à l'infini (Danger de congestion).
2. **Points d'Articulation** : Trouve les nœuds "Fragiles". Si un point d'articulation tombe en panne, le réseau est coupé en deux.
3. **Composantes Fortement Connexes (SCC)** : Vérifie si tout le monde peut parler à tout le monde.

### Option 7 : Simulation de Trafic

* **But** : Simuler une charge réelle sur le réseau pour voir s'il tient le coup.
* **Entrées** :
  * *Nb Cycles* : Durée de la simulation (ex: 1000 "tics" d'horloge).
  * *Capacité File* : Taille de la mémoire tampon des routeurs (ex: 10 paquets).
  * *Taux Arrivée (0.0 - 1.0)* : Intensité du trafic. 0.1 = calme, 0.9 = saturation.
* **Résultat** :
  * `Taux de perte : 5%` : 5% des paquets ont été jetés car le réseau était plein.
  * `Attente moyenne` : Temps moyen passé dans les bouchons.

### Option 8 : K-Plus Courts Chemins (Bonus)

* **But** : Trouver non pas LE meilleur, mais les 3, 5 ou 10 meilleurs chemins. Utile si le chemin principal tombe en panne.
* **Entrée** : Source, Destination, et K (nombre de chemins voulus).
* **Résultat** : Liste classée des solutions alternatives.

### Option 9 : Exporter Visualisation (Bonus)

* **But** : Créer une image du réseau.
* **Action** : Donne un nom de fichier (ex: `reseau.dot`).
* **Utilisation** : Le fichier `.dot` généré permet de dessiner le graphe (via le site web viz-js.com ou l'outil Graphviz). Cela permet de VÉRIFIER visuellement que votre graphe (Option 1) a bien été chargé.
