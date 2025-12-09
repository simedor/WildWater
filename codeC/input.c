#include "header.h"
#include <ctype.h>
#define BUFFER_SIZE 4096

/*
Supprimer le caractère de saut de ligne final
*/
void nettoyerLigne(char* chaine) {
  char* p = strchr(chaine, '\n');
  if (p != NULL) *p = '\0';
  p = strchr(chaine, '\r');
  if (p != NULL) *p = '\0';
}

/*
Vérifier si une chaîne est un nombre valide
*/
int estNumerique(char* chaine) {
  if (chaine == NULL || strlen(chaine) == 0) return 0;
  if (strcmp(chaine, "-") == 0) return 0;
  return (isdigit(chaine[0]) || chaine[0] == '-');
}

/*
Charger les données pour faire l'histogramme des sources
*/
void chargerDonnees(char* cheminFichier, pAVL* a) {
  FILE* fichier = fopen(cheminFichier, "r");
  if (fichier == NULL) {
    fprintf(stderr, "Erreur : Impossible d'ouvrir le fichier %s\n", cheminFichier);
    exit(1);
  }

  printf("Le fichier a ete ouvert sans erreur");
  
  char ligne[BUFFER_SIZE];
  char ligne_copie[BUFFER_SIZE];
  while (fgets(ligne, BUFFER_SIZE, fichier) != NULL) {
    nettoyerLigne(ligne);
    if (strlen(ligne) == 0) continue;
    strcpy(ligne_copie, ligne);

    char* col1 = strtok(ligne_copie, ";");
    char* col2 = strtok(NULL, ";");
    char* col3 = strtok(NULL, ";");
    char* col4 = strtok(NULL, ";");

    if (col1 == NULL || col2 == NULL || col3 == NULL || col4 == NULL) continue;
    if (strcmp(col1, "-") != 0 && strlen(col1) > 0) continue;
    if (strcmp(col3, "-") == 0) continue;
    if (!estNumerique(col4)) continue;

    Usine u_temp;
    strncpy(u_temp.ID, col3, 49);
    u_temp.ID[49] = '\0';
    u_temp.volumeSource = atof(col4);
    
    *a = insertionAVL(*a, u_temp, &((*a)->equilibre));
  }
  printf("Les donnees ont ete rentres dans l'AVL");
  fclose(fichier);
}
