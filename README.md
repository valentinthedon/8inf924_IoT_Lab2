# 8INF924 - Laboratoire 2
## Introduction
Au sein de ce dépot vous trouverez l'ensemble des livrables du Laboratoire 2 du cours "8INF924 - Internet of Things" de l'UQAC. Ce projet a été réalisé par :

* Valentin THEDON - THEV10079801
* Samia CHAHBOUNIA - CHAS24580000

Dans ce README, nous répondrons à l'ensemble des questions du TP et expliciterons notre démarche.

## Environnement

### Hardware
* [Arduino MKR WAN 1310](https://docs.arduino.cc/hardware/mkr-wan-1310)
* [Capteur de gaz](https://wiki.dfrobot.com/Analog_Gas_Sensor_MQ9__SKU_SEN0134) : permet de detecter différents gaz tel que le Monoxyde de Carbone, le méthane, le propane, etc. Ce capteur fonctionne grâce à la sensibilité du [dioxyde d'étain](https://fr.wikipedia.org/wiki/Dioxyde_d%27%C3%A9tain) à la présence de ces gazs dans l'air. En utilisant la lecture analogique, il nous est possible de récupérer la valeur du capteur, ici ce sont des ppm, qui mesure la pollution dans l'air. Pour les gaz combustibles (type méthane, propane, etc.) il peut capter jusqu'à 10 000 ppm.
### Software
Pour le développement des logiciels embarqués dans le microcontrôleur, nous avons utilisé [Arduino IDE](https://wiki-content.arduino.cc/en/software)

## Exercice 1
Pour l'exercice 1, le principe est simple, c'est le même que pour le [Laboratoire 1](https://github.com/valentinthedon/8inf924_IoT_Lab1). Nous lisons sur un pin analogique la valeur du capteur de gaz que nous affichons en console via le *Serial Monitor*. Il est également possible de visualiser l'acquisition des données via un graphique en ouvrant le *Serial Plotter*

Le microcontrôleur utilisé dans ce laboratoire n'est pas le même que celui utilisé dans le premier laboratoire. En effet, nous exploitons ici, un [Arduino MKR WAN 1310](https://docs.arduino.cc/hardware/mkr-wan-1310) qui a pour principale différence avec l'[Arduino MKR Wifi 1010](https://docs.arduino.cc/hardware/mkr-wifi-1010), la méthode de transmission des données, le 1010 dispose du Wifi ce qui permets de pouvoir transmettre de la données sur de la courte portée (une centaine de mètre au maximum). de l'autre coté le 1310 utilise le protocole [LoRa](https://lora-alliance.org/) permettant d'atteindre un portée de plusieurs kilomètres.

## Exercice 2

### Partie 1

L'exercice 2 consistait dans un premier temps à mettre en place un compteur classique et d'afficher la donnée sur le cloud de [The Things Network](https://nam1.cloud.thethings.network/). Pour cela, il est nécéssaire d'ajouter notre microcontrôleur à la plateform TTN.

![Affichage TTN de la reception du compteur](/exo2-1_TTN.PNG)

Une fois la connexion établie, on remarque la reception de données sur le cloud TTN. Comme le montre l'image ci-dessus, il semblerait que les valeurs recus par TTN soit discontinues. En jettant un oeil au *Serial Monitor*, on observe, qu'un grand nombre de paquet n'a été envoyé. Cela est dû aux limites du microcontrôleur ainsi qu'a sa méthode de transmission, qui limite la bande passante et la cadence d'envoi des paquets. Pour solutionner ce problème, j'ai trouvé plusieurs solutions : 
* Placer un `delay()`, avec environ 20 secondes. ce qui va ralentir notre compteur et donc notre envoi de donnée pour ne pas avoir de pertes.
* La deuxième solution que nous avons pu trouver été de creer des paquets plus gros, mais avec l'ensemble des données collecté entre 2 envois
* Enfin nous exploiterons une dernière solution dan la partie 2 de cette exercice.

### Partie 2

Pour la mise en place d'envoi de données capteurs, rien de plus simple, il nous suffit tout d'abord de remplacer la valeur envoyé, le compteur, par la donnée capteur. Encore une fois, avec le `decoder.js`, nous pouvons lire voir la bonne réception des données capteurs.

Comme explicité plus haut, nous ne pouvons envoyer nos données de manière trop rapide. Pour palier à ce problème, nous avons fixer un intervalle d'envoi de paquet de 20s. Afin de minimiser la perte de donnée et coller au plus proche de la réalité. Nous récuperons une valeur capteur toutes les secondes (un surplus de données pourrait entrainer des problèmes de mémoire). En 20 secondes nous recevons donc 20 données, et pour éviter des paquets trop grand, nous faisons donc la moyenne de ces 20 données, moyenne que nous envoyons dans le cloud TTN.

![Affichage TTN de la reception du compteur](/exo2_TTN.PNG)

Par la suite, nous connectons notre microcontrôleur à *Cayenne MyDevice*. Nous créons la connexion puis modifions les *Payload Formatters* dans TTN pour que les données soit compréhénsible pour *Cayenne*. De plus, nous avons modifié le widget pour y mettre un jauge avec différentes couleurs selo la valeur. Cela nous semblait plus intérréssant pour une donnée de ce type. Voilà ci dessous, le tableau de bord *Cayenne*

![Affichage TTN de la reception du compteur](/exo2_Cayenne.PNG)

**NB :** Comme vous pouvez le constater, la donnée capteur n'a pas les même valeurs que dans la partie précédente. Cela est du à un problème de type. Cela serait dû au fait que l'entier soit signé. Pour palier a ce problème, nous divisons par 100 la donnée à envoyer.

## Exercice 3

Enfin, dans ce dernier exercice nous mettons en place la rétroaction du réseau LoRa qui consiste à allumé la LED intégré à l'Arduino.

Pour ce faire nous créons dans la fonction `setup()`, un paquet permettant d'indiquer à *Cayenne* que nous avons un canal digital souhaitant recevoir de la donnée. Nous utilisons la fonction `addDigitalOutput()`. Suite à cela, un nouveau widget apparaît sur *Cayenne*, nous le transformons en bouton. 

Pour finaliser tout cela, nous modifions notre code pour qu'il lise les données de la gateway, et allume ou eteigne la LED en consequence.

Ci-dessous, notre interface TTN et notre tableau de bord final *Cayenne*.

![Affichage TTN de la reception du compteur](/exo3_TTN.PNG)

**NB :** Notez la présence d'un paquet, en première ligne permettant d'éteindre la LED.

![Affichage TTN de la reception du compteur](/exo3_Cayenne.PNG)
## Problèmes rencontrés

Au cours de ce laboratoire, nous avons dû faire face à un problème récurent, occasionnant des ralentissements. En effet à chaque flash de code, l'Arduino, pouvait eprouver une grande difficulté à se connecter au cloud TTN, ce qui pouvait nous faire attendre jusque 10 min parfois.

![Bug Gateway](/gateway_bug.PNG)

## Références

- [Bug des entiers signés](https://www.thethingsnetwork.org/forum/t/cayenne-lpp-format-analog-data-wrong/14676)
- [Documentation CayenneLPP lib](https://docs.mydevices.com/docs/lorawan/cayenne-lpp)
- [Exemple d'utilisation CayenneLPP](https://www.thethingsnetwork.org/docs/devices/arduino/api/cayennelpp/)
- [Connecter un MKR1310 à TTN](https://docs.arduino.cc/tutorials/mkr-wan-1300/the-things-network)
-[Documentation MKRWAN lib](https://www.arduino.cc/reference/en/libraries/mkrwan/)