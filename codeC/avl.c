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
int equilibre(*Arbre a) {
  if (!a) return 0;
  return a
