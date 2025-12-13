#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <math.h>

/*
Structure représentant une usine.
- volumeSource : Somme des volumes captés
- capacite : Capacité maximale
- volumeTraite : Volume réel
- ID : Identifiant unique
*/
typedef struct usine {
  char ID[50];
  double volumeSource;
  double capacite;
  double volumeTraite;
} Usine, *pUsine;

/*
Structure représentant un noeud du réseau (Usine, Stockage, Jonction, etc.).
*/
typedef struct node {
    char ID[50];
    int nbEnfants;
    tuyau* listeEnfants;
    struct node* fg;
    struct node* fd;
    int equilibre;
} Node, *pNode;

/*
Structure représentant un tuyau.
*/
typedef struct tuyau {
    struct pNode destinataire;
    double fuite;
    struct tuyau* pSuivant;
} Tuyau;

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
pAVL rotationGauche(pAVL a);
pAVL rotationDroite(pAVL a);
pAVL rotationGaucheDouble(pAVL a);
pAVL rotationDroiteDouble(pAVL a);
pAVL creerArbre(Usine u);
pAVL insertionAVL(pAVL a, Usine u, int* h);
pAVL equilibrerAVL(pAVL a);
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
