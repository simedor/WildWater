# include "header.h"

/*
Ecrire une ligne de données pour une usine et gère la conversion d'unités
*/
void afficherNoeud(Usine* u, FILE* f) {
    double volume = u->volumeSource / 1000.0;
    fprintf(f, "%s;%.3f\n", u->ID, volume);
}

/*
Ecrire les identifiants dans l'ordre alphabétique inverse
*/
void parcoursInfixeInverse(Arbre* a, FILE* f) {
  if (a != NULL) {
    parcoursInfixeInverse(a->fd, f);
    afficherNoeud(a->u, f);
    parcoursInfixeInverse(a->fg, f);
  }
}

/* 
Générer le fichier CSV pour faire l'histogramme
*/
void outputHistoSource(char* nomFichier, Arbre* a) {
    FILE* f = fopen(nomFichier, "w");
    if (f == NULL) {
        fprintf(stderr, "Erreur : Impossible de créer le fichier %s\n", nomFichier);
        exit(1);
    }
    fprintf(f, "identifier;source volume (k.m3.year-1)\n");
    parcoursInfixeInverse(a, f);
    fclose(f);
}
