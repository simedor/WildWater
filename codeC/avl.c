#include "avl.h"
pArbre creerArbre(){
	pAVL nv=malloc(sizeof(AVL));
  nv->u=NULL;
  nv->capte=nv->conso=nv->capacitemax=0;
	nv->fg=nv->fd=NULL;
	nv->eq=0;
return nv;
}
parbre rotationgauche(parbre a){
	int eq_a=0
	int eq_p=0;
	parbre pivot=a->fd;
	a->fd=pivot->fg;
	eq_a=a->eq;
	eq_p=pivot->eq;
	a->eq=eq_a-min(eq_p,0)-1;
	pivot->eq=min(eq_a-2, eq_a+eq_p-2,eq_p-1);
	a=pivot;
	return a;
}
parbre rotationdroite(parbre a){
	int eq_a=0
	int eq_p=0;
	parbre pivot=a->fg;
	a->fg=pivot->fd;
	eq_a=a->eq;
	eq_p=pivot->eq;
	a->eq=eq_a-min(eq_p,0)+1;
	pivot->eq=max(eq_a+2, eq_a+eq_p+2,eq_p+1);
	a=pivot;
	return a;
}
Arbre* equilibrerAVL(Arbre* a) {
	if (a != NULL) {
		if (a->equilibre <= -2) {
			if (a->fg->equilibre <= 0) {
				return rotationDroite(a);
			} else {
				return rotationDroiteDouble(a);
			}
		} else if (a->equilibre >= 2) {
			if (a->fd->equilibre >= 0) {
				return rotationGauche(a);
			} else {
				return rotationGaucheDouble(a);
			}
		}
	}
	return a;
}

Arbre* insertionAVL(Arbre* a, int v, int* h) {
	if (a == NULL) {
		*h = 1;
		return creerArbre(v);
	}
	if (v < a->value) {
		a->fg = insertionAVL(a->fg, v, h);
		*h = -*h;
	} else if (v > a->value) {
		a->fd = insertionAVL(a->fd, v, h);
	} else {
		*h = 0;
		return a;
	}
	if (*h != 0) {
		a->equilibre = a->equilibre + *h;
		a = equilibrerAVL(a);
		if (a->equilibre == 0) {
			*h = 0;
		} else {
			*h = 1;
		}
		
	}
	return a;
}
