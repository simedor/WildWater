#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef struct usine {
  char* ID;
  float capte;
  float capacitemax;
  float conso;
}Usine;
typedef struct avl {
  struct *Usine u;
  struct *avl fg;
  struct *avl fd;
  int eq;
}AVL, *pAVL;

Arbre* creerArbre(char* id);

Arbre* insertionAVL(Arbre* a, char* id, long volume, int* h_change);

Arbre* equilibreAVL(Arbre* a);

void libererArbre(Arbre* a);

void afficherArbreInverse(Arbre* a);

int max(int a, int b);
int min(int a, int b);
