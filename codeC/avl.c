#include "header.h"

// créer un arbre et lui assigner l'usine en paramètre
pAVL creerArbre(Usine u) { 
	pAVL nouveau = malloc(sizeof(AVL));
	if (nouveau == NULL) exit(1);
	nouveau->u = malloc(sizeof(Usine));
	if (nouveau->u == NULL) exit(1);
  	strncpy(nouveau->u->ID, u.ID, 49);
	nouveau->u->ID[49] = '\0';
  	nouveau->u->volumeSource = u.volumeSource;
	nouveau->u->volumeTraite = nouveau->u->capacite = 0;
	nouveau->fg = nouveau->fd = NULL;
	nouveau->equilibre = 0;
	return nouveau;
}

int max(int a, int b) { 
	if (a > b) return a;
	return b;
}

int min(int a, int b) {
	if (a < b) return a;
	return b;
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
		// L'usine existe déjà mais on doit additionner les volumes
		a->u->volumeSource = a->u->volumeSource + u.volumeSource;
		*h = 0;
		return a;
	}
	if (*h != 0) {
		a->equilibre = a->equilibre + *h;
		a = equilibrerAVL(a);
		if (a->equilibre == 0) *h = 0;
		else *h = 1;
	}
	return a;
}

void libererMemoireAVL(pAVL a) {
    if (a != NULL) {
        libererMemoireAVL(a->fg);
        libererMemoireALV(a->fd);
		free(a->u);
        free(a);
    }
}
