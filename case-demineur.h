/*
	case d'un jeu de démineur

	Une case d'un jeu de démineur est caractérisée par
	- le fait d'être ou non minée		-> booléen
	- le fait d'être ou non dévoilée		-> booléen
	- une marque éventuelle : mine, mine?

	Valeurs par défaut :
	- case non minée, non dévoilée, non marquée

	Ensuite, une case :
	- peut devenir minee
	- peut devenir devoilée
	- peut passer
			de la marque MARQUE_AUCUNE à MARQUE_MINE,
			de la marque MARQUE_MINE à MARQUE_MINE_INTERRO,
			de la marque MARQUE_MINE_INTERRO à MARQUE_AUCUNE

	Améliorations possibles :
	- gérer un champ nb_mines_adjacantes qui serait mis à jour dynamqiuement (plutôt que recalculer ce nombre au besoin)
*/

#ifndef CASE_DEMINEUR_H
#define CASE_DEMINEUR_H

/* ________
	Structures de données
*/

/* __ Type booléen */
enum e_booleen{
	BOOLEEN_FAUX,
	BOOLEEN_VRAI
};

typedef enum e_booleen booleen;

/* __ Marquage d'une case */
enum e_marque{
	MARQUE_AUCUNE,
	MARQUE_INTERRO,
	MARQUE_MINE,				/* le joueur pense que la case est minée */
	MARQUE_MINE_INTERRO		/* le joueur fait l'hypothèse que la case est minée */
};

typedef enum e_marque marque;

/* __ Case */
struct s_case_demineur {
    booleen est_minee;
    booleen est_devoilee;
	marque	marque_joueur;	
};

typedef struct s_case_demineur case_demineur;

/* ________
	Fonctions de manipulation
*/

/* Accesseurs en ecriture / lecture
	Précondition pour tous :  c est l'adresse d'une variable case_demineur
*/

/* 	Postcondition : les champs est_minee, est_devoilee, marque_joueur de la variable *c valent respectivement BOOLEEN_FAUX, BOOLEEN_FAUX et MARQUE_AUCUNE */
void case_initialiser(case_demineur* c);

/* 	Postcondition : le champ est_minee de la variable *c vaut BOOLEEN_VRAI */
void case_miner(case_demineur* c);

/* 	Postcondition : le champ est_devoilee de la variable *c vaut BOOLEEN_VRAI */
void case_devoiler(case_demineur* c);

/*	Précondition :  le champ marque_joueur est à valeur dans {MARQUE_AUCUNE, MARQUE_MINE, MARQUE_MINE_INTERRO}
 	Postcondition : le champ marque_joueur de la variable *c vaut :
		MARQUE_MINE 			si sa marque est initialement MARQUE_AUCUNE,
		MARQUE_MINE_INTERRO si sa marque est initialement MARQUE_MINE,
		MARQUE_AUCUNE		si sa marque est initialement MARQUE_MINE_INTERRO */
void case_marquer(case_demineur* c);

/*	Renvoie le champ est_minee de la variable *c */
booleen case_est_minee(case_demineur* c);

/*	Renvoie le champ est_devoilee de la variable *c */
booleen case_est_devoilee(case_demineur* c);

/*	Renvoie le champ marque_joueur de la variable *c	*/
marque case_get_marque_joueur(case_demineur* c);

#endif

