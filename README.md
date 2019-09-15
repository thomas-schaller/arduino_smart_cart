

## Description

Ce code permet de controller voiture quatre roues motrices controlée par un arduino uno couplé un sensor shield et un motor driver.
Les roues sont actionnées par 4 servomoteurs.
Le kit est disponible à https://www.banggood.com/Multifunction-Bluetooth-Controlled-Robot-Smart-Car-Kits-For-Arduino-p-906628.html?cur_warehouse=CN
![image de la voiture](https://imgaz1.staticbg.com/thumb/large/oaupload/banggood/images/80/19/c7809f37-cbfb-48ff-af2f-34c601e470a1.jpg)

### mode manuel

Un recepteur infrarouge permet de controler la voiture à distance avec la télécommande infrarouge.
Un recepteur bluetooth permet de la commander également avec un téléphone.
Il est possible :
* d'avancer
* tourner à droite,
* tourner à gauche,
* reculer,
* s'arreter.

### mode suiveur de ligne

En appuyant sur "*" de la télécommande, la voiture entre en mode suiveur de ligne.
Trois capteur sous la voiture permet de détecter une ligne noire, et la voiture suivra alors cette ligne.
Il faut appuyer sur "OK" de la télécommande pour revenir en mode manuel.

### mode ultrason

En appuyant sur "#" de la télécommande, la voiture est autonome. Elle utilise les ultrasons pour détecter les obstacles et les éviter.
Il faut appuyer sur "OK" de la télécommande pour revenir en mode manuel.

## Origine
Le code a été originalement trouvé sur le site http://www.Banggood.com et adapté par https://www.TD72PRO.com.
J'ai repris le code de ce dernier ou j'ai factorisé les fonctions de contrôle des moteurs qui se répétaient.

## Ajout de fonctionnalités au code d'origine

J'ai apporté des modifications mineurs au code :
* J'ai ajouté un ecran lcd 2 ligne qui permet d'afficher du texte. Il est utilisé actuellement pour afficher le mode courant et les touches de la télécommande infrarouge.
* En mode infrarouge, le robot teste la distance avant d'avancer tout droit. Dans ce cas, un message d'avertissement indique la présence d'un obstacle.
* J'ai ajouté une led d'avertissement qui s'allume lorsqu'un message d'avertissement s'affiche.
