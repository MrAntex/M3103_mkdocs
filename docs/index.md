#<center> Introduction</center>

<center>Documentation Programme Algo Avancée - M3103 - CHIGNARD Julien</center>

<br/>

## Le Programme

Le programme consiste en un programme de comptage du nombre d'occurences des mots dans un fichier texte. En l'occurence le fichier texte utilisé sera *Voyage au centre de la Terre*, de Jules Verne.

Ce programme s'appuiera sur l'utilisation de table de hachage grâce à la librairie `unordered_map`.

Le programme permet de lister tous les différents mots contenus dans ce document texte ainsi que leurs occurences respectives.

Dans un premier temps, il est possible de rechercher un mot dans le livre et d'en connaître le nombre d'occurences.

Il est ensuite possible de tester les performances de la table de hachage. Ceci est fait en calculant le temps mis pour insérer et rechercher un mot dans des tables de tailles variables grâce à la librairie `chrono`.

Mis en rapport avec le taux de charge de la table, on peut se rendre compte des moments ou une insertion à nécéssité une expansion de la table.

L'évolution des temps d'insertion aisi que des temps de recherche nous servent à montrer la complexité de la table de hachage utilisée, qui est de *O(1)* dans les deux cas (à l'exception des insertions demandant des réallocations quand c'est nécéssaire).

## Build

Tapez simplement `make` et le Makefile se chargera du reste.
