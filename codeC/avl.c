#include "avl.h"
pArbre creerArbre(){
	pAVL nv=malloc(sizeof(AVL));
  	nv->u=NULL;
  	nv->capte=nv->conso=nv->capacitemax=0;
	nv->fg=nv->fd=NULL;
	nv->eq=0;
return nv;
}
int max(int a, int b){
	if(a > b){
		return a;
	}
	else{
		return b;
	}
}
int min(int a, int b){
	if(a =< b){
		return a;
	}
	else{
		return b;
	}
}


pAVL rotationgauche(pAVL a){
    int eq_a = 0;
    int eq_p = 0;
    pAVL pivot = a->fd;
    a->fd = pivot->fg;
    pivot->fg = a;
    eq_a = a->eq;
    eq_p = pivot->eq;
    a->eq = eq_a - max(eq_p, 0) - 1;
    pivot->eq = min(eq_a - 2, min(eq_a + eq_p - 2, eq_p - 1));
    a = pivot;
    return a;
}

pAVL rotationdroite(pAVL a){
    int eq_a = 0;
    int eq_p = 0;
    pAVL pivot = a->fg;
    a->fg = pivot->fd;
    pivot->fd = a;
    eq_a = a->eq;
    eq_p = pivot->eq;
    a->eq = eq_a - min(eq_p, 0) + 1;
    pivot->eq = max(eq_a + 2, max(eq_a + eq_p + 2, eq_p + 1));
    a = pivot;
    return a;
}

pAVL doublerotationgauche(pAVL a){
    a->fd = rotationdroite(a->fd);
    return rotationgauche(a);
}

pAVL doublerotationdroite(pAVL a){
    a->fg = rotationgauche(a->fg);
    return rotationdroite(a);
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
