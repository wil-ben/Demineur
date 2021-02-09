/* Jeu de démineur : jeu console, fichier source
*/

#include "demineur-jeu.h"

#include <stdlib.h>
#include <stdio.h>

/* ____ Fonction principale
*/
int main() {
	menu_principal choix_princ = MENU_PRINC_NOUVELLE_PARTIE;
	demineur* d = NULL;

	/* __ construction & initialisation d'une partie (dimensions par défaut) */
	d =demineur_construire();
	if (d == NULL) {
		printf("Programme %s :: allocation memoire impossible, sortie du programme en erreur.\n", __FILE__);
		return EXIT_FAILURE;
	}

	/* __ boucle infinie de menu */
	while(choix_princ != MENU_PRINC_QUITTER) {
		demineur_afficher_dim(d);
		choix_princ = demineur_menu_principal();

		if (choix_princ == MENU_PRINC_NOUVELLE_PARTIE) {
			demineur_jouer(d);
			demineur_nouvelle_partie(d);
		}
		else if (choix_princ == MENU_PRINC_REDIMENSIONNER) {
			demineur_redimensionner(d);
		}

		/* on purge le buffer (entrée clavier stdin) avant nouvelle itération */
		purger_buffer();
	}

	/* __ on quitte */

	/* ... poliment */
	printf("Au revoir... merci d'avoir joue avec nous !\n");

	/* ... et proprement (destruction) */
	d =demineur_detruire(d);

	return EXIT_SUCCESS;
}

/* ____ Entrées / Sorties
*/

/* __ Menus (affichage & saisie choix utilisateur) */

/*	Menu principal (affichage & saisie choix utilisateur)
	Cette fonction retourne une valeur parmi MENU_PRINC_QUITTER, MENU_PRINC_NOUVELLE_PARTIE, MENU_PRINC_REDIMENSIONNER */
menu_principal demineur_menu_principal() {
	menu_principal choix = MENU_PRINC_QUITTER;
	char c;

	printf("Saisir 'J'/'j' pour lancer une partie dans les dimensions actuelles, 'D'/'d' pour dimensionner, toute autre touche pour quitter\n");
	c = getchar();

	if (c == 'J' || c == 'j')
		choix = MENU_PRINC_NOUVELLE_PARTIE;
	else if (c == 'D' || c == 'd') 
		choix = MENU_PRINC_REDIMENSIONNER;

	/* restitution saisie */
	printf("\tvous avez saisi '%c'\n", c);

	return choix;
}

/*	Menu redimensionner (affichage & saisie choix utilisateur) :
	Cette fonction retourne une valeur parmi MENU_DIM_ANNULER, MENU_DIM_DEBUTANT, MENU_DIM_MOYEN, MENU_DIM_EXPERT, MENU_DIM_MANUEL  */
menu_dim demineur_menu_dimensions() {
	menu_dim choix = MENU_DIM_ANNULER;
	char c;

	dimension deb = DIM_DEBUTANT; 
	dimension moy = DIM_MOYEN; 
	dimension exp = DIM_EXPERT; 

	/* invitation saisie */
	printf("____ Menu redimensionner -- Saisir :\n");
	printf("\t'D'/'d' pour partie débutant : %u * %u cases pour %u mines\n", dimension_get_hauteur(& deb), dimension_get_largeur(& deb), dimension_get_nbmines(& deb));
	printf("\t'M'/'m' pour partie moyen : %u * %u cases pour %u mines\n", dimension_get_hauteur(& moy), dimension_get_largeur(& moy), dimension_get_nbmines(& moy));
	printf("\t'E'/'e' pour partie expert : %u * %u cases pour %u mines\n", dimension_get_hauteur(& exp), dimension_get_largeur(& exp), dimension_get_nbmines(& exp));
	printf("\t'S'/'s' pour saisie libre\n");
	printf("\ttoute autre touche pour annuler\n");

	/* saisie & restitution saisie */
	purger_buffer();
	c = getchar();
	printf("\tvous avez saisi '%c'.\n", c);

	/* traduction en menu */
	if (c == 'D' || c == 'd')
		choix = MENU_DIM_DEBUTANT;
	else if (c == 'M' || c == 'm') 
		choix = MENU_DIM_MOYEN;
	else if (c == 'E' || c == 'e') 
		choix = MENU_DIM_EXPERT;
	else if (c == 'S' || c == 's') 
		choix = MENU_DIM_MANUEL;

	return choix ;
}

/* __ Saisies */

/*	Fonction de saisie d'un jeu de dimensions
	Preconditions : h, l et m adresses de variables entières
	Postconditions : 1 <= *h <= H_MAX, 1 <= *l <= L_MAX, 0 <= *m <= (*h)x(*l) */
void demineur_saisir_dimensions(unsigned int* h, unsigned int* l, unsigned int* m) {
	unsigned int ko=1;
	int n = 0;

	/* Saisie : choix valeurs */
	*h = 0;
	*l = 0;
	*m = 0;

	while(ko) {
		purger_buffer();
		printf("Saisir 'h l n' avec h (hauteur) in [1,%d], l (largeur) in [1,%d] et n (nombre de mines) in [0,h*l] si saisie manuelle, n<0 sinon\n", H_MAX, L_MAX);
		scanf("%u %u %d", h, l, &n);

		/* contrôle (et information à l'utilisateur des erreurs éventuelles) de la validité de la saisie */
		if (1 > *h || *h > H_MAX || 1 > *l || *l > L_MAX) {
			ko = 1;
			printf("\tles dimensions (h == %u, l == %u) sont incorrectes (hors bornes) : il faut choisir h in [1,%d], l in [1,%d].\n", *h, *l, H_MAX, L_MAX);
		}
		else if (n > (int) ((*h)*(*l)) ) {
			ko = 1;
			printf("\tle nombre de mines m == %d est incoherent : il faut choisir m in [0,hxl==%u] pour le fixer manuellement, ou saisir une valeur m < 0 pour demander a ce qu'il soit fixe automatiquement.\n", n, (*h)*(*l));
		}
		else {
			ko = 0;
		}
	}

	/* affectation du nombre de mines */
	if (n < 0)
		*m = NB_MINES_DEFAUT(*h,*l);
	else
		*m = (int)n;

	/* restitution saisie */
	printf("\tnouvelles dimensions : hauteur %u, largeur %u et nombre de mines %u\n", *h, *l, *m);
}

/*	Fonction de saisie d'une action de jeu
	Preconditions : 
	- d adresse d'une variable demineur vérifiant (*d).dim.hauteur <= H_MAX et (*d).dim.largeur <= L_MAX
	- action, i et j adresses de variables entières
	Postconditions : acion in {0,1}, 0 <= *i <= (*d).dim.hauteur -1, 0 <= *j <= (*d).dim.largeur -1 */
void demineur_saisir_action(demineur* d, int* action, int* i, int* j) {
	unsigned int h, l, m, ko=1;

	/* Récupération dimensions du jeu */
	demineur_get_dimensions(d, &h, &l, &m);

	/* Saisie : choix action (dévoiler ou marquer) et case (coordonnées (i,j)) */
	*action = -1;
	*i = -1;
	*j = -1;

	while(ko) {
		printf("Saisir 'action i j' où action = 0 pour dévoiler et 1 pour marquer, i in [0,%u], j in [0,%u]\n", h -1, l -1);
		scanf("%d %d %d", action, i, j);

		/* contrôle (et information à l'utilisateur des erreurs éventuelles) de la validité de la saisie */
		if (*action < 0 || *action > 1) {
			ko = 1;
			printf("\tchoix d'action %d incompris : les choix d'action disponibles sont 0 pour devoiler une case et 1 pour modifier la marque d'une case.\n", *action);
		}
		else if (*i < 0 || *i >= (int)h || *j < 0 || *j >= (int)l ) {
			ko = 1;
			printf("\tles coordonnees (i == %d, j == %d) sont incorrectes (hors bornes) : il faut choisir i in [0,%u], j in [0,%u].\n", *i, *j, h -1, l -1);
		}
		else if (demineur_case_est_devoilee(d, *i, *j)) {
			ko = 1;
			printf("\tla case d'indice (i == %d, j == %d) ne peut etre choisie pour aucune action car celle-ci est deja devoilee.\n", *i, *j);
		}
		else
			ko = 0;
	}

	/* restitution saisie */
	if (*action == 0)
		printf("\tvous avez choisi de dévoiler la case (%d,%d)\n", *i, *j);
	else
		printf("\tvous avez choisi de marquer la case (%d,%d)\n", *i, *j);
}

/*	Sous-routine permettant de vider le buffer avant une saisie
	Postcondition : buffer vidé
	Afin de sécuriser les saisies, il est fortement conseillé de faire appel à cette fonction avant tout appel à la fonction 'scanf' */
void purger_buffer() {
	char c;

	while (1) {
		c = getchar();
		if (c == '\n' || c == EOF)
			break;
	}
}

/* __ Affichages */

/* 	Fonction d'affichage des dimensions courantes du démineur
	Precondition : d adresse d'une variable demineur */
void demineur_afficher_dim(demineur* d) {
	unsigned int h, l, m;

	/* Récupération dimensions du jeu */
	demineur_get_dimensions(d, &h, &l, &m);

	/* Etat de la partie */
	printf("\n*** Jeu de demineur : dimensions actuelles %ux%u, %u mines a trouver\n", h, l, m);
}

/* Fonction d'affichage de la partie :
	- si la partie est finie, on affiche :
		la mine éventuellement dévoilée avec '!', le nombre de mines adjacentes des autres cases dévoilées, et toutes les autres mines avec 'X'	
	- sinon (partie gagnée ou en cours), on affiche :
		le nombre de mines adjacentes des cases dévoilées et les marques des cases non dévoilées
	Preconditions : d adresse d'une variable demineur vérifiant (*d).dim.hauteur <= H_MAX et (*d).dim.largeur <= L_MAX */
void demineur_afficher_jeu(demineur* d) {
	unsigned int h, l, m;
	int i, j;
	marque ma;
	etat_partie etat;

	/* Dimensions du jeu*/
	demineur_get_dimensions(d, &h, &l, &m);
	printf("Demineur de taille %ux%u avec %u mines : partie ", h, l, m);

	/* Etat de la partie */
	etat =demineur_get_etat(d);
	if(etat == PARTIE_ENCOURS)
		printf("en cours, %d mines restent a trouver. ", demineur_get_nbmines_a_deviner(d));
	else if (etat == PARTIE_GAGNEE)
		printf("gagnee, bravo ! ");
	else if (etat == PARTIE_PERDUE)
		printf("perdue, malheureusement :( ");
	else
		printf("en etat INDETERMINE. ");

	/* Plateau */
	printf("Plateau:\n |");
	for(j=0 ; j < (int)l ; j++)
		printf("%d ", j);
	printf("|\n_");
	for(j=0 ; j < (int)l ; j++)
		printf("__");
	printf("_|\n");

	for(i=0 ; i < (int)h ; i++) {
		printf("%d|", i);
		for(j=0 ; j < (int)l ; j++) {
			/* Si la case est dévoilée : on indique la présence d'une mine si la case est minée (partie alors perdue), le nombre de mines adjacentes sinon */
			if(demineur_case_est_devoilee(d, i, j)) {
				if(! demineur_case_est_minee(d, i, j))
					printf("%d ", demineur_case_get_nbmines_adj(d, i, j));
				else
					printf("%c ", '!');
			}
			/* Si la case n'est pas dévoilée : on indique la présence d'une mine si la case est minée et la partie finie, la marque posée par le joueur sinon */
			else if (demineur_case_est_minee(d, i, j) && etat != PARTIE_ENCOURS)
				printf("%c ", 'X');
			else {
				ma = demineur_case_get_marque(d, i, j);
				if (ma == MARQUE_MINE)
					printf("%c ", 'M');
				else if (ma == MARQUE_INTERRO)
					printf("%c ", '?');
				else if (ma == MARQUE_AUCUNE)
					printf("%c ", ' ');
				else
					printf("%c ", '!');
			}
		}

		printf("|\n");
	}
}

/* ____ Jeu
*/

/* Fonction permettant de dérouler une partie
	Preconditions : d adresse d'une variable demineur dont les dimensions sont valides, et dans laquelle une toute nouvelle partie est en cours */
void demineur_jouer(demineur* d) {
	int action, i, j;

	demineur_afficher_jeu(d);
	
	do {
		demineur_afficher_jeu(d);
		demineur_saisir_action(d, &action, &i, &j);

		if (action == 0)
			demineur_case_devoiler(d, i, j);
		else
			demineur_case_marquer(d, i, j);
	} while(demineur_get_etat(d) == PARTIE_ENCOURS);

	printf("Fin de partie :\n");
	demineur_afficher_jeu(d);
}

/*	Fonction permettant de redimensionner le jeu
	Precondition : d adresse d'une variable demineur */
void demineur_redimensionner(demineur* d) {
	menu_dim choix_dim;
	unsigned int h, l, m;

	/* Saisie choix utilisateur */
	choix_dim = demineur_menu_dimensions();

	/* Traitement choix utilisateur */
	if (choix_dim == MENU_DIM_DEBUTANT || choix_dim == MENU_DIM_MOYEN || choix_dim == MENU_DIM_EXPERT)
		demineur_set_niveau(d, (demineur_niveau)choix_dim);
	else if (choix_dim == MENU_DIM_MANUEL) {
		demineur_saisir_dimensions(&h, &l, &m);
		demineur_set_dimensions(d, h, l, m);
	}
}
