#<center>Fonctionnement</center>

<center>Comment le programme fonctionne étape par étape et les bouts de code correspondants.</center>

<br/>
##Commandes & options
Les options sont gérée grâce à la librairie `getopt`.

La commande de base est la suivante : `./hachageJV`, à laquelle s'ajoutent plusieurs options que sont les suivantes :

* `-m [mot]` - Affiche le nombre d'occurences d'un mot dans le fichier.
* `-c` - Mode de calcul des temps, sans cette option le programme se contente de remplir la table de hachage (et d'y rechercher un mot si l'option -m est utilisée).
* `-d` - Mode d'édition de données. Active l'écriture des données de temps sur un fichier .dat quand utilisée avec l'option -c.
* `-a [min]` - Borne inférieure de la range de taille de table à tester. Est fixée à 10 par défaut. Doit être positive et inférieure à la borne supérieure.
* `-z [max]` - Borne supérieure de la range de la taille de table à tester. Est fixée à 300 par défaut. Doit être supérieure à la borne inférieure et inférieure à 13 996 (nombre de mots différents dans Voyage au centre de la Terre).
* `-i [iterations]` - Nombre d'itérations pour chaque calcul de moyenne de temps. Augmenter ce nombre donnera des résultats plus lisses au prix de la performance. Est fixée à 10 par défaut. Doit être positive.
* `-p [pas]` - Pas pour tester chaque taille de table dans la range indiquée. Est fixé à 1 par défaut. Doit être positif.
* `-f [fichier]` - Permet de choisir le fichier de sortie du nombre d'occurences des mots. Attention ce fichier sera écrasé sans confirmation au début du programme. Est fixé à 'occurences.txt' par défaut.

<br/>
##Initialisation

Après vérification des options et initialisation des différentes variables, le programme commence par ouvrir le fichier texte pour en compter les occurences des différents mots :

    std::ifstream ifs; # Déclaration d'un flux d'entrée.
    ifs.open(ficTexte,std::ifstream::in); # Ouverture du fichier texte à analyser en lecture.

Ensuite, grâce à la librairie `vector` et pour éviter des souçis de performance, il va nous falloir stocker tout le contenu de ce fichier texte dans une variable. Cette variable sera dans notre cas un vecteur de string (librairie `cstring`) :

    std::vector<std::string> livre; # Cette déclaration est en réalité faite avant mais cela aide à la compréhension.

    while(ifs >> mot && ifs.good()) # Tant que l'on est pas arrivé à la fin du fichier
		livre.push_back(mot);       # On ajoute chaque mot dans le vecteur.

Une fois cela fait, le programme est prêt à fonctionner.

<br/>
##I - Nombre d'occurences

Le premier mode de fonctionnement de ce programme est le comptage du nombre d'occurences de chaque mot présent dans le fichier texte et la possibilité de rechercher le nombre d'occurences d'un mot. Voici le fonctionnement de cette partie :

###Incrémentation

Il nous faut pour commencer entrer tous les mots dans la table de hachage, on va faire cela en se servant du mot-clé `auto` :

    for (auto& mot: livre) # Va parcourir tous les index du vecteur de mots.
      map[mot]++;          # On incrémente son nombre d'occurrence dans la table de hachage.

Les avantages de cette syntaxe sont que *auto* est plus efficace qu'une simple boucle for en terme de mémoire, et `map[mot]++;` couvre aussi bien le cas où le mot est déja présent dans la table que celui où il ne l'est pas (dans ce cas il créera cette clé et initialisera sa valeur à 1).

###Recherche d'un mot

Une fois que la table de hachage est remplie, nous pouvons rechercher le nombre d'occurences d'un mot en particulier :

    std::unordered_map<std::string, int>::const_iterator motch;

    motch= map.find(motcherche); # On regarde si le mot cherché est bien dans la table.

    if(motch != map.end()) # Si c'est le cas
    std::cout << "Nombre d'occurences du mot '"<<motcherche<<"' : "<<map[mot]<<std::endl; # On affiche son nombre d'occurences.

    else # Sinon
    std::cout << "Le mot : '" << motcherche <<"' ne figure pas dans le fichier." << std::endl; # On indique qu'il ne figure pas dans la table.

###Sortie

Pour sortir tous les mots et leurs occurences, il faut d'abord vider le fichier précédent d'occurences, pour ne pas provoquer de doublons :

    # Cette suite dopérations est en réalité effectuée plus tôt dans l'éxecution mais cela aide à la compréhension.

    std::ofstream ofs; # On crée le flux de sortie.

    std::cout << "Réinitialisation du fichier d'occurences..." << std::endl; # Pour savoir que la procédure à commencée.

    if( remove(writefic.c_str()) != 0 ) # Suppression du fichier.
      perror( "Echec de l'opération :/ " ); # Erreur
    else
      puts( "Fichier réinitialisé avec succès !" ); # Succès

    ofs.open(writefic, std::ofstream::out); # On réouvre le fichier en lecture, ce qui à pour effet de le recréer.

Ensuite, il faut bien évidemment le remplir :

    for (auto& x: map) # Nouvelle utilisation de auto pour parcourir la table de hachage.
      ofs << x.first << ": " << x.second << std::endl;
        # On écrit chaque mot (clé) avec son nombre d'occurences (valeur) dans le fichier.

<br/>
##II - Calcul de la complexité

Le second mode d'utilisation de ce programme est celui qui nous intérresse le plus : les temps d'insertion et de recherche.

Voici comment tout cela fonctionne :

###Tailles de la table
Le but de ce programme est de pouvoir observer la complexité des temps d'insertion et de recherche, donc de mesurer ces temps pour des tailles de table de hachage variables.

Pour ce faire on va dans un premier temps remplir la table jusqu'a la taille minimale de la range entrée en paramètre :

    while(map.size()<mininsert && map.size() < livre.size()){
        # Tant que la taille de la table est inférieure à la taille souhaitée et qu'on a pas entré tout le livre.

      int chosen = std::rand() % livre.size(); # On tire un chiffre aléatoire en tre 0 et le nombre de mots du livre.
      mot = livre[chosen]; # Cela nous donne un mot.
      map[mot]++; # On ajoute ce mot / incrémente son nombre d'occurences.
    }

Ensuite, un fois notre table de hachage initialisée à la taille minimale voulue, on rentre dans la boucle "principale" qui va pour chaque taille de la table voulue dans la range entrée en paramètre (en prenant le pas en compte) calculer le temps d'insertion, le temps de recherche, le taux de charge et écrire ces valeurs dans le fichier de sortie :

    for(uint w=mininsert;w<=maxinsertall;w=w+pas){
        # On va parcourir la range de taille de table voulue en prenant le pas entré en paramètre comme pas.

      while(map.size()<w && map.size()<livre.size()){ # On ajuste la taille de la table si nécessaire (si pas > 1).
        int chosen = std::rand() % livre.size();
        mot = livre[chosen];
        map[mot]++;
      }

      # Les calculs sont faits içi...
      # Sortie...
    }

Un fois cette boucle réalisée, nous pouvons nous pencher sur le reste.

###Temps de recherche

Le temps de recherche est calculé avant le temps d'insertion, pour ce faire on va simplement sélectionner un mot au hasard dans le livre :

    int chosen = (std::rand() % livre.size()); # Un index de mot au hasard dans le livre.
    mot = livre[chosen]; # On extrait le mot sélectionné.

Cela nous permet de pouvoir piocher aussi boen un mot qui existe déja dans la table comme un qui n'y figure pas (pour avoir des résultats reflétant la réalité).

On a plus qu'a rechercher ce mot dans la table. cette recherche est effectuée grâce à la fonction `find()` de `unordered_map`. Qui renvoie l'index du mot recherché dans la table si il s'y trouve, et `std::unordered_map::end` sinon.

On initialise donc une variable pour pouvoir stocker ce résultat, bien qu'on ne s'en serve pas par la suite :

    std::unordered_map<std::string, int>::const_iterator found; # Initialisation du conteneur de la sortie du find().

Enfin, pour calculer le temps pris pour cette recherche, nous nous servons de la librairie `chrono` grâce à laquelle nous allons créer 2 time points en nanosecondes : le premier juste avant la recherche et le second juste après. Nous pouvons ensuite récupérer le temps pris pour la recherche grâce à la différence de ces deux time points :

    std::chrono::steady_clock::time_point debfind = std::chrono::steady_clock::now(); # Premier time point T1.

    found = map.find(mot); # On recherche le mot.

    std::chrono::steady_clock::time_point endfind = std::chrono::steady_clock::now(); # Second time point T2.

    std::chrono::nanoseconds time_find = std::chrono::duration_cast<std::chrono::nanoseconds>(endfind - debfind);
      # T2 - T1 pour avoir la durée de la recherche.

On récupère donc le temps pris par la recherche dans la variable `time_find`, en nanosecondes.

###Temps d'insertion

Comme pour la recherche, le temps d'insertion début par le choix d'un mot au hasard dans le livre :

    int chosen = (std::rand() % livre.size()); # Un index de mot au hasard dans le livre.
    mot = livre[chosen]; # On extrait le mot sélectionné.

Ensuite on insère le mot dans la table et on chronomètre le temps pris pour cette insertion comme pour le temps de la recherche, grâce à 2 time points avant et après l'insertion :

    std::chrono::steady_clock::time_point debinsert = std::chrono::steady_clock::now(); # T1

      newmap[mot]++; # Insertion

    std::chrono::steady_clock::time_point endinsert = std::chrono::steady_clock::now(); # T2

    std::chrono::nanoseconds time_insert = std::chrono::duration_cast<std::chrono::nanoseconds>(endinsert - debinsert); # T2 - T1

###Taux de charge

La mesure du taux de charge est très importante car elle permet de voir quand la table devient saturée et quand les collisions ont le plus de chances d'arriver (cf : <a href="https://programming.guide/hash-table-load-factor-and-capacity.html">Load Factor</a>).

Grâce à l'évolution du taux de charge de la table, et en la mettant en parallèle avec les temps d'insertion, on pourra voir quand la table est automatiquement réallouée lorsque sa taille devient trop petite (et donc que son taux de charge est trop haut).

Ce taux de charge est calculé grâce à la fonction suivante :

    float tauxCharge = map.load_factor(); # Renvoie un float entre 0 et 1 qui correpond au taux de charge.

###Moyennes

Pour avoir des résultats plus lisses et pour éviter les erreurs occasionnelles qui pourraient provoquer des pics sur le graphe, on va donc réaliser plusieurs fois chaque recherche et chaque insertion et en faire les moyennes. Chaque recherche et chaque insertion sera faite avec des mots différents car ce n'est pas l'insertion ou la recherche d'un mot en particulier mais bien le comportement de la table pour une taille donnée.

Pour ce faire on va avoir besoin de 2 vecteurs (librairie `vector`) de `std::chrono::nanoseconds` dans lesquels nous allons stocker respectivement nos temps d'insertion et de recherche. On commence par les initialiser :

    std::vector<std::chrono::nanoseconds> tabtempsinsert; # Vecteur des temps d'insertion
    std::vector<std::chrono::nanoseconds> tabtempsfind; # Vecteur des temps de recherche

Ensuite, il va nous falloir réaliser chaque opérations plusieurs fois. Ce nombre de fois est paramétré par l'utilisateur et est stocké dans un `uint` nommé `iterations`.

Exemple pour la recherche :

    tabtempsfind.clear(); # On vide le vecteur qui a pu être rempli par les itérations précédentes.
    for(uint i=1;i<=iterations;++i){ # Pour le nombre d'itérations défini.

      # Procédure de recherche et mesure du temps pris...

      tabtempsfind.push_back(time_find); # On ajoute le temps trouvé au vecteur correspondant.
    }

On va ensuite faire la somme de tous les temps contenus dans le vecteur. Par gain de temps et pour nous éviter des casts évitables, on stockera ce temps dans la même variable que celle ayant servi à mesurer le temps juste avant :

    time_find = std::chrono::nanoseconds::zero(); # On remet le temps à 0.

    for(auto& n : tabtempsfind)                   # Pour chaque index du vecteur.
      time_find += n;                             # On incrémente la variable de ce temps.

Pour avoir une moyenne il nous faut ensuite diviser cette somme par le nombre de temps qu'elle contient, rien de plus simple :

    double totimefind = time_find.count() / (double)tabtempsfind.size();
                               # .count() permet de récupérer une valeur numérique du temps.
                                          # Le cast est nécéssaire pour une division.

On récupère donc la moyenne des temps de recherche dnas la variable `totimefind`. (Oui bon j'aurais pu trouver mieux comme nom je sais)

---
Le cas de l'insertion est très légéremment différent, bien que son début soit identique à celui de la recherche :

    tabtempsinsert.clear(); # On vide le vecteur

    for(uint i=1;i<=iterations;++i){ # Pour chaque itération.

    int chosen = (std::rand() % livre.size()); # On choisit un mot...
    mot = livre[chosen];


On ne pourra pas simplement insérer plusieurs mots à la suite et compter le temps, car la taille de la table va évoluer sans que l'on le veuille.

Pour pallier à ce souci, on va devoir pour chaque itération créer une copie de la table de hachage :

    std::unordered_map<std::string, int> newmap(map); # Constructeur par copie.

La suite est identique à la procédure de calcul de temps d'insertion expliquée plus haut à cela près qu'au lieu d'insérer le mot dans la table on l'insère dans la copie de la table :

    # Time point 1
    newmap[mot]++; # Insertion
    # Time point 2
    # time_insert = T2 - T1

    tabtempsinsert.push_back(time_insert); # Ajout du temps au vecteur des temps d'insertion.

La suite est identique à la recherche, en changeant de vecteur utilisé evidemment :

    time_insert = std::chrono::nanoseconds::zero(); # On remet le temps à 0.

    for(auto& n : tabtempsinsert) #Pour chaque temps dans le vecteur.
      time_insert += n; #On incrémente la variable de ce temps.

    double totimeinsert = time_insert.count() / (double)tabtempsinsert.size();
      # On en fait ensuite la moyenne.

On récupère la moyenne des temps d'insertion dans la variable `totimeinsert`.

###Sortie

La sortie se fait dans un fichier `data.dat` unique constitué de la sorte :

|<center>Taille table</center>|<center>Temps Insertion</center>|<center>Temps Recherche</center>|<center>Taux de charge</center>|
|:--------:|:--------:|:---------:|:----:|
|   0   | 2.69e-06 | 4.20e-07 | 0.001337 |
|   1   | 1.337e-05 | 3.86e-06 | 0.0713705 |
|   2   | ... | ... | ... |

Le fichier à été initialisé comme l'a été celui du nombre d'occurences (expliqué plus haut), et est donc vide au début de l'écriture. Pour écrire dedans on va simplement séparer toutes les données par un espace :

    ficall<<map.size()-1<<" "<<totimeinsert/100000000<<" "<<totimefind/100000000<<" "<<map.load_factor()<<std::endl;
    # On divise tous les temps par 100 000 000 pour avoir des secondes.

<br/>

---
####Barre de progression

Pour se rendre compte de la progression du programme, car son utilisation peut prendre beaucoup de temps, j'ai réalisé une barre de progression.

Elle suit les tailles de table testées, on ne sait pas précisemment à quelle itération on en est (pour les moyennes) mais on sait quelle taille de table est testée.

Voici son fonctionnement :

On commence par trouver le pourcentage de progression actuel, qui est pris en utilisant la position actuelle de la boucle (`w`) et en la comparant avec son maximum (`maxinsertall`), auquel on soustrait le point de départ (`mininsert`) car sinon une étude sur des tailles allant par exemple de 5000 à 10 000 commençerait à 50% dès le début.

    float progress = (float)(w-mininsert)/(maxinsertall-mininsert); # Progression en pourcentage.
    int pos = 60 * progress; # On le multiplie par 60 (taille arbitraire de la barre en caractères).

Vient ensuite la barre en elle-même :

    std::cout << "["; # Le coté gauche de la barre, fixe.

    for (int k = 0; k < 60; ++k) { # Pour toute la longueur de la barre.
      if (k < pos) std::cout << "#"; # Si on est avant la position de la progression, on affiche un "#".
      else if (k == pos) std::cout << ">"; # Si on est exactement à cette position, on affiche un ">".
      else std::cout << " "; # Et sinon on affiche un espace (barre vide).
    }

    std::cout << "] " << std::ceil(progress * 100.0) << " % | "<<std::ceil((w-mininsert)/pas)<<"/"<<std::ceil((maxinsertall-mininsert)/pas)<<"\r";
      # On affiche la progression en %, on prend l'arrondi supérieur pour finir à 100% et non à 99%.
      # Vient ensuite le retour numérique, on calcule à quel test on en est sur le nombre total de tests à réaliser en prenant en compte les pas.
      # On finit par un \r pour ramener le curseur au début de la ligne pour préparer la prochaine progression.

    std::cout.flush(); # Empêche un retour à la ligne automatique.

La barre s'affiche ensuite comme ceci :

    [##################################>                        ] 59% | 779/1337
