# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include "header.h"

void parcours_infixe_inverse(Arbre* a, FILE* f, char* mode) {
  if (a != NULL) {
    parcours_infixe_inverse(a->droit, f, mode);
    afficher_noeud(a->usine, f, mode);
    parcours_infixe_inverse(a->gauche, f, mode);
  }
}
