/* case d'un jeu de démineur : fichier d'entête

	Une case d'un jeu de démineur est caractérisée par
	- le fait d'être ou non minée							-> type énuméré booleen
	- le fait d'être ou non dévoilée						-> type énuméré booleen
	- une marque éventuelle : mine, mine?, pas de marque	-> type énuméré marque
	- le nombre de mines qui l"entourent					-> entier

	Initialement, une case est (valeurs par défaut) :
	- case non minée, non dévoilée, non marquée
	et son nombre de mines voisines est 0

	On peut ensuite:
	- miner une case
	- attribuer à une case son nombre de mines voisines
	- dévoiler une case (son éventuelle marque est alors effacée)
	- faire évoluer la marque d'une caser 	de	MARQUE_AUCUNE à MARQUE_MINE, MARQUE_MINE 
											à 	MARQUE_INTERRO, MARQUE_INTERRO à MARQUE_AUCUNE
	- réinitialiser une case
*/

#ifndef CASE_DEMINEUR_H
#define CASE_DEMINEUR_H

/* ________
	Structures de données
*/

/* __ Type booléen */
enum e_booleen {
	BOOLEEN_FAUX,
	BOOLEEN_VRAI
};
typedef enum e_booleen booleen;

/* __ Marquage d'une case */
enum e_marque {
	MARQUE_AUCUNE,
	MARQUE_MINE,		/* le joueur pense que la case est minée */
	MARQUE_INTERRO		/* le joueur fait l'hypothèse que la case est minée */
};
#define MARQUE_MIN MARQUE_AUCUNE
#define MARQUE_MAX MARQUE_INTERRO
typedef enum e_marque marque;

/* __ Case */
struct s_case_demineur {
    booleen est_minee;
    booleen est_devoilee;
	marque	marque_joueur;	
	unsigned int nbmines_vois;
};
typedef struct s_case_demineur case_demineur;

/* ________
	Fonctions de manipulation
		Pour toutes, on suppose que c est bien l'adresse mémoire d'une variable case_demineur (mémoire allouée)
*/

/* Accesseurs en ecriture / lecture
*/

/* Initialise la case
	Postconditions : les champs	est_minee, est_devoilee, marque_joueur, nbmines_vois 
		valent respectivement 	BOOLEEN_FAUX, BOOLEEN_FAUX, MARQUE_AUCUNE, 0 */
void case_initialiser(case_demineur* c);

/* Mine la case
	Postcondition : le champ est_minee vaut BOOLEEN_VRAI */
void case_miner(case_demineur* c);

/* Dévoile la case
	Postconditions : le champ est_devoilee vaut BOOLEEN_VRAI, le champ marque vaut MARQUE_AUCUNE */
void case_devoiler(case_demineur* c);

/* Modifie la marque de la case
	Précondition : la marque initiale est MARQUE_AUCUNE, MARQUE_MINE ou MARQUE_INTERRO 
		(la violation de cette condition produit une sortie en erreur du programme)
 	Postcondition : la marque est devenue
 		MARQUE_MINE 		si elle était initialement MARQUE_AUCUNE,
		MARQUE_INTERRO si elle était initialement MARQUE_MINE,
		MARQUE_AUCUNE		si elle était initialement MARQUE_INTERRO */
void case_marquer(case_demineur* c);

/* Renvoie BOOLEEN_VRAI si la case est minée et BOOLEEN_FAUX sinon */
booleen case_est_minee(case_demineur* c);

/* Renvoie BOOLEEN_VRAI si la case est dévoilée et BOOLEEN_FAUX sinon */
booleen case_est_devoilee(case_demineur* c);

/* Renvoie la marque associée à la case */
marque case_get_marque(case_demineur* c);

/* Augmente d'une unité le nombre de mines voisines à une case */
void case_ajouter_mine(case_demineur* c);

/* Renvoie le nombre de mines voisines à la case */
unsigned int case_get_nbmines(case_demineur* c);

#endif
