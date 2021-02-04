/*
	case d'un jeu de démineur : implémentation des fonctions de manipulation
*/

#include "case-demineur.h"

/* ________
	Fonctions de manipulation
*/

/* Accesseurs en ecriture / lecture
	Précondition pour tous :  c est l'adresse d'une variable case_demineur
*/

/* 	Postcondition : les champs est_minee, est_devoilee, marque_joueur de la variable *c valent respectivement BOOLEEN_FAUX, BOOLEEN_FAUX et MARQUE_AUCUNE */
void case_initialiser(case_demineur* c)
{
	c->est_minee = BOOLEEN_FAUX;
	c->est_devoilee = BOOLEEN_FAUX;
	c->marque_joueur = MARQUE_AUCUNE;
}

/* 	Postcondition : le champ est_minee de la variable *c vaut BOOLEEN_VRAI */
void case_miner(case_demineur* c)
{
	c->est_minee = BOOLEEN_VRAI;
}

/* 	Postcondition : le champ est_devoilee de la variable *c vaut BOOLEEN_VRAI */
void case_devoiler(case_demineur* c)
{
	c->est_devoilee = BOOLEEN_VRAI;
}

/*	Précondition :  le champ marque_joueur est à valeur dans {MARQUE_AUCUNE, MARQUE_MINE, MARQUE_MINE_INTERRO}
 	Postcondition : le champ marque_joueur de la variable *c vaut :
		MARQUE_MINE 			si sa marque est initialement MARQUE_AUCUNE,
		MARQUE_MINE_INTERRO si sa marque est initialement MARQUE_MINE,
		MARQUE_AUCUNE		si sa marque est initialement MARQUE_MINE_INTERRO */
void case_marquer(case_demineur* c)
{
	switch(c->marque_joueur)
	{
		case MARQUE_AUCUNE :
			c->marque_joueur = MARQUE_MINE;
			break;

		case MARQUE_MINE :
			c->marque_joueur = MARQUE_MINE_INTERRO;
			break;

		case MARQUE_MINE_INTERRO :
			c->marque_joueur = MARQUE_AUCUNE;
			break;

		default:
			break;
	}
}

/*	Renvoie le champ est_minee de la variable *c */
booleen case_est_minee(case_demineur* c)
{
	return c->est_minee;
}

/*	Renvoie le champ est_devoilee de la variable *c */
booleen case_est_devoilee(case_demineur* c)
{
	return c->est_devoilee;
}

/*	Renvoie le champ marque_joueur de la variable *c	*/
marque case_get_marque_joueur(case_demineur* c)
{
	return c->marque_joueur;
}

