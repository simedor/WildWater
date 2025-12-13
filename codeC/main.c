# include "header.h"

/*
Fonction principale.
Arguments attendus (définis par le script Shell) :
- argv[0] : Nom de l'exécutable
- argv[1] : Chemin du fichier CSV de données (ex: "data/data.csv")
- argv[2] : Commande principale ("histo" ou "leaks")
- argv[3] : Sous-commande ("src", "max", "real") OU ID Usine (pour leaks)
*/

int main(int argc, char* argv[]) {

  // 1. VERIFICATION DES ARGUMENTS
  
  if (argc != 4) {
    fprintf(stderr, "Erreur : Arguments insuffisants.\n");
    return 1;
  }
  char* cheminFichier = argv[1];
  char* commande = argv[2];
  char* mode = argv[3];

  // 2. VERIFICATION DE LA COMMANDE ET PREPARATION DU FICHIER DE SORTIE
  
  char nomFichierSortie[256];
  int commandeEntree = 0; // 0 = histo, 1 = leaeks

  if (strcmp(commande, "histo") == 0) {
    commandeEntree = 0;
    if (strcmp(mode, "src") == 0) {
      strcpy(nomFichierSortie, "vol_source.csv");
      printf("Mode histo src active\n");
    } else if (strcmp(mode, "max") == 0) {
      strcpy(nomFichierSortie, "vol_max.csv");
      printf("Mode histo max active\n");
    } else if (strcmp(mode, "real") == 0) {
      strcpy(nomFichierSortie, "vol_real.csv");
      printf("Mode histo real active\n");
    } else {
      fprintf(stderr, "Erreur : Mode '%s' inconnu.\n", mode);
      return 1;
    }
  } else if (strcmp(commande, "leaks") == 0) {
    commandeEntree = 1;
    strcpy(nomFichierSortie, "leaks.csv");
    printf("Mode leaks active\n");
  } else {
    fprintf(stderr, "Erreur : Commande '%s' inconnue.\n", commande);
    return 1;
  }
  
  // 3. CHARGEMENT DES DONNEES

  pAVL a = NULL;
  printf("Lecture du fichier %s en cours...\n", cheminFichier);
  chargerDonnees(cheminFichier, &a, mode);
  printf("Donnees chargees avec succes.\n");
  if (a == NULL) {
        fprintf(stderr, "L'arbre est vide apres chargement. Verifiez le fichier d'entree.\n");
  }

  // 4. GENERATION DU FICHIER CSV

  if (commandeEntree == 0) {
    outputHisto(nomFichierSortie, a, mode);
  } else {
    outputLeaks(nomFichierSortie, a);
  }
  printf("Succes ! Fichier genere : %s\n", nomFichierSortie);
  
  // 5. NETTOYAGE MÉMOIRE
  
  libererMemoireAVL(a);
  
  return 0;
}
