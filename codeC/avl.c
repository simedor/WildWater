#include "avl.h"

int hauteur(*Arbre a) {
  if (!a) return 0;
  return a->hauteur;
}

int equilibre(*Arbre a) {
  if (!a) return 0;
  return a
