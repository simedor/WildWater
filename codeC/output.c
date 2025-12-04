# include "header.h"

void afficher_noeud(Usine* u, FILE* f) {
    fprintf(f, "%s;%ld\n", u->id, u->vol_source);
}

void parcours_infixe_inverse(Arbre* a, FILE* f) {
  if (a != NULL) {
    parcours_infixe_inverse(a->droit, f);
    afficher_noeud(a->usine, f);
    parcours_infixe_inverse(a->gauche, f);
  }
}

void output_histo(char* nom_fichier, Arbre* racine) {
    FILE* f = fopen(nom_fichier, "w");
    if (f == NULL) {
        fprintf(stderr, "Erreur : Impossible de créer le fichier %s\n", nom_fichier);
        exit(2);
    }
    fprintf(f, "identifier;source volume (k.m3.year-1)\n");
    parcours_infixe_inverse(racine, f);
    fclose(f);
}
