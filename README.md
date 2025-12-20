# Projet WildWater

## Compilation : 
### linux : <br />
- make clean <br />
- make <br />
- ./Shell.sh [nom du fichier que vous voulez analyser] [commandes histo ou leaks] <br />

### **Auteurs :** <br />

**GODINEAU Alexandre**  (https://github.com/Alegodix) <br /> 
**Siméon Dorninger** (https://github.com/simedor) <br /> 
**Timothée Renouf** (https://github.com/timothee23-gh) <br /> 
**Promotion :** Pré-Ing2 2025-2026 <br />

## Projet d'info 3

## Structure du dépot GitHub:

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
