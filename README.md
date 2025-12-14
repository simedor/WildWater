# ===================================================================
# Projet WildWater
# ===================================================================

## Compilation : 
### linux : <br />
- make clean <br />
- make <br />
- ./Shell.sh data/data.csv histo src <br />

**Auteurs :** <br />

**GODINEAU Alexandre**  (https://github.com/Alegodix) <br /> 
**Siméon Dorninger** (https://github.com/simedor) <br /> 
**Timothée Renouf** (https://github.com/timothee23-gh) <br /> 
**Promotion :** Pré-Ing2 2025-2026 <br />

## Projet d'info 3

## Structure du dépot GitHub:

```text ProjetWildWater/
├── data/              # Pour stocker le fichier CSV source
├── codeC/             # Dossier contenant  tout le code C
│   ├── Makefile       # Fichier de compilation 
│   ├── main.c         # Point d'entrée du C
│   ├── header.h       # Déclarations globales (structures, prototypes)
│   ├── avl.c          # Fonctions de gestion des AVL (rotation, insertion...)
│   ├── input.c        # Gestion de la lecture du CSV
│   └── output.c       # Gestion de l'écriture des fichiers temporaires
├── tests/             # Dossier pour les preuves de tests (images, logs)
├── myScript.sh        # Le script Shell (point d'entrée principal)
└── ReadMe.md          # Instructions de compilation/utilisation
``` 
