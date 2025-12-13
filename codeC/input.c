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
  // On vérifie le premier caractère (chiffre ou signe négatif)
  return (isdigit(chaine[0]) || chaine[0] == '-');
}

/*
Charger les données pour faire l'histogramme des sources
*/
void chargerDonnees(char* cheminFichier, pAVL* a, char* mode) {
  FILE* fichier = fopen(cheminFichier, "r");
  if (fichier == NULL) {
    fprintf(stderr, "Erreur : Impossible d'ouvrir le fichier %s\n", cheminFichier);
    exit(1);
  }

  printf("Le fichier a ete ouvert sans erreur\n");
  
  char ligne[BUFFER_SIZE];
  char ligne_copie[BUFFER_SIZE];

  // Détection du mode de fonctionnement (0 = Leaks , 1 = Histo)
  int estModeHisto = (strcmp(mode, "src") == 0 || strcmp(mode, "max") == 0 || strcmp(mode, "real") == 0);
  
  while (fgets(ligne, BUFFER_SIZE, fichier) != NULL) {
    nettoyerLigne(ligne);
    if (strlen(ligne) == 0) continue;
    strcpy(ligne_copie, ligne);
    
    // On récupère les données des différentes colonnes
    char* col1 = strtok(ligne_copie, ";");
    char* col2 = strtok(NULL, ";");
    char* col3 = strtok(NULL, ";");
    char* col4 = strtok(NULL, ";");
    char* col5 = strtok(NULL, ";");

    // On filtre les lignes 
    if (col1 == NULL || col2 == NULL || col3 == NULL || col4 == NULL) continue;
    if (strcmp(col1, "-") != 0 && strlen(col1) > 0) continue;
    if (!estNumerique(col4)) continue;
    // Si l'ID de la ligne ne correspond pas à l'argument on ignore
    if (estModeHisto == 0 && strcmp(col2, mode) != 0) continue;

    pUsine u_temp = creerUsine();
    strncpy(u_temp->ID, col2, 49);
    
    int ligneValide = 0;

    // La fonction atof convertit une chaîne de caractère en double
    // CAS 1 : Mode MAX (Capacité uniquement)
    if (strcmp(mode, "max") == 0) {
      if (strcmp(col3, "-") == 0) {
        u_temp->capacite = atof(col4);
        ligneValide = 1;
      }
    } 
    // CAS 2 : Mode SRC ou REAL (Consommation uniquement)
    else if (strcmp(mode, "src") == 0 || strcmp(mode, "real") == 0) {
      if (strcmp(col3, "-") != 0) {
        double volumeBrut = atof(col4);
        if (strcmp(mode, "src") == 0) {
          u_temp->volumeSource = volumeBrut;
        } else {
          double fuite = 0.0;
          if (col5 != NULL && estNumerique(col5)) {
            fuite = atof(col5);
          }
          u_temp->volumeTraite = volumeBrut * (1.0 - (fuite / 100.0));
        }
        ligneValide = 1;
      }
    } 
    // CAS 3 : Mode LEAKS (Capacité et consommation)
    else {
      if (strcmp(col3, "-") == 0) {
        // C'est une ligne de CAPACITÉ
        u_temp->capacite = atof(col4);
        ligneValide = 1;
      } else {
        // C'est une ligne de CONSOMMATION
        double volumeBrut = atof(col4);
        double fuite = 0.0;
        if (col5 != NULL && estNumerique(col5)) {
          fuite = atof(col5);
        }
        u_temp->volumeTraite = volumeBrut * (1.0 - (fuite / 100.0));
        ligneValide = 1;
      }
    }

    if (ligneValide == 1) {
      int h = 0;
      *a = insertionAVL(*a, *u_temp, &h);
    }
  }
  
  fclose(fichier);
}
