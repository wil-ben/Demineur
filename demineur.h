/*
	modèle du jeu de démineur

	Un jeu de démineur est caractérisé par :
	- un jeu de dimensions : hauteur (>= 1), largeur (>= 1), nombre de mines (<= hauteur * largeur)
	- un jeu de hauteur x largeur cases : il s'agit des cases d'indices (i,j) in {0 ,..., hauteur-1} x {0 ,..., largeur-1} d'un tableau de H_MAX x L_MAX cases
	- un placement des mines	 sur cases du jeu : cette information est portée par les cases du jeu qui sont minées, ou non
	- un état d'avancement du jeu : cette information est portée par les cases du jeu qui sont minées ou non, dévoilées ou non
	- de plus, chaque case du jeu peut être marquée MARQUE_MINE (le joueur pense que la case contient une mine), MARQUE_MINE_INTERRO (le joueur fait l'hypothèse que la case contient une mine) ou MARQUE_AUCUNE (pas de marque)

	Utilise :
	- dimension.h (jeu de dimensions d'une partie de démineur)
	- case-demineur.h (case d'un jeu de démineur)

	Fonctionnement :

	- Les dimensions du jeu, consistant en une hauteur h, une largeur l et un nombre nbm de mines, peuvent être définies :
				- par défaut (DIM_DEFAUT)
		ou		- par le choix d'un niveau parmi des niveaux prédéfinis (DIM_DEBUTANT, DIM_MOYEN, DIM_EXPERT)
		ou		- en spécifiant une hauteur h (>=1) et une largeur l (>=1) à partie desquels le nombre de mines est défini automatiquement (NB_MINES_DEFAUT(h,l))
		ou		- en spécifiant une hauteur h (>=1), une largeur l (>=1) et un nombre de mines nbm (0 <= nbm <= h*l)		
		pour ces deux dernières options, dans la limite d'une hauteur et d'une largeur maximales H_MAX et L_MAX

		Fonctions :	demineur* demineur_construire();
					void demineur_set_debutant(demineur* d);
					void demineur_set_moyen(demineur* d);
					void demineur_set_expert(demineur* d);
					int demineur_set_dimensions(demineur* d, unsigned int h, unsigned int l, int nbm);

					void demineur_get_dimensions(demineur* d, unsigned int* h, unsigned int* l, unsigned int* nbm);

	- Lancer une nouvelle partie consiste à : 
				- réinitialiser les cases du jeu (les cases du jeu sont alors toutes non minées, non dévoilées, non marquées) 
		puis 	- placer aléatoirement les mines parmi les cases du jeu (nbm cases du jeu sont alors minées)

		Fonctions :	void demineur_nouvelle_partie(demineur* d);
					void demineur_set_debutant(demineur* d);
					void demineur_set_moyen(demineur* d);
					void demineur_set_expert(demineur* d);
					int demineur_set_dimensions(demineur* d, unsigned int h, unsigned int l, int nbm);

	- Une partie est en l'état :
		soit		- perdue si une case minée du jeu est dévoilée
		soit		- gagnée si toutes les cases non minées du jeu sont dévoilées
		soit		- en cours dans tous les autres cas

		Fonctions :	int demineur_est_gagnee(demineur* d);
					int demineur_est_perdue(demineur* d);
					int demineur_est_en_cours(demineur* d);

	- Dérouler une partie consiste à :
				- dévoiler une case du jeu qui n'est pas encore dévoilée
		ou		- modifier la marque d'une case du jeu qui n'est pas encore dévoilée (la marque de la case devient MARQUE_MINE, MARQUE_MINE_INTERRO, MARQUE_AUCUNE si sa marque initiale est respectivement MARQUE_AUCUNE, MARQUE_MINE, MARQUE_MINE_INTERRO)

		Fonctions :	int demineur_case_devoiler(demineur* d, int i, int j);
					int demineur_case_marquer(demineur* d, int i, int j);
					int demineur_case_get_nb_mines_adj(demineur* d, int i, int j);

		__ Dévoiler une case consiste à indiquer au joueur :
				- si la case est minée : que la case est minée (partie perdue)
				- si la case est non minée : le nombre de cases du jeu qui sont adjacentes à cette case et sont minées

		Les cases adjacentes d'une case d'indice (i,j) du jeu sont les cases d'indice (i',j') du tableau de cases pour i',j' vérifiant :
			i' in {i-1, i+1}, j' in {j-1, j+1}, 0 <= i'<= hauteur -1 et 0 <= j'<= largeur -1 (au plus 8 telles cases)

		Lorsqu'une case non minée et sans mine adjacente est dévoilée par le joueur, ses cases adjacentes, si elles participent au jeu, sont automatiquement dévoilées.

	Evolution des champs des structures :
		- champs hauteur, largeur et nb_mines du champ dimension : seules les fonctions de construction et de redimensionnement agissent sur ces champs
		- champ est_minée des cases du tableau de cases : seules les fonctions qui réinitialisent une partie permettent d'agir sur ce champ
		- champ est_devoilee des cases du tableau de cases :
			- les fonctions qui réinitialisent une partie affectent ce champ à BOOLEEN_FAUX pour toute case du jeu
			- en cours de partie, ce champ passe éventuellement (une et une seule fois) de BOOLEEN_FAUX à BOOLEEN_VRAI
		- champ marque_joueur des cases du tableau de cases :
			- les fonctions qui réinitialisent une partie affectent ce champ à MARQUE_AUCUNE pour toute case du jeu
			- en cours de partie, ce champ peut évoluer un nombre indéfini de fois selon le cycle (MARQUE_AUCUNE, MARQUE_MINE, MARQUE_MINE_INTERRO, MARQUE_AUCUNE) 

	Contraintes d'intégrité vérifiées tout au long du cycle de vie :
		- les champs hauteur, largeur et nb_mines du champ dim vérifient toujours 1 <= hauteur <= H_MAX, 1 <= largeur <= L_MAX et 0 <= nb_mines <= hauteur*largeur
		- dans le tableau plateau, dim.nb_mines cases d'indice 0 <= i <= dim.hauteur -1 et 0 <= j <= dim.largeur -1 ont un champ est_minee de valeur BOOLEEN_VRAI
		- est dévoilée toute case non minée du jeu qui est liée à une case dévoilée, non minée et sans mine adjacente (dans le jeu) du jeu par un chemin de cases non minées et sans mine adjacente (dans le jeu) du jeu

	Précautions d'usage :

	- Sur le plan fonctionnel, le modèle n'interdit pas :
		- de dévoiler / marquer des cases non dévoilées du jeu tandis que la partie est finie
		- de réinitialiser la partie tandis que la partie est en cours

	- Sur le plan technique, les fonctions faisant appel aux fonctions du modèle sont responsables pour :
		- transmettre un paramètre d qui est bien l'adresse d'une variable demineur
			(cas échéants : toutes les fonctions, sinon le constructeur et le destructeur)
		- transmettre un paramètre d vérifiant, si *d != NULL, que *d est bien l'adresse d'une variable demineur
			(cas échéant : le destructeur)
		- transmettre un paramètre d qui est bien l'adresse d'une variable demineur vérifiant (*d).dim.hauteur <= H_MAX et (*d).dim.largeur <= L_MAX 
			(cas échéants : toutes les fonctions, sinon les fonctions de construction / destruction, de redimensionnement, de lecture des dimensions)
		- transmettre un paramètre d qui est bien l'adresse d'une variable demineur vérifiant (*d).dim.nb_mines <= (*d).dim.hauteur x (*d).dim.largeur
			(cas échéant : nouvelle partie)

	Améliorations possibles :
	- mémoriser les dernières dimensions utilisées (en utilisant un fichier) pour pouvoir les proposer au prochain lancement
	- gérer le temps de déroulement d'une partie (par exemple par un champ heure_debut + éventuellement heure_fin), mémoriser alors les meilleurs temps, ...
	- gérer un champ etat_partie qui serait mis à jour dynamiquement (plutôt que calculer l'état à la demande)
	- gérer un champ nb_cases_nonmonees_devoilees qui serait mis à jour dynamiquement (plutôt que calculer ce nombre à la demande)
	- gérer le pseudo du joueur
	- affiner la gestion des niveaux de difficulté (dimensions les plus pertinentes pour la proportion de mines)
*/

#ifndef DEMINEUR_H

#define DEMINEUR_H

#include "dimension.h"
#include "case-demineur.h"

/* ________________________________
	Dimension
*/

/* ________
	Structures de données
*/

/* __ Dimensions maximales & dimensions par défaut */
#define H_MAX 16
#define L_MAX 30
#define NB_MINES_DEFAUT(h,l) ( ( (h)*(l)*20) / 100 )		/* 20% de mines */

#define DIM_DEBUTANT {9,9,10}
#define DIM_MOYEN {16,16,40}
#define DIM_EXPERT {16,30,99}
#define DIM_DEFAUT DIM_DEBUTANT


enum e_demineur_niveau
{
	DEMINEUR_NIVEAU_NONE = -1,
	DEMINEUR_NIVEAU_DEBUTANT,
	DEMINEUR_NIVEAU_MOYEN,
	DEMINEUR_NIVEAU_EXPERT,
	DEMINEUR_NIVEAU_NB	/* utilise pour indiquer le nombre de niveaux prédéfinis considérés par le modèle  */
};

typedef enum e_demineur_niveau demineur_niveau;

/* __ Démineur */
struct s_demineur
{
	dimension dim;
	case_demineur plateau[H_MAX][L_MAX];
};

typedef struct s_demineur demineur;

/* __ Statut actions de jeu
	valeur STATUT_KO_INDICE < 0 pour utilisation par les accesseurs en lecture demineur_case_est_{minee,devoilee} et demineur_case_get_{marque_joueur,nb_mines_adj} */
enum e_statut_action
{
	STATUT_OK = 0,
	STATUT_KO_INDICE = -2,
	STATUT_KO_DEVOILEE = -1
};

typedef enum e_statut_action statut_action;

/* ________
	Fonctions de manipulation
*/

/* __ Construction / initialisation / destruction d'un jeu de démineur
*/

/* Cette fonction renvoie l'adresse d'une variable demineur en cas de succès, NULL en cas d'échec (échec allocation mémoire)
	Postconditions en cas de succès :
		- déclaration d'une variable demineur d (allocation dynamique)
		- les d->dim = DIM_DEFAUT
		- les d->dim.hauteur * d->dim.largeur premières cases de d->plateau sont initialisées
		- toutefois parmi ces cases, d->dim.nb_mines cases sont minées
		- graine aléatoire initialisée
	Cette fonction fait appel à la fonction demineur_nouvelle_partie */
demineur* demineur_construire();

/*	Préconditions : 
		- d adresse d'un variable demineur vérifiant (*d).dim.hauteur <= H_MAX, (*d).dim.largeur <= L_MAX et (*d).dim.nb_mines <= (*d).dim.hauteur x (*d).dim.largeur
			(contraintes techniques d'accès aux champs de d)
		- un appel préalable à la fonction srand a été fait
	Postconditions :
		- les d->dim.hauteur * d->dim.largeur premières cases de d->plateau sont initialisées
		- toutefois parmi ces cases, d->dim.nb_mines cases sont minées */
void demineur_nouvelle_partie(demineur* d);

/*	Préconditions :
		- d adresse d'une variable pointeur
		- si *d <> NULL, alors *d est l'adresse d'une variable demineur allouee dynamiquement
	Postcondition : la mémoire éventuellement allouée en l'adresse *d est libérée tandis que la variable *d vaut NULL */
void demineur_detruire(demineur** d);

/* __ Accesseurs en ecriture / lecture : dimension du jeu
	Précondition pour tous : d est l'adresse d'une variable demineur
*/

/* La fonction renvoie 1 si h <= 0 ou l <= 0 ou h > H_MAX ou l > L_MAX ou nbm > h*l, et 0 sinon
	Précondition : un appel préalable à la fonction srand a été fait
	Postcondition : si 1 <= h <= H_MAX, 1 <= l <= L_MAX et nbm <= h*l, le champ dim de la variable *d vaut :
	- {h,l,nbm} si nbm >= 0
	- {h,l,NB_MINES_DEFAUT(h,l)} sinon
	- les d->dim.hauteur * d->dim.largeur premières cases de d->plateau sont initialisées
	- toutefois parmi ces cases, d->dim.nb_mines cases sont minées
	Cette fonction fait appel à la fonction demineur_nouvelle_partie */
int demineur_set_dimensions(demineur* d, unsigned int h, unsigned int l, int nbm);

/* La fonction renvoie 0 si 	DEMINEUR_NIVEAU_NONE +1 <= niveau <= DEMINEUR_NIVEAU_NB -1 et 1 sinon
	Précondition : un appel préalable à la fonction srand a été fait
	Postcondition en cas de succès :
	- d->dim vaut DIM_DEBUTANT, DIM_MOYEN, DIM_EXPERT si demineur_niveau vaut respectivement DEMINEUR_NIVEAU_DEBUTANT, DEMINEUR_NIVEAU_MOYEN, DEMINEUR_NIVEAU_EXPERT
	- les d->dim.hauteur * d->dim.largeur premières cases de d->plateau sont initialisées
	- toutefois parmi ces cases, d->dim.nb_mines cases sont minées
	Cette fonction fait appel à la fonction demineur_set_dimensions */
int demineur_set_niveau(demineur* d, demineur_niveau niveau);

/*	Precondition : les parametres h, l et nbm sont les adresses de variables entières
	Postcondition : les variables *h, *l et *nbm valent le champ respectivement hauteur, largeur et nb_mines de la variable (*d).dim */
void demineur_get_dimensions(demineur* d, unsigned int* h, unsigned int* l, unsigned int* nbm);

/* __ Accesseurs en ecriture / lecture : case
	Préconditions pour tous : d est l'adresse d'un variable demineur vérifiant en outre (*d).dim.hauteur <= H_MAX, (*d).dim.largeur <= L_MAX (contraintes techniques)
*/

/*	Cette fonction renvoie :
		- STATUT_KO_INDICE si la case d'indice (i,j) est hors borne ssi (i,j) not in {0,...,d->dim.hauteur -1} x {0,...,d->dim.largeur -1}
		- STATUT_KO_DEVOILEE si la case d'indice (i,j) participe au jeu mais est déjà dévoilée
		- STATUT_OK sinon (la case d'indice (i,j) peut alors être dévoilée)
	Postconditions si la case d'indice (i,j) peut être dévoilée :
	- la case d'indice (i,j) est dévoilée
	- si la case d'indice (i,j) est non minée et n'est adjacente à aucune mine du jeu, toute case non minée et initialement non dévoilée du jeu que l'on peut atteindre depuis la case (i,j) par un chemin de cases du jeu qui sont toutes non minées et sans mine adjacente dans le jeu est dévoilée
		L'exploration des cases à dévoiler se fait alors en profondeur d'abord, par la pile des appels récursifs à la fonction statique demineur_devoiler_depuis_case
	Cette fonction fait appel à la fonction demineur_case_get_nb_mines_adj */
int demineur_case_devoiler(demineur* d, int i, int j);

/*	Cette fonction renvoie :
		- STATUT_KO_INDICE si la case d'indice (i,j) est hors borne ssi (i,j) not in {0,...,d->dim.hauteur -1} x {0,...,d->dim.largeur -1}
		- STATUT_KO_DEVOILEE si la case d'indice (i,j) participe au jeu mais est déjà dévoilée
		- STATUT_OK sinon (la case d'indice (i,j) peut alors être marquée)
	Postconditions si la case d'indice (i,j) peut être marquée :
		le champ marque_joueur de la case d'indice (i,j) vaut :
		MARQUE_MINE 			si sa marque est initialement MARQUE_AUCUNE,
		MARQUE_MINE_INTERRO si sa marque est initialement MARQUE_MINE,
		MARQUE_AUCUNE		si sa marque est initialement MARQUE_MINE_INTERRO	*/
int demineur_case_marquer(demineur* d, int i, int j);

/*	Cette fonction renvoie :
		- STATUT_KO_INDICE si la case d'indice (i,j) est hors borne ssi (i,j) not in {0,...,d->dim.hauteur -1} x {0,...,d->dim.largeur -1}
		- sinon, 1 si la case d'indice (i,j) est minée et 0 sinon */
int demineur_case_est_minee(demineur* d, int i, int j);

/*	Cette fonction renvoie :
		- STATUT_KO_INDICE si la case d'indice (i,j) est hors borne ssi (i,j) not in {0,...,d->dim.hauteur -1} x {0,...,d->dim.largeur -1}
		- sinon, 1 si la case d'indice (i,j) est dévoilée et 0 sinon */
int demineur_case_est_devoilee(demineur* d, int i, int j);

/*	Cette fonction renvoie :
		- STATUT_KO_INDICE si la case d'indice (i,j) est hors borne ssi (i,j) not in {0,...,d->dim.hauteur -1} x {0,...,d->dim.largeur -1}
		- la marque de la case d'indice (i,j) sinon */
marque demineur_case_get_marque_joueur(demineur* d, int i, int j);

/*	Cette fonction renvoie :
		- STATUT_KO_INDICE si la case d'indice (i,j) est hors borne ssi (i,j) not in {0,...,d->dim.hauteur -1} x {0,...,d->dim.largeur -1}
		- le nombre de cases minées parmi les cases adjacentes à la case d'indice (i,j) qui participent à la partie sinon */
int demineur_case_get_nb_mines_adj(demineur* d, int i, int j);

/* __ Accesseurs en ecriture / lecture : partie
	Préconditions pour tous : d est l'adresse d'un variable demineur vérifiant en outre
								(*d).dim.hauteur <= H_MAX, (*d).dim.largeur <= L_MAX (contraintes techniques)
*/

/*	Cette fonction renvoie la différence entre le nombre de mines du jeu et le nombre de cases du jeu qui sont non dévoilées et marquées MARQUE_MINE par le joueur */
int demineur_get_nb_mines_restant_a_deviner(demineur* d);

/*	Renvoie 1 si aucune case minée du jeu n'est dévoilée tandis que toutes les cases non minées du jeu sont dévoilées, 0 sinon */
int demineur_est_gagnee(demineur* d);

/*	Renvoie 1 si une case minée du jeu est dévoilée, 0 sinon */
int demineur_est_perdue(demineur* d);

/*	Renvoie 1 si la partie n'est ni gagnée, ni perdue, 0 sinon
	Cette fonction fait appel aux fonctions demineur_est_gagnee et demineur_est_perdue  */
int demineur_est_encours(demineur* d);

#endif

