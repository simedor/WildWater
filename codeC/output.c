# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include "header.h"

void afficher_noeud(Usine* u, FILE* f) {
    fprintf(f, "%s;%ld\n", u->id, u->capacite);
}

void parcours_infixe_inverse(Arbre* a, FILE* f) {
  if (a != NULL) {
    parcours_infixe_inverse(a->droit, f);
    afficher_noeud(a->usine, f);
    parcours_infixe_inverse(a->gauche, f);
  }
}

void output_histo(char* nom_fichier, Arbre* racine, char* mode) {
    FILE* f = fopen(nom_fichier, "w");
    if (f == NULL) {
        fprintf(stderr, "Erreur : Impossible de créer le fichier %s\n", nom_fichier);
        exit(2);
    }
    fprintf(f, "identifier;max volume (k.m3.year-1)\n");
    parcours_infixe_inverse(racine, f, mode);
    fclose(f);
