#include "header.h"
#include "couleurs.h" // Ajout de l'inclusion des couleurs

/*
Écrit les données d'une usine dans le fichier selon le mode choisis (src, max, real).
Utilisée par outputHisto.
*/
void afficherNoeud(Usine* u, FILE* fichier, char* mode) {
    if (u == NULL || fichier == NULL) exit(1);
    double valeur = 0;
    double valeur2 = 0;
    if (strcmp(mode, "src") == 0) {
        valeur2 = u->volumeSource;
        valeur = u->capacite;
    } else if (strcmp(mode, "max") == 0) {
        valeur = u->capacite;
    } else if (strcmp(mode, "real") == 0) {
        valeur2 = u->volumeTraite;
        valeur = u->capacite;
    } else {
        //msg d'erreur au cas où
        fprintf(stderr, ROUGE "Erreur interne: Mode '%s' non reconnu par afficherNoeud.\n" RESET, mode);
        return; 
    }
    
    // On n'affiche que si la valeur est supérieure à zéro (pour la clarté de l'histogramme)
    if (valeur > 0) {
        fprintf(fichier, "%s;%.3f;%.3f\n", u->ID, valeur / 1000.0, valeur2 / 1000.0);
    }
}

/*
Ecrit les données dans l'ordre alphabétique inverse des identifiants.
*/
void parcoursInfixeInverse(pAVL a, FILE* fichier, char* mode) {
  if (a != NULL) {
    parcoursInfixeInverse(a->fd, fichier, mode);
    afficherNoeud(a->u, fichier, mode);
    parcoursInfixeInverse(a->fg, fichier, mode);
  }
}
/* Générer le fichier CSV pour faire l'histogramme
*/
void outputHisto(char* nomFichier, pAVL a, char* mode) {
    FILE* fichier = fopen(nomFichier, "w");
    if (fichier == NULL) {
        fprintf(stderr, ROUGE "Erreur : Impossible de créer le fichier %s\n" RESET, nomFichier);
        exit(1);
    }

    // Ecriture de l'en-tête CSV (Headers)
    if (strcmp(mode, "src") == 0) {
        fprintf(fichier, "identifier;volume source(M.m3.year-1);capacite(M.m3.year-1)\n");
    } else if (strcmp(mode, "real") == 0) {
        fprintf(fichier, "identifier;volume traite(M.m3.year-1);capacite(M.m3.year-1)\n");
    } else if (strcmp(mode, "max") == 0) {
        fprintf(fichier, "identifier;capacite(M.m3.year-1)\n");
    } else {
        fprintf(stderr, ROUGE "Erreur : Mode '%s' non reconnu.\n" RESET, mode);
        fclose(fichier);
        exit(1);
    }

    // Ecriture des données
    parcoursInfixeInverse(a, fichier, mode);
    
    fclose(fichier);
    printf(VERT "Fichier %s généré.\n" RESET, nomFichier);
}

double sommePertes(pUsine u, double eauArrivante) {
    if (u == NULL || u->nbEnfants == 0) {
        return 0.0;
    }

    // L'eau est répartie équitablement entre les enfants
    double eauParEnfant = eauArrivante / u->nbEnfants;
    double totalPertes = 0.0;

    // Parcours de la liste chaînée des voisins (les tuyaux partants)
    pTuyau t = u->listeEnfants;
    while (t != NULL) {
        // 1. Calcul de la fuite sur CE tronçon
        // (pourcentageFuite est stocké tel quel, ex: 2.5 pour 2.5%)
        double perteTuyau = eauParEnfant * (t->pourcentageFuite / 100.0);
        
        // 2. Calcul de l'eau restante qui arrive au noeud suivant
        double eauRestante = eauParEnfant - perteTuyau;
        
        // 3. Appel récursif : On ajoute la perte de ce tuyau + les pertes générées en aval
        totalPertes = totalPertes + perteTuyau + sommePertes(t->noeud, eauRestante);
        
        t = t->pSuivant;
    }
    
    return totalPertes;
}

/* 
Générer le fichier CSV pour leaks
*/
void outputLeaks(char* nomFichier, pAVL a, char* ID) {
    FILE* fichier = fopen(nomFichier, "w");
    if (fichier == NULL) {
        perror("Erreur creation fichier leaks");
        exit(1);
    }

    // Ecriture de l'en-tête
    fprintf(fichier, "identifier;Leak volume(M.m3.year-1)\n");

    pUsine depart = rechercher(a, ID);

    if (depart == NULL) {
        fprintf(fichier, "%s;-1\n", ID); 
        printf("Usine '%s' introuvable.\n", ID);
    } else {
        double volumeInitial = depart->capacite;
        double totalFuites = sommePertes(depart, volumeInitial);
        fprintf(fichier, "%s;%.3f\n", depart->ID, totalFuites / 1000.0);
    }

    fclose(fichier);
}
