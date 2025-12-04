#include "header"
#define BUFFER_SIZE 4096

void nettoyer_ligne(char* chaine) {
  char* p = strchr(chaine, '\n');
  if (p) *p = '\0';
  p = strchr(chaine, '\r');
  if (p) *p = '\0';
}

void chargerDonnees(char* cheminFichier, Arbre** racine) {
  FILE* fichier = fopen(cheminFichier, "r");
    
    // Vérification ouverture fichier (Critère robustesse : code retour positif en cas d'erreur)
  if (fichier == NULL) {
    fprintf(stderr, "Erreur : Impossible d'ouvrir le fichier %s\n", cheminFichier);
    exit(2);
  }
  char ligne[BUFFER_SIZE];
  char ligne_copie[BUFFER_SIZE]; // Copie car strtok modifie la chaîne
  int numero_ligne = 0;

    // Lecture ligne par ligne
  while (fgets(ligne, BUFFER_SIZE, fichier) != NULL) {
    numero_ligne++;
    nettoyer_ligne(ligne);
    if (strlen(ligne) == 0) continue;
      strcpy(ligne_copie, ligne);

    // -- COLONNE 1 --
    char* col1 = strtok(ligne_copie, ";");
    if (col1 == NULL) continue; // Ligne vide ou malformée

    // Si la colonne 1 n'est pas "-", ce n'est pas une définition d'usine
    // (Cela permet d'ignorer toutes les lignes de trajets Source->Usine ou Usine->Stockage)
    if (strcmp(col1, "-") != 0 && strlen(col1) > 0) {
      continue; 
    }

    // -- COLONNE 2 (Identifiant) --
    char* id_usine = strtok(NULL, ";");
    if (id_usine == NULL || strcmp(id_usine, "-") == 0) continue;

    // -- COLONNE 3 --
    char* col3 = strtok(NULL, ";");
    // Si col3 existe et n'est pas "-", on ignore (car une usine a "-" en col 3)
    if (col3 != NULL && strcmp(col3, "-") != 0 && strlen(col3) > 0) {
      continue;
    }

    // -- COLONNE 4 (Capacité) --
    char* cap_str = strtok(NULL, ";");
    if (cap_str == NULL || strcmp(cap_str, "-") == 0) continue;

    // 3. Conversion et Insertion
    // À ce stade, on est sûr d'avoir une ligne "Usine" valide
        
    long capacite = atol(cap_str); // Conversion string -> long
  Usine u;
  // Copie sécurisée de l'ID (max 50 chars pour éviter débordement)
  strncpy(u.id, id_usine, 49);
  u.id[49] = '\0'; 
  u.capacite = capacite;
        
  // Initialisation des autres valeurs à 0 (pour plus tard)
  u.vol_source = 0; insertion(*racine, u);
  u.vol_traite = 0;

  // Insertion dans l'AVL
  // Note : insertion renvoie la nouvelle racine potentielle après équilibrage
  *racine = insertion(*racine, u);
  }

    fclose(fichier);


  }
