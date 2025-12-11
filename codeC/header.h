#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <math.h>

/*
Structure représentant les données d'une usine
- volumeSource : Somme des volumes captés (utilisé pour histo src)
- capacite : Capacité maximale (utilisé pour histo max)
- volumeTraite : Volume réel (utilisé pour histo real)
- ID : Identifiant unique
*/
typedef struct usine {
  char ID[50];
  double volumeSource;
  double capacite;
  double volumeTraite;
} Usine, *pUsine;

/*
Structure de l'arbre AVL, contenant un pointeur vers une usine
*/
typedef struct AVL {
  Usine* u;
  struct AVL* fg;
  struct AVL* fd;
  int equilibre;
} AVL, *pAVL;

// Module avl.c

int max(int a, int b);
int min(int a, int b);
pAVL rotationGauche(pAVL a);
pAVL rotationDroite(pAVL a);
pAVL rotationGaucheDouble(pAVL a);
pAVL rotationDroiteDouble(pAVL a);
pUsine creerUsine();
pAVL creerArbre(Usine u);
pAVL insertionAVL(pAVL a, Usine u, int* h);
pAVL equilibrerAVL(pAVL a);
void libererMemoireAVL(pAVL a);

// Module input.c

void nettoyerLigne(char* chaine);
int estNumerique(char* chaine);
void chargerDonnees(char* cheminFichier, pAVL* a, char* mode);

// Module output.c

void parcoursInfixeInverse(pAVL a, FILE* f, char* mode);
void afficherNoeud(Usine* u, FILE* f, char* mode);
void outputHisto(char* nomFichier, pAVL a, char* mode);
