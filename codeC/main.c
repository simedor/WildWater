#include "header.h"
#include "couleurs.h" 

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
    fprintf(stderr, ROUGE "Erreur : Arguments insuffisants.\n" RESET);
    return 1;
  }
  char* cheminFichier = argv[1];
  char* commande = argv[2];
  char* mode = argv[3];

  // 2. VERIFICATION DE LA COMMANDE ET PREPARATION DU FICHIER DE SORTIE
  
  char nomFichierSortie[256];
  int commandeEntree = 0; // 0 = histo, 1 = leaks

  if (strcmp(commande, "histo") == 0) {
    commandeEntree = 0;
    if (strcmp(mode, "src") == 0) {
      strcpy(nomFichierSortie, "vol_source.csv");
      printf(CYAN "Mode histo src activ\n" RESET);
    } else if (strcmp(mode, "max") == 0) {
      strcpy(nomFichierSortie, "vol_max.csv");
      printf(CYAN "Mode histo max activé\n" RESET);
    } else if (strcmp(mode, "real") == 0) {
      strcpy(nomFichierSortie, "vol_real.csv");
      printf(CYAN "Mode histo real activé\n" RESET);
    } else {
      fprintf(stderr, ROUGE "Erreur : Mode '%s' inconnu.\n" RESET, mode);
      return 1;
    }
  } else if (strcmp(commande, "leaks") == 0) {
    commandeEntree = 1;
    strcpy(nomFichierSortie, "leaks.csv");
    printf(CYAN "Mode leaks activé\n" RESET);
  } else {
    fprintf(stderr, ROUGE "Erreur : Commande '%s' inconnue.\n" RESET, commande);
    return 1;
  }
  
  // 3. CHARGEMENT DES DONNEES

  pAVL a = NULL;
  printf(JAUNE "Lecture du fichier %s en cours...\n" RESET, cheminFichier);
  chargerDonnees(cheminFichier, &a, commande, mode);
  printf(VERT "Donnees chargées avec succès.\n" RESET);
  if (a == NULL) fprintf(stderr, MAGENTA "L'arbre est vide après chargement. Vérifiez le fichier d'entrée.\n" RESET);

  // 4. GENERATION DU FICHIER CSV

  if (commandeEntree == 0) {
    outputHisto(nomFichierSortie, a, mode);
  } else {
    outputLeaks(nomFichierSortie, a, mode);
  }
  printf(VERT_CLAIR "Succès ! Fichier généré : %s\n" RESET, nomFichierSortie);
  
  // 5. NETTOYAGE MÉMOIRE
  
  libererMemoireAVL(a);
  printf("Mémoire AVL libérée.\n");
  
  return 0;
}
