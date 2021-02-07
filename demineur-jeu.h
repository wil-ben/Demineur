/* Jeu de démineur : jeu console, fichier d'entête

	____ TODO : programme test (notamment : les E/S)
*/

#ifndef DEMINEUR_JEU_H
#define DEMINEUR_JEU_H

#include "demineur.h"

/* ________________________________
	Structures de données : types énumérés pour les actions possibles
*/typedef int etat_partie;

/* ____ Menu principal
*/
enum etat_part{
	PARTIE_GAGNEE,
	PARTIE_ENCOURS,
	PARTIE_PERDUE
};

enum e_menu_principal {
	MENU_PRINC_QUITTER,
	MENU_PRINC_REDIMENSIONNER,
	MENU_PRINC_NOUVELLE_PARTIE
};
typedef enum e_menu_principal menu_principal;

/* ____ Menu redimensionner
*/

enum e_menu_dim {
	MENU_DIM_ANNULER,
	MENU_DIM_DEBUTANT = DEMINEUR_NIVEAU_DEBUTANT,
	MENU_DIM_MOYEN = DEMINEUR_NIVEAU_MOYEN,
	MENU_DIM_EXPERT = DEMINEUR_NIVEAU_EXPERT,
	MENU_DIM_MANUEL
};
typedef enum e_menu_dim menu_dim;

/* ________________________________
	Fonctions
*/

/* ____ Entrées / Sorties
*/

/* __ Menus (affichage & saisie choix utilisateur) */

/*	Menu principal (affichage & saisie choix utilisateur)
	Cette fonction retourne une valeur parmi MENU_PRINC_QUITTER, MENU_PRINC_NOUVELLE_PARTIE, MENU_PRINC_REDIMENSIONNER */
menu_principal demineur_menu_principal();

/*	Menu redimensionner (affichage & saisie choix utilisateur) :
	Cette fonction retourne une valeur parmi MENU_DIM_ANNULER, MENU_DIM_DEBUTANT, MENU_DIM_MOYEN, MENU_DIM_EXPERT, MENU_DIM_MANUEL  */
menu_dim demineur_menu_dimensions();

/* __ Saisies */

/*	Fonction de saisie d'un jeu de dimensions
	Preconditions : h, l et nbm adresses de variables entières
	Postconditions : 1 <= *h <= H_MAX, 1 <= *l <= L_MAX, 0 <= *nbm <= (*h)x(*l) */
void demineur_saisir_dimensions(unsigned int* h, unsigned int* l, unsigned int* nbm);

/*	Fonction de saisie d'une action de jeu
	Preconditions : 
	- d adresse d'une variable demineur vérifiant (*d).dim.hauteur <= H_MAX et (*d).dim.largeur <= L_MAX
	- action, i et j adresses de variables entières
	Postconditions : acion in {0,1}, 0 <= *i <= (*d).dim.hauteur -1, 0 <= *j <= (*d).dim.largeur -1 */
void demineur_saisir_action(demineur* d, int* action, int* i, int* j);

/*	Sous-routine permettant de vider le buffer (utilisee afin de sécuriser les saisies)
	Postcondition : buffer vidé */
void purger_buffer();

/* __ Affichages */

/* 	Fonction d'affichage des dimensions courantes du démineur
	Precondition : d adresse d'une variable demineur */
void demineur_afficher_dim(demineur* d);

/* Fonction d'affichage de la partie :
	- si la partie est finie, on affiche :
		la mine éventuellement dévoilée avec '!', le nombre de mines adjacentes des autres cases dévoilées, et toutes les autres mines avec 'X'	
	- sinon (partie gagnée ou en cours), on affiche :
		le nombre de mines adjacentes des cases dévoilées et les marques des cases non dévoilées
	Preconditions : d adresse d'une variable demineur vérifiant (*d).dim.hauteur <= H_MAX et (*d).dim.largeur <= L_MAX */
void demineur_afficher_jeu(demineur* d);

/* ____ Jeu
*/

/* Fonction permettant de dérouler une partie
	Preconditions : d adresse d'une variable demineur dont les dimensions sont valides, et dans laquelle une toute nouvelle partie est en cours */
void demineur_jouer(demineur* d);

/*	Fonction permettant de redimensionner le jeu
	Precondition : d adresse d'une variable demineur */
void demineur_redimensionner(demineur* d);

int demineur_get_etat(demineur*);

#endif

