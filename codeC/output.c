#include "header.h"

/*
Écrit les données d'une usine dans le fichier selon le mode choisis (src, max, real) et gère la conversion d'unité.
*/
void afficherNoeud(Usine* u, FILE* fichier, char* mode) {
    if (u == NULL || fichier == NULL) exit(1);
    double valeur = 0;
    if (strcmp(mode, "src") == 0) {
        valeur = u->volumeSource;
    } else if (strcmp(mode, "max") == 0) {
        valeur = u->capacite;
    } else if (strcmp(mode, "real") == 0) {
        valeur = u->volumeTraite;
    } else {
        fprintf(stderr, "Erreur : Mode '%s' non reconnu.\n", mode);
        fclose(fichier);
        exit(1);
    }
    fprintf(fichier, "%s;%.3f\n", u->ID, valeur / 1000.0);
}

/*
Ecrit les données dans l'ordre alphabétique inverse des identifiants.
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

    // Ecriture de l'en-tête CSV
    if (strcmp(mode, "src") == 0) {
        fprintf(fichier, "identifier;volume source(M.m3.year-1)\n");
    } else if (strcmp(mode, "max") == 0) {
        fprintf(fichier, "identifier;capacite(M.m3.year-1)\n");
    } else if (strcmp(mode, "real") == 0) {
        fprintf(fichier, "identifier;volume traite(M.m3.year-1)\n");
    } else {
        fprintf(stderr, "Erreur : Mode '%s' non reconnu.\n", mode);
        fclose(fichier);
        exit(1);
    }

    // Ecriture des données
    parcoursInfixeInverse(a, fichier, mode);
    
    fclose(fichier);
}

