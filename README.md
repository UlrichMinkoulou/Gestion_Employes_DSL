------------------------------
## Employee Management & Secure Messaging System## Technical Service Edition | Drone.Solutions
Ce système de gestion d'entreprise et de messagerie sécurisée est une application C++17 optimisée pour les interventions rapides du service technique de Drone.Solutions. Conçue pour l'efficacité, elle repose sur une architecture légère en ligne de commande (CLI), garantissant une exécution instantanée sans les dépendances d'une interface graphique lourde.
## Architecture & Performance
L'application a été développée avec une priorité absolue sur la fiabilité et la rapidité d'exécution :

* Mise en cache intelligente : Système de caching propriétaire réduisant les accès disques redondants vers SQLite.
* Sécurité Industrielle : Utilisation de libsodium pour le hachage cryptographique des accès (Argon2id).
* Gestion Mémoire : Utilisation intensive des Smart Pointers (std::unique_ptr, std::shared_ptr) pour garantir l'absence de fuites mémoire.
* Rendu Technique : Génération de documents et de QR Codes via des intégrations bas niveau (stb_image, canvas_ity).

## Assurance Qualité
La stabilité du système est assurée par une suite de 17 tests unitaires développés sous le framework GoogleTest (gtest). Ces tests couvrent :

* L'intégrité des transactions SQL.
* La validité des algorithmes de chiffrement.
* La persistance et la cohérence du cache.
* La robustesse des flux de messagerie.

## Prérequis

* Compilateur : Supportant le C++17 (GCC 7+, Clang 5+).
* Bibliothèques : sqlite3, libsodium, googletest.
* Système de build : cmake (version 3.11 ou supérieure).

## Installation et Compilation

# Installation des dépendances (Linux)
sudo apt-get install libsqlite3-dev libsodium-dev libgtest-dev
# Génération du projet et compilation
cmake -B build
cmake --build build
# Lancement des tests unitaires
./build/run_tests
# Lancement de l'application
./build/main

## Structure du Projet

* src/ : Logique métier et implémentation des services.
* include/ : Définitions des interfaces et structures de données.
* tests/ : Suite complète de validation industrielle sous GoogleTest.
* lib/ : Intégrations tierces (QR Code, Image processing, Canvas).


## Quality Assurance & Testing

La fiabilité de l'architecture est garantie par une suite de tests unitaires rigoureux utilisant le framework GoogleTest (gtest). Chaque module critique est validé pour assurer l'intégrité des données et la stabilité du système.
Bilan visuel des tests

<p align="center">
  <img src="/Test_results.png" alt="GoogleTest Results">
  <br>
  <i>Capture d'écran montrant le succès des 20 tests unitaires répartis sur les suites DataBaseTest et Admin_dbTest.</i>
</p>

Logic métier : Validation des flux d'adhésion et de recherche.
Sécurité : Vérification des protocoles de hachage et des accès admin.
Messagerie : Tests d'intégrité sur l'envoi et la réception des messages.
Performance : Monitoring des temps de réponse (moyenne < 400ms).

## À propos
Cette version a été spécifiquement optimisée pour un usage exclusif par le service technique de Drone.Solutions, privilégiant la réactivité système et la sécurité des données.
------------------------------

