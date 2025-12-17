#include "header.h"
#include "couleurs.h"
#include <ctype.h>
#define BUFFER_SIZE 4096

/*
Supprimer le caractère de saut de ligne final (\n ou \r)
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
Retourne un pointeur vers le noeud dans l'arbre ou le crée s'il n'existe pas.
*/
pUsine trouverOuCreer(pAVL* a, char* ID) {
  pUsine usineExistante = rechercher(*a, ID);
  if (usineExistante != NULL) return usineExistante;
  Usine temp;
  memset(&temp, 0, sizeof(Usine));
  strncpy(temp.ID, ID, 49);
  int h = 0;
  *a = insertionAVL(*a, temp, &h);
  return rechercher(*a, ID);
}

/*
Charger les données pour faire l'histogramme des sources.
*/
void chargerDonnees(char* cheminFichier, pAVL* a, char* commande, char* mode) {
  FILE* fichier = fopen(cheminFichier, "r");
  if (fichier == NULL) {
    fprintf(stderr, ROUGE "Erreur : Impossible d'ouvrir le fichier %s\n" RESET, cheminFichier);
    exit(1);
  }

  char ligne[BUFFER_SIZE];
  char ligne_copie[BUFFER_SIZE];
  
  int histoActive = (strcmp(commande, "histo") == 0);
  int leaksActive = (strcmp(commande, "leaks") == 0);
  
  while (fgets(ligne, BUFFER_SIZE, fichier) != NULL) {
    nettoyerLigne(ligne);
    if (strlen(ligne) == 0) continue;
    strcpy(ligne_copie, ligne);

    // Découpage sécurisé
    char* col1 = strtok(ligne_copie, ";");
    char* col2 = strtok(NULL, ";");
    char* col3 = strtok(NULL, ";");
    char* col4 = strtok(NULL, ";");
    char* col5 = strtok(NULL, ";");

    // Vérification minimale : on a besoin au moins des identifiants
    if (col2 == NULL || col3 == NULL || col4 == NULL) continue;

    // --- MODE HISTOGRAMME ---
    if (histoActive) {
        // Pour l'histo, on a besoin que col4 soit un nombre (Capacité ou Volume)
        if (!estNumerique(col4)) continue;

        // 1. Capacité (Usine seule)
        if (strcmp(col3, "-") == 0 && strcmp(col2, "-") != 0) {
            pUsine u = trouverOuCreer(a, col2);
            u->capacite = atof(col4);
        }
        // 2. Trajet (Source -> Usine)
        else if (strcmp(col3, "-") != 0 && strcmp(col2, "-") != 0) {
            if (strcmp(mode, "src") == 0 || strcmp(mode, "real") == 0) {
                pUsine u = trouverOuCreer(a, col3); 
                double volumeBrut = atof(col4);
                
                if (strcmp(mode, "src") == 0) {
                   u->volumeSource += volumeBrut;
                } else if (strcmp(mode, "real") == 0) {
                   double fuite = 0.0;
                   if (col5 != NULL && estNumerique(col5)) {
                       fuite = atof(col5);
                   }
                   u->volumeTraite += volumeBrut * (1.0 - (fuite / 100.0));
                }
            }
        }
    }
    // --- MODE LEAKS (CORRIGÉ) ---
    else if (leaksActive) {
      // 1. Tuyau (Connexion)
      if (strcmp(col3, "-") != 0) {
        pUsine parent = trouverOuCreer(a, col2);
        pUsine enfant = trouverOuCreer(a, col3);
        double fuite = (col5 && estNumerique(col5)) ? atof(col5) : 0.0;
        
        // Ajout du voisin (Tuyau aval)
        ajouterVoisin(parent, enfant, fuite);

        // NOUVEAU : Si la colonne 4 contient un nombre, c'est une ligne Source -> Usine.
        // On calcule donc le volume réel qui arrive à l'usine (enfant).
        if (estNumerique(col4)) {
            double volSource = atof(col4);
            // On ajoute ce volume à l'usine, moins les fuites du trajet source->usine
            enfant->volumeTraite += volSource * (1.0 - (fuite / 100.0));
        }
      }
      // 2. Capacité de l'usine (Toujours utile au cas où)
      else if (strcmp(col2, "-") != 0 && estNumerique(col4)) {
         pUsine u = trouverOuCreer(a, col2);
         u->capacite = atof(col4);
      }
    }
  }
  fclose(fichier);
}