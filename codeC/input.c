#include "header.h"
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
  // On vérifie le premier caractère (chiffre ou signe négatif)
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
    fprintf(stderr, "Erreur : Impossible d'ouvrir le fichier %s\n", cheminFichier);
    exit(1);
  }

  printf("Le fichier a ete ouvert sans erreur\n");
  
  char ligne[BUFFER_SIZE];
  char ligne_copie[BUFFER_SIZE];
  
  int histoActive = (strcmp(commande, "histo") == 0);
  int leaksActive = (strcmp(commande, "leaks") == 0);
  
  while (fgets(ligne, BUFFER_SIZE, fichier) != NULL) {
    nettoyerLigne(ligne);
    if (strlen(ligne) == 0) continue;
    strcpy(ligne_copie, ligne);
    
    // On récupère les données des différentes colonnes
    char* col1 = strtok(ligne_copie, ";");
    char* col2 = strtok(NULL, ";");           // Amont (Source, Usine, ou Stockage)
    char* col3 = strtok(NULL, ";");           // Aval (Usine, Stockage, ou Jonction)
    char* col4 = strtok(NULL, ";");           // Volume / Capacité
    char* col5 = strtok(NULL, ";");           // Fuite

    // On vérifie si la ligne est valide
    if (col1 == NULL || col2 == NULL || col3 == NULL || col4 == NULL) continue;
    if (!estNumerique(col4)) continue;

    // On initialise les variables pour stocker les données de la ligne 
    char* idUsine = NULL;
    double capacite = 0;
    double volume = 0;
    int lignePertinente = 0;
    
    // ====================================================
    // LOGIQUE DE SELECTION DES LIGNES SELON LE MODE
    // La fonction atof convertit une chaîne de caractère en double.
    // La fonction memset initialise les valeurs à 0.
    // ====================================================

    if (histoActive) {
      // CAS 1 : Mode MAX
      // On cherche les lignes qui déninissent une usine
      // Format: "-; ID_Usine; -; Capacité; -"
      if (strcmp(mode, "max") == 0) {
        if (strcmp(col3, "-") == 0 && strcmp(col2, "-") != 0) {
          idUsine = col2;
          capacite = atof(col4);
          lignePertinente = 1;
        }
      } 
      
      // CAS 2 : Mode SRC ou REAL
      // On cherche les lignes de trajet : SOURCE -> USINE
      // Format : "- ; ID_SOURCE ; ID_USINE ; VOLUME ; FUITE"
      else if (strcmp(mode, "src") == 0 || strcmp(mode, "real") == 0) {
        if (strcmp(col3, "-") != 0 && strcmp(col2, "-") != 0) {
          idUsine = col3;
          double volumeBrut = atof(col4);
          
          // On vérifie le mode (src ou real) pour savoir si on prend le volume brut ou si on prend en compte les fuites
          if (strcmp(mode, "src") == 0) {
            volume = volumeBrut;
          } else {
            double fuite = 0.0;
            if (col5 != NULL && estNumerique(col5)) {
              fuite = atof(col5);
            }
            volume = volumeBrut * (1.0 - (fuite / 100.0));
          }
          lignePertinente = 1;
        }
      }
    } 
      
    // CAS 3 : Mode LEAKS
    // ****** A FAIRE, VERSION SIMPLIFIEE ******
    else if (leaksActive) {
      if ((strcmp(col1, mode) == 0) || (strcmp(col2, mode) == 0)) {
        pUsine parent = trouverOuCreer(a, col2);

        if (strcmp(col3, "-") == 0) {
          // C'est une définition de capacité (Pas de voisin)
          parent->capacite = atof(col4);
        } else {
          // C'est un tuyau vers un Enfant (Aval)
          pUsine enfant = trouverOuCreer(a, col3);
                    
          // Création du lien
          double fuite = (col5 && estNumerique(col5)) ? atof(col5) : 0.0;
          ajouterVoisin(parent, enfant, fuite);
        }
      }
    }
    
    // ====================================================
    // INSERTION DANS L'AVL
    // ====================================================
    
    if (lignePertinente == 1 && idUsine != NULL) {
      // Initialisation de la structure usine à insérer
      Usine u_temp;
      memset(&u_temp, 0, sizeof(Usine));

      // Remplissage de la structure
      strncpy(u_temp.ID, idUsine, 49);
      u_temp.ID[49] = '\0';
      if (histoActive) {
        if (strcmp(mode, "max") == 0) {
          u_temp.capacite = capacite;
        } else if (strcmp(mode, "src") == 0) {
          u_temp.volumeSource = volume;
        } else if (strcmp(mode, "real") == 0) {
          u_temp.volumeTraite = volume;
        } 
      }

      // Insertion dans l'AVL
      int h = 0;
      *a = insertionAVL(*a, u_temp, &h);
    }
  }
  
  fclose(fichier);
}
