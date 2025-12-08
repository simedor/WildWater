# include "header.h"

/*
Ecrire une ligne de données pour une usine et gère la conversion d'unités
*/
void afficherNoeud(Usine* u, FILE* fichier) {
    double volume = u->volumeSource / 1000.0;
    fprintf(fichier, "%s;%.3f\n", u->ID, volume);
}

/*
Ecrire les identifiants dans l'ordre alphabétique inverse
*/
void parcoursInfixeInverse(Arbre* a, FILE* fichier) {
  if (a != NULL) {
    parcoursInfixeInverse(a->fd, fichier);
    afficherNoeud(a->u, fichier);
    parcoursInfixeInverse(a->fg, fichier);
  }
}

/* 
Générer le fichier CSV pour faire l'histogramme
*/
void outputHistoSource(char* nomFichier, Arbre* a) {
    FILE* fichier = fopen(nomFichier, "w");
    if (fichier == NULL) {
        fprintf(stderr, "Erreur : Impossible de créer le fichier %s\n", nomFichier);
        exit(1);
    }
    fprintf(fichier, "identifier;source volume (k.m3.year-1)\n");
    parcoursInfixeInverse(a, fichier);
    fclose(fichier);
}
