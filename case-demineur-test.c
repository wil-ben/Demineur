/* Case d'un jeu de démineur : programme test

	TODO : 
	- tester assert() sur case_marquer()
*/

#include "case-demineur.h"
#include <stdlib.h>
#include <stdio.h>

/* ________________________________ Déclaration des fonctions
*/

/* Affichage détaillé d'une case de démineur par accès direct aux champs de la structure  */
void afficher(case_demineur* c);

/* Affichage détaillé d'une case de démineur par utilisation des fonctions de manipulation */
void afficher_get(case_demineur* c);

/* ________________________________ Programme de test
*/

int main() {
	case_demineur c = {1, 2, 3, 4};
	case_demineur cinit ={BOOLEEN_FAUX, BOOLEEN_FAUX, MARQUE_AUCUNE, 0};
	case_demineur cminee ={BOOLEEN_VRAI, BOOLEEN_FAUX, MARQUE_AUCUNE, 0};
	int i;
	marque ma;
	
	/* Test 1 : accesseurs en lecture */
	printf("Test 1 accesseurs en lecture. Les 2 affichages doivent être identiques:\n\t");
	afficher(&c);
	printf("\t");
	afficher_get(&c);

	/* Test 2 : accesseurs en ecriture */
	printf("\n\nTest 2 accesseur en écriture :");
	printf("\n\nTest 2.1 : case_initialiser. Les 2 affichages doivent être identiques:\n\t");
	case_initialiser(&c);
	afficher(&cinit);
	printf("\t");
	afficher(&c);

	printf("\n\nTest 2.2 : case_miner. Les 2 affichages doivent être identiques:\n\t");
	case_miner(&c);
	afficher(&cminee);
	printf("\t");
	afficher(&c);

	printf("\n\nTest 2.3: case_marquer. Le champ marque_joueur doit suivre le cycle (%d, %d, %d, %d):", MARQUE_AUCUNE, MARQUE_MINE, MARQUE_INTERRO, MARQUE_AUCUNE);
	for (i = 1 ; i <= 8 ; i ++) {
		printf("\n\tetat initial: ");
		afficher(&c);
		printf("\tetat final: ");
		case_marquer(&c);
		afficher(&c);
	}

	printf("\n\nTest 2.4: case_devoiler. Les champs est_devoilee et marque_joueur doivent valoir %d et %d:", BOOLEEN_VRAI, MARQUE_AUCUNE);
	for (ma = MARQUE_MIN ; ma <= MARQUE_MAX ; ma ++) {
		printf("\n\tetat initial: ");
		case_initialiser(&c);
		c.marque_joueur =ma;
		afficher(&c);
		printf("\tetat final: ");
		case_devoiler(&c);
		afficher(&c);
	}

	printf("\n\nTest 2.5: case_ajouter_mine. Le champ nbmines_vois doit être incrémenté de 1:");
	for (i = 1 ; i <= 3 ; i ++) {
		printf("\n\tetat initial: ");
		afficher(&c);
		printf("\tetat final: ");
		case_ajouter_mine(&c);
		afficher(&c);
	}

	printf("\n");

	/* Fin du programme de test */
	return EXIT_SUCCESS;
}

/* ________________________________ Définition des fonctions
*/

/* Affichage détaillé d'une case de démineur par accès direct aux champs de la structure  */
void afficher(case_demineur* c) {
	printf("[minée:%d|devoilée:%d|marque:%d|nbmines:%u]", c->est_minee, c->est_devoilee, c->marque_joueur, c->nbmines_vois);
}

/* Affichage détaillé d'une case de démineur par utilisation des fonctions de manipulation */
void afficher_get(case_demineur* c){
	printf("[minée:%d|devoilée:%d|marque:%d|nbmines:%u]", case_est_minee(c), case_est_devoilee(c), case_get_marque(c), case_get_nbmines(c));
}
