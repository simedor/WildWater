#include "header.h"
#include "couleurs.h" // Ajout

// --- Affichage pour HISTO ---
void parcoursInfixeHisto(pAVL a, FILE* fichier, char* mode) {
    // ... [Code inchangé] ...
    if (a != NULL) {
        parcoursInfixeHisto(a->fg, fichier, mode);
        
        double valeur = 0;
        int condition = 0;

        if (strcmp(mode, "src") == 0) {
            valeur = a->u->volumeSource;
            if (valeur > 0) condition = 1; // On ignore les stations qui ne produisent rien
        } else if (strcmp(mode, "max") == 0) {
            valeur = a->u->capacite;
            if (valeur > 0) condition = 1;
        } else if (strcmp(mode, "real") == 0) {
            valeur = a->u->volumeTraite;
            if (valeur > 0) condition = 1;
        }

        if (condition) {
            // [SUJET] Format : Identifier ; Value
            // Attention aux unités, si besoin diviser par 1000 selon ton choix précédent
            // Ici je garde brut ou /1000 selon ta logique précédente (vol_source était ok avec /1000)
            fprintf(fichier, "%s;%.3f\n", a->u->ID, valeur / 1000.0);
        }

        parcoursInfixeHisto(a->fd, fichier, mode);
    }
}

// --- Affichage pour LEAKS ---
void parcoursInfixeLeaks(pAVL a, FILE* fichier) {
    // ... [Code inchangé] ...
    if (a != NULL) {
        parcoursInfixeLeaks(a->fg, fichier);
        
        // [SUJET] LEAKS : Identifier les stations où Volume > Capacité
        // Il faut que la capacité soit définie (>0) et que le volume entrant soit supérieur.
        if (a->u->capacite > 0 && a->u->volumeTraite > a->u->capacite) {
            double diff = a->u->volumeTraite - a->u->capacite;
            // Format : Station;Capacité;Volume;Surplus (en M.m3 si division par 1000)
            fprintf(fichier, "%s;%.3f;%.3f;%.3f\n", 
                    a->u->ID, 
                    a->u->capacite / 1000.0, 
                    a->u->volumeTraite / 1000.0, 
                    diff / 1000.0);
        }
        
        parcoursInfixeLeaks(a->fd, fichier);
    }
}

void outputHisto(char* nomFichier, pAVL a, char* mode) {
    FILE* fichier = fopen(nomFichier, "w");
    if (fichier == NULL) exit(1);

    // En-tête CSV
    if (strcmp(mode, "src") == 0) fprintf(fichier, "Station;Volume Source (M.m3)\n");
    else if (strcmp(mode, "max") == 0) fprintf(fichier, "Station;Capacite (M.m3)\n");
    else fprintf(fichier, "Station;Volume Traite (M.m3)\n");

    parcoursInfixeHisto(a, fichier, mode);
    fclose(fichier);
}

void outputLeaks(char* nomFichier, pAVL a) {
    FILE* fichier = fopen(nomFichier, "w");
    if (fichier == NULL) exit(1);
    
    // En-tête CSV Leaks
    fprintf(fichier, "Station;Capacite;Volume;Surplus\n");
    
    parcoursInfixeLeaks(a, fichier);
    
    fclose(fichier);
    printf(VERT "Fichier %s généré.\n" RESET, nomFichier);
}