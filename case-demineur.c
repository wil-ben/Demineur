/* Case d'un jeu de dmineur : fichier source
*/

#include <assert.h>	/* pour assert() */
#include "case-demineur.h"

/* ________ Fonctions de manipulation
*/

/* Accesseurs en ecriture / lecture
*/

/* (R)initialise la case */
void case_initialiser(case_demineur* c) {
	c->est_minee = BOOLEEN_FAUX;
	c->est_devoilee = BOOLEEN_FAUX;
	c->marque_joueur = MARQUE_AUCUNE;
	c->nbmines_vois = 0;
}

/* Mine la case */
void case_miner(case_demineur* c) {
	c->est_minee = BOOLEEN_VRAI;
}

/* Dvoile la case (l'ventuelle marque de la case est alors efface) */
void case_devoiler(case_demineur* c) {
	c->est_devoilee = BOOLEEN_VRAI;
	c->marque_joueur = MARQUE_AUCUNE;
}

/* Modifie la marque de la case : une marque MARQUE_AUCUNE, MARQUE_MINE ou MARQUE_INTERRO devient respectivement MARQUE_MINE, MARQUE_INTERRO ou MARQUE_AUCUNE) */
void case_marquer(case_demineur* c) {
	switch(c->marque_joueur) {
		case MARQUE_AUCUNE :
			c->marque_joueur = MARQUE_MINE;
			break;

		case MARQUE_MINE :
			c->marque_joueur = MARQUE_INTERRO;
			break;

		case MARQUE_INTERRO :
			c->marque_joueur = MARQUE_AUCUNE;
			break;

		default:
			assert(c->marque_joueur == MARQUE_MINE || c->marque_joueur == MARQUE_INTERRO || c->marque_joueur == MARQUE_AUCUNE);
	}
}

/* Renvoie BOOLEEN_VRAI si la case est mine et BOOLEEN_FAUX sinon */
booleen case_est_minee(case_demineur* c) {
	return c->est_minee;
}

/* Renvoie BOOLEEN_VRAI si la case est dvoile et BOOLEEN_FAUX sinon */
booleen case_est_devoilee(case_demineur* c) {
	return c->est_devoilee;
}

/* Renvoie la marque associe ˆ la case */
marque case_get_marque(case_demineur* c) {
	return c->marque_joueur;
}

/* Augmente d'une unit le nombre de mines voisines ˆ une case */
void case_ajouter_mine(case_demineur* c) {
	c->nbmines_vois ++;
}

/* Renvoie le nombre de mines voisines ˆ la case */
unsigned int case_get_nbmines(case_demineur* c) {
	return c->nbmines_vois;
}
