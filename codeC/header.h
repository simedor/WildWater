#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <math.h>
typedef struct usine {
  char* ID;
  float capte;
  float capacitemax;
  float conso;
}Usine;

typedef struct AVL {
  struct *Usine u;
  struct *AVL fg;
  struct *AVL fd;
  int eq;
}AVL, *pAVL;

pAVL creerArbre(char* id, float ct, float capamax, float cs);

pAVL insertionAVL(pAVL a, char* id, float volume, int* h);

pAVL equilibreAVL(pAVL a);

void libererArbre(pAVL a);

void afficherArbreInverse(pAVL a);

int max(int a, int b);
int min(int a, int b);
