//Fichier contenant les fonctions de gestion de l'arbre AVL

#include "header.h"

int max(int a, int b) { return a > b ? a : b; }

int min(int a, int b) { return a < b ? a : b; }

pAVL creerArbre(Usine u) { 
	// Allocation mémoire de l'arbre et de la structure Usine contenue
	pAVL nouveau = malloc(sizeof(AVL));
	if (nouveau == NULL) exit(1);
	nouveau->u = malloc(sizeof(Usine));
	if (nouveau->u == NULL) exit(1);

	// Remplissage des variables
  	strncpy(nouveau->u->ID, u.ID, 49);
	nouveau->u->ID[49] = '\0';
  	nouveau->u->volumeSource = u.volumeSource;
	nouveau->u->volumeTraite = u.volumeTraite;
	nouveau->u->capacite = u.capacite;
	nouveau->fg = nouveau->fd = NULL;
	nouveau->equilibre = 0;
	nouveau->u->listeEnfants = NULL;
    nouveau->u->nbEnfants = 0;
	
	return nouveau;
}


pAVL rotationGauche(pAVL a) {
    int eq_a = 0;
    int eq_p = 0;
    pAVL pivot = a->fd;
    a->fd = pivot->fg;
    pivot->fg = a;
    eq_a = a->equilibre;
    eq_p = pivot->equilibre;
    a->equilibre = eq_a - max(eq_p, 0) - 1;
    pivot->equilibre = min(eq_a - 2, min(eq_a + eq_p - 2, eq_p - 1));
    a = pivot;
    return a;
}

pAVL rotationDroite(pAVL a) {
    int eq_a = 0;
    int eq_p = 0;
    pAVL pivot = a->fg;
    a->fg = pivot->fd;
    pivot->fd = a;
    eq_a = a->equilibre;
    eq_p = pivot->equilibre;
    a->equilibre = eq_a - min(eq_p, 0) + 1;
    pivot->equilibre = max(eq_a + 2, max(eq_a + eq_p + 2, eq_p + 1));
    a = pivot;
    return a;
}

pAVL rotationGaucheDouble(pAVL a) {
    a->fd = rotationDroite(a->fd);
    return rotationGauche(a);
}

pAVL rotationDroiteDouble(pAVL a){
    a->fg = rotationGauche(a->fg);
    return rotationDroite(a);
}

pAVL equilibrerAVL(pAVL a) {
	if (a != NULL) {
		if (a->equilibre <= -2) {
			if (a->fg->equilibre <= 0) return rotationDroite(a);
			else return rotationDroiteDouble(a);
		} else if (a->equilibre >= 2) {
			if (a->fd->equilibre >= 0) return rotationGauche(a);
			else return rotationGaucheDouble(a);
		}
	}
	return a;
}

pAVL insertionAVL(pAVL a, Usine u, int* h) { 
	if (a == NULL) {
		*h = 1;
		return creerArbre(u);
	}
	int cmp = strcmp(u.ID, a->u->ID);
	if (cmp < 0) {
		a->fg = insertionAVL(a->fg, u, h);
		*h = -*h;
	} else if (cmp > 0) {
		a->fd = insertionAVL(a->fd, u, h);
	} else {
		a->u->volumeSource += u.volumeSource;
		a->u->volumeTraite += u.volumeTraite;
		if (u.capacite > 0) a->u->capacite = u.capacite;
		*h = 0;
		return a;
	}
	if (*h != 0) {
		a->equilibre += *h;
		a = equilibrerAVL(a);
		if (a->equilibre == 0) *h = 0;
		else *h = 1;
	}
	return a;
}

void libererMemoireAVL(pAVL a) {
    if (a != NULL) {
        libererMemoireAVL(a->fg);
        libererMemoireAVL(a->fd);
		if (a->u != NULL) {
			pTuyau enfant = a->u->listeEnfants;
            while (enfant != NULL) {
            	pTuyau temp = enfant;
                enfant = enfant->pSuivant;
                free(temp);
            }
			free(a->u);
		}
        free(a);
    }
}

/*
Retourne un pointeur vers l'Usine si elle est dans l'AVL, sinon NULL
*/
Usine* rechercher(pAVL a, char* ID) {
    if (a == NULL) return NULL;
    int cmp = strcmp(ID, a->u->ID);
    if (cmp < 0) return rechercher(a->fg, ID);
    if (cmp > 0) return rechercher(a->fd, ID);
	return a->u;
}

/*
Ajoute enfant dans la liste des enfants de parent.
*/
/* modification si jamais l'enfant est déjà là avant de l'ajouter*/
void ajouterVoisin(pUsine parent, pUsine enfant, double fuite) {
    // Vérification anti-doublon (ROBUSTESSE)
    pTuyau temp = parent->listeEnfants;
    while (temp != NULL) {
        if (temp->noeud == enfant) return; // Le lien existe déjà, on ne fait rien !
        temp = temp->pSuivant;
    }

    // Si on arrive ici, c'est que le lien n'existe pas encore
    pTuyau nouveauTuyau = malloc(sizeof(Tuyau));
    if (nouveauTuyau == NULL) exit(1);
    
    nouveauTuyau->noeud = enfant;
    nouveauTuyau->pourcentageFuite = fuite;
    
    nouveauTuyau->pSuivant = parent->listeEnfants;
    parent->listeEnfants = nouveauTuyau;
    
    parent->nbEnfants++;
}
