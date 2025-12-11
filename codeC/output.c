#include "header.h"

/*
Ecrire une ligne de données pour une usine et gère la conversion d'unités
*/
void afficherNoeud(Usine* u, FILE* fichier, char* mode) {
    double valeur = 0;
    if (strcmp(mode, "src") == 0) {
        valeur = u->volumeSource;
    } else if (strcmp(mode, "max") == 0) {
        valeur = u->capacite;
    } else if (strcmp(mode, "real") == 0) {
        valeur = u->volumeTraite;
    }
    fprintf(fichier, "%s;%.3f\n", u->ID, valeur / 1000.0);
}

/*
Ecrire les identifiants dans l'ordre alphabétique inverse
*/
void parcoursInfixeInverse(pAVL a, FILE* fichier, char* mode) {
  if (a != NULL) {
    parcoursInfixeInverse(a->fd, fichier, mode);
    afficherNoeud(a->u, fichier, mode);
    parcoursInfixeInverse(a->fg, fichier, mode);
  }
}

/* 
Générer le fichier CSV pour faire l'histogramme
*/
void outputHisto(char* nomFichier, pAVL a, char* mode) {
    FILE* fichier = fopen(nomFichier, "w");
    if (fichier == NULL) {
        fprintf(stderr, "Erreur : Impossible de créer le fichier %s\n", nomFichier);
        exit(1);
    }
    if (strcmp(mode, "src") == 0) {
        fprintf(fichier, "identifier;volume source(M.m3.year-1)\n");
    } else if (strcmp(mode, "max") == 0) {
        fprintf(fichier, "identifier;capacite(M.m3.year-1)\n");
    } else if (strcmp(mode, "real") == 0) {
        fprintf(fichier, "identifier;volume traite(M.m3.year-1)\n");
    }
    parcoursInfixeInverse(a, fichier, mode);
    fclose(fichier);
}

