/* Jeu de démineur : programme test

	____ TODO :
	Test assert() :
	- indice de case invalide : fonctions 
		int demineur_case_est_minee(demineur* d, int i, int j);
		int demineur_case_est_devoilee(demineur* d, int i, int j);
		marque demineur_case_get_marque(demineur* d, int i, int j);
		int demineur_case_get_nbmines_adj(demineur* d, int i, int j);
	- case de marque incomprise : fonction 
		marque demineur_case_get_marque(demineur* d, int i, int j);

	Tester bonne évolution du champ état (depuis l'introduction de l'état PARTIE_INIT)

	Tester la bonne gestion du temps : fonctions
		int demineur_case_devoiler(demineur* d, int i, int j);
		int demineur_case_marquer(demineur* d, int i, int j);
		double demineur_get_temps(demineur* d);
*/

#ifndef DEMINEUR_TEST_H

#include "demineur.h"

/*	Tests à dérouler :
 	1:	Construction / destruction / ré-initialisation
	2:	Redimensionnement (dont ré-initialisation) automatique
	3:	Redimensionnement (dont ré-initialisation) manuel	
	4:	Accesseurs (lecture, calcul, action de jeu) aux cases, sur des cases fonctionnellement incorrectes
	5:	Accesseurs en lecture aux champs des structures dimension et cases du jeu d'une structure demineur
	6:	Calcul du nombre de mines du jeu qui sont adjacentes à une case du jeu
	7:	Action de jeu marquer case, sur case fonctionnellement correcte 
	8:	Action de jeu devoiler case, sur case fonctionnellement correcte 
	9:	Calcul du nombre de mines restant à deviner
	10:	Detection partie gagnee => test accesseurs en lecture 'demineur_est_gagnee' et 'demineur_est_encours'
	11:	Detection partie perdue => test accesseurs en lecture 'demineur_est_perdue' et 'demineur_est_encours'
*/
typedef enum e_test {
	TEST_CONSTRUCTION =0,
	TEST_REDIM_AUTO,
	TEST_REDIM_MANUEL,
	TEST_CASE_MARQUER,
	TEST_CASE_DEVOILER,
	TEST_GET
} demineur_test_choix;
#define TEST_MIN TEST_CONSTRUCTION
#define TEST_MAX TEST_GET

/* ________________________________
	Fonctions
*/

/* ____ Programme test */
	
/* Déroulement d'une fonction de test spécifique */
void demineur_test(demineur_test_choix choix);

/* ____ Fonctions de test */

/* Construction / destruction / ré-initialisation
	Fonctions testées :
	- demineur* demineur_construire();
	- void demineur_nouvelle_partie(demineur* d);
	- void demineur_detruire(demineur** d); */
void demineur_test_init();

/* Redimensionnement (dont ré-initialisation) pour des jeux de dimension prédéfinis
	Fonction testée :
	- int demineur_set_niveau(demineur* d, demineur_niveau niveau);
	Cette fonction fait appel à la fonction 'demineur_construire' du modèle, qu'il s'agit alors d'avoir péalablement testée */
void demineur_test_niveaux();

/* Redimensionnement (dont ré-initialisation) manuel
	Fonction testée :
	- int demineur_set_dimensions(demineur* d, unsigned int h, unsigned int l, int nbm);
	Cette fonction fait appel aux fonctions :
	- demineur* demineur_construire();
	- int demineur_set_niveau(demineur* d, demineur_niveau niveau);
	du modèle, qu'il s'agit alors d'avoir péalablement testées */
void demineur_test_dimensions_manuelles();

/* Action de jeu : marquer une case (indices corrects et indices incorrects) */
void demineur_test_marquer_case();

/* Action de jeu : dévoiler une case (indices corrects et indices incorrects) */
void demineur_test_devoiler_case();

/* Accesseurs en lecture aux champs de la dimension et des cases de jeu d'une structure demineur
	Fonctions testées :
	- void demineur_get_dimensions(demineur* d, unsigned int* h, unsigned int* l, unsigned int* nbm);
	- int demineur_case_est_minee(demineur* d, int i, int j);
	- int demineur_case_est_devoilee(demineur* d, int i, int j);
	- marque demineur_case_get_marque_joueur(demineur* d, int i, int j);
	Cette fonction fait appel à la fonction 'demineur_construire' du modèle, qu'il s'agit alors d'avoir péalablement testée */
void demineur_test_get();

#endif

