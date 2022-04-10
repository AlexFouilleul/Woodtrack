# Bienvenue sur Woodtrack - Une solution pour prévenir du vol de bois🌳

<p align="center">
  <img src="Images/Logo.png" width="100">
</a>

  </p>

## 💡 Introduction

Woodtrack est un projet de 2ème année d'école d'ingénieur en filière IESE à Polytech Grenoble. Ce projet vise à proposer une solution destinée à la scierie [Bois Du Dauphinée (BDD)](https://www.scierie-bdd.com/) afin de contrer le problème de vol de bois lors de la livraison de celui-ci.

## ⚙️ Fonctionnalités
- Le système
  - Traquer plusieurs positions GPS
  - Communication Bluetooth Low Energy (BLE)
  - Longue durée de batterie
  - Peu couteux

- Application Android
  - Affichage des positions sur la carte avec des marqueurs
  - Les marqueurs peuvent donner la latitude, longitude et l'heure
  - Postionner la localisation sur Google Maps
  - Stockage des positions *(en cours de développement)*

## Guide de démarrage et d'utilisation

### Mise en place du système

1. Connectez l'Arduino au PC et téléversez le programme.
2. Soudez les composants sur le PCB en faisant attention au sens de branchement du connecteur de pile.
3. Connectez la pile et démarrez le système en utilisant l'interrupteur situé sous le module GPS.
4. Installez le boitier sur la sangle.
5. Insérez le système à l'intérieur de la boite et refermez la à l'aide du couvercle.
6. Téléchargez le fichier .apk sur votre smartphone et installez-le.

### Utilisation du système
1. Amusez-vous à fermer et ouvrir la sangle à différents endroits.
2. Ouvrez la sangle, lancez l'application puis cliquez sur le bouton **Scanner**.
3. Sélectionner le traqueur Woodtrack.
4. Patientez quelques secondes pour ensuite apercevoir les différentes positions GPS sur l'application.
5. Vous pouvez cliquer sur les marqueurs et zommer/dézommer pour avoir plus d'informations sur les positions.
6. Une fois la session terminée pensez à vous déconnecter en utilisant le bouton **Déconnexion**.

## 🖼️ Démonstration

*(Ajouter photos et vidéos)*
<p align="center">
  <img src="Images/Demo.png" width="100">
</a>

## 💻 Matériel

- Arduino Nano 33 Ble
- Adafruit Ultimate GPS
- Interrupteur à lame souple (ILS)
- Résistance 1kΩ
- Interrupteur 
- Pile 9V
- Connecteur Pile 9V
- Une sangle à cliquet
- PCB (fichiers fournis)
- Boitier (fichiers fournis)

## 🧑‍💻 Contributeurs

Anas Bachri - [anas.bachri@etu.univ-grenoble-alpes.fr](mailto:anas.bachri@etu.univ-grenoble-alpes.fr)<br>
Alex Fouilleul - [alex.fouilleul@etu.univ-grenoble-alpes.fr](mailto:alex.fouilleul@etu.univ-grenoble-alpes.fr)<br>
Sarah Pallaro - [sarah.pallaro@etu.univ-grenoble-alpes.fr](mailto:sarah.pallaro@etu.univ-grenoble-alpes.fr)<br>

Polytech Grenoble 
Université Grenoble Alpes

## 📝 Licence

Shield: [![CC BY-NC-SA 4.0][cc-by-nc-sa-shield]][cc-by-nc-sa]

This work is licensed under a
[Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License][cc-by-nc-sa].

[![CC BY-NC-SA 4.0][cc-by-nc-sa-image]][cc-by-nc-sa]

[cc-by-nc-sa]: http://creativecommons.org/licenses/by-nc-sa/4.0/
[cc-by-nc-sa-image]: https://licensebuttons.net/l/by-nc-sa/4.0/88x31.png
[cc-by-nc-sa-shield]: https://img.shields.io/badge/License-CC%20BY--NC--SA%204.0-lightgrey.svg
