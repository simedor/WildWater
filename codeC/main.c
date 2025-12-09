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

  // 1. VÉRIFICATION DES ARGUMENTS
  if (argc != 4) {
    fprintf(stderr, "Erreur : Arguments insuffisants.\n");
    fprintf(stderr, "Usage : %s <fichier.csv> <histo|leaks> <mode|id_usine>\n", argv[0]);
    return 1;
  }
  char* cheminFichier = argv[1];
  char* commande = argv[2];
  char* mode = argv[3];
  if (strcmp(commande, "histo") != 0) {
    fprintf(stderr, "Erreur : Commande '%s' non gérée par ce programme.\n", commande);
    fprintf(stderr, "Seule la commande 'histo' est disponible.\n");
    return 1;
  }

  // 2. INITIALISATION
  pAVL a = NULL;
  char nomFichierSortie[256];

  // 3. TRAITEMENT
  if (strcmp(mode, "src") == 0) {
    printf("Lecture du fichier %s en cours...\n", cheminFichier);
    chargerDonnees(cheminFichier, &a);
    strcpy(nomFichierSortie, "vol_source.csv");
    outputHistoSource(nomFichierSortie, a);
    printf("Succès ! Fichier généré : %s\n", nomFichierSortie);
  }
  
  // 4. NETTOYAGE MÉMOIRE
  libererMemoireAVL(a);
  
  return 0;
}
