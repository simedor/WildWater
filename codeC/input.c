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
  
void chargerDonnees(char* cheminFichier, pAVL* a, char* commande, char* mode) {
    FILE* fichier = fopen(cheminFichier, "r");
    if (fichier == NULL) {
        perror("Erreur ouverture fichier");
        exit(1);
    }

    char ligne[BUFFER_SIZE];
    int histoActive = (strcmp(commande, "histo") == 0);
    int leaksActive = (strcmp(commande, "leaks") == 0);
    int capaciteTrouvee = 0;

    while (fgets(ligne, BUFFER_SIZE, fichier) != NULL) {
        nettoyerLigne(ligne);
        if (strlen(ligne) == 0) continue;

        // Découpage manuel (robuste pour les champs vides)
        char* cols[5] = {NULL, NULL, NULL, NULL, NULL};
        char* ptr = ligne;
        int i = 0;
        cols[i++] = ptr;
        while (*ptr && i < 5) {
            if (*ptr == ';') {
                *ptr = '\0';
                cols[i++] = ptr + 1;
            }
            ptr++;
        }

        // Sécurité : on ignore les lignes trop courtes
        if (i < 4) continue; 

        char* col1 = cols[0];
        char* col2 = cols[1];
        char* col3 = cols[2];
        char* col4 = cols[3];
        char* col5 = (i == 5) ? cols[4] : "0";

        // --- CORRECTION MAJEURE ICI ---
        // On ne saute la ligne QUE si on est en mode Histo (car on a besoin des volumes)
        // En mode Leaks, col4 peut être "-" pour les tuyaux, c'est normal.
        if (histoActive && !estNumerique(col4)) continue;

        double val = estNumerique(col4) ? atof(col4) : 0.0;
        double fuite = (estNumerique(col5)) ? atof(col5) : 0.0;

        // --- 1. MODE HISTOGRAMME (Inchangé) ---
        if (histoActive) {
            char* idUsine = NULL;
            // Mode MAX
            if (strcmp(mode, "max") == 0) {
                // Logique Cas A (ID en col1) ou Cas B (ID en col2) pour la capacité
                if (i>=3 && strcmp(col2, "-")==0 && strcmp(col1, "-")!=0 && estNumerique(col3)) { 
                    // Cas spécial : ID; -; Capacité
                     idUsine = col1; val = atof(col3); // attention décalage indices si format spécial
                }
                else if (strcmp(col3, "-") == 0 || strlen(col3) == 0) {
                    if (strcmp(col2, "-") != 0) idUsine = col2;
                }
                
                if (idUsine) {
                    Usine u; memset(&u, 0, sizeof(Usine));
                    strncpy(u.ID, idUsine, 49);
                    u.capacite = val;
                    int h=0; *a = insertionAVL(*a, u, &h);
                }
            }
            // Mode SRC / REAL
            else {
                if (strcmp(col2, "-") != 0 && strcmp(col3, "-") != 0) {
                    idUsine = col3;
                    Usine u; memset(&u, 0, sizeof(Usine));
                    strncpy(u.ID, idUsine, 49);
                    if (strcmp(mode, "src") == 0) u.volumeSource = val;
                    else if (strcmp(mode, "real") == 0) u.volumeTraite = val * (1.0 - (fuite/100.0));
                    int h=0; *a = insertionAVL(*a, u, &h);
                }
            }
        }

        // --- 2. MODE LEAKS (Corrigé) ---
        else if (leaksActive) {
            // A. RECHERCHE DE LA CAPACITÉ INITIALE (Ligne de définition)
            // On cherche l'ID exact soit en Col 1 (Cas spécial) soit en Col 2 (Standard)
            int estDefCapacite = 0;
            
            // Cas spécial : Plant #ID;-;Capacité;-
            if (strcmp(col1, mode) == 0 && strcmp(col2, "-") == 0 && estNumerique(col3)) {
                pUsine u = trouverOuCreer(a, col1);
                u->capacite = atof(col3);
                capaciteTrouvee = 1;
                estDefCapacite = 1;
            }
            // Cas Standard : -;Plant #ID;-;Capacité
            else if (strcmp(col2, mode) == 0 && strcmp(col3, "-") == 0 && estNumerique(col4)) {
                pUsine u = trouverOuCreer(a, col2);
                u->capacite = val;
                capaciteTrouvee = 1;
                estDefCapacite = 1;
            }

            // B. CHARGEMENT DES TUYAUX
            // Si ce n'est pas une ligne de capacité, c'est un tuyau.
            // IMPORTANT : On charge TOUS les tuyaux valides (Col2->Col3) sans filtrer par ID usine ici.
            // Pourquoi ? Car filtrer par col1 est risqué si le fichier est mal rempli.
            // L'AVL va trier tout ça, et le parcours récursif ne suivra que les bons chemins.
            if (!estDefCapacite && strcmp(col2, "-") != 0 && strcmp(col3, "-") != 0) {
                 pUsine parent = trouverOuCreer(a, col2);
                 pUsine enfant = trouverOuCreer(a, col3);
                 ajouterVoisin(parent, enfant, fuite);
            }
        }
    }
    
    fclose(fichier);
}
