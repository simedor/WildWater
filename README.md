# Projet WildWater

## **Auteurs :** <br />

**GODINEAU Alexandre**  (https://github.com/Alegodix) <br /> 
**Siméon Dorninger** (https://github.com/simedor) <br /> 
**Timothée Renouf** (https://github.com/timothee23-gh) <br /> 
**Promotion :** Pré-Ing2 2025-2026 <br />


## Description

Ce projet est un programme un programme permettant de faire la
synthèse de données d’un système de distribution d’eau

---

## Compilation et Installation

### Prérequis
- Environnement Linux/Unix.
- `gcc` et `make` pour la compilation.
- `gnuplot` pour la génération des graphiques.

### Commandes de build

Mettez le fichier .dat/.csv que vous souhaitez analyser à la racine.

Pour compiler le projet et générer l'exécutable, placez-vous à la racine du projet cloné et exécutez :
- ```bash
  make clean && make
  chmod +x Launch.sh

### Utilisation


#### **Pour générer les histogrammes des usines :**
- pour les valeurs maximales :
  ```bash
  ./Launch.sh <fichier .csv> histo max
- pour les valeurs sources :
  ```bash
  ./Launch.sh <fichier .csv> histo src
- pour les valeurs réelles :
  ```bash
  ./Launch.sh <fichier .csv> histo real

#### **Pour avoir les fuites (leaks) :**
- ```bash
  ./Launch.sh <fichier .csv> leaks "<nom de l'usine précis>"

## Structure du dépôt GitHub:

```text ProjetWildWater/
├── data/              # Pour stocker le fichier CSV source
├── codeC/             # Dossier contenant  tout le code C
│   ├── main.c         # Point d'entrée du C
│   ├── avl.c          # Fonctions de gestion des AVL (rotation, insertion...)
│   ├── input.c        # Gestion de la lecture du CSV
│   └── output.c       # Gestion de l'écriture des fichiers temporaires
├── include/           # Dossier contenant  tous les fichiers .h
│   ├── header.h       # Déclarations globales (structures, prototypes)
│   └── couleurs.h     # Ensemble de raccourcis pour les couleurs
├── Makefile           # Fichier de compilation 
├── tests/             # Dossier pour les preuves de tests (images, logs)
├── Launch.sh          # Le script Shell (point d'entrée principal)
└── README.md          # Instructions de compilation/utilisation
``` 
