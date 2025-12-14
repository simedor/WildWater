#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <math.h>

/*
Structure représentant un tuyau.
*/
typedef struct tuyau {
    struct usine* noeud;
    double pourcentageFuite;
    struct tuyau* pSuivant;
} Tuyau, *pTuyau;

/*
Structure représentant une usine.
- volumeSource : Somme des volumes captés
- capacite : Capacité maximale
- volumeTraite : Volume réel
- ID : Identifiant unique
*/
typedef struct usine {
  char ID[50];

  // pour histo
  double volumeSource;
  double capacite;
  double volumeTraite;

  // pour leaks
  int nbEnfants;
  pTuyau listeEnfants;

} Usine, *pUsine;

/*
Structure de l'arbre AVL, contenant un pointeur vers une usine.
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
/*pAVL rotationGauche(pAVL a);
pAVL rotationDroite(pAVL a);
pAVL equilibrerAVL(pAVL a);
pAVL rotationGaucheDouble(pAVL a);
pAVL rotationDroiteDouble(pAVL a);*/
pUsine rechercher(pAVL a, char* ID);
void ajouterVoisin(pUsine parent, pUsine enfant, double fuite);
pAVL creerArbre(Usine u);
pAVL insertionAVL(pAVL a, Usine u, int* h);
void libererMemoireAVL(pAVL a);

// Module input.c

void nettoyerLigne(char* chaine);
int estNumerique(char* chaine);
void chargerDonnees(char* cheminFichier, pAVL* a, char* commande, char* mode);

// Module output.c

void afficherNoeud(Usine* u, FILE* f, char* mode);
void afficherNoeudLeaks(Usine* u, FILE* f);
void parcoursInfixeInverse(pAVL a, FILE* f, char* mode);
void parcoursInfixe(pAVL a, FILE* f);
void outputHisto(char* nomFichier, pAVL a, char* mode);
void outputLeaks(char* nomFichier, pAVL a);
