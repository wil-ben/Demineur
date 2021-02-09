/* Jeu de démineur : fichier d'entête

	Dépendances:
	- dimension.h (jeu de dimensions d'une partie de démineur)
	- case-demineur.h (case d'un jeu de démineur)

**** Description rapide ****
	Un jeu de démineur est caractérisé par :
	- un tableau de H_MAX x L_MAX cases (type case_demineur), qui peuvent être:
		- minées ou non (type booleen)
		- dévoilées ou non (type booleen)
		- marquée minée, peut-être minée, ou non marquée (type marque)
		- voisines d'un certain nombre de mines
	- un jeu de dimensions (type dimension): 
		hauteur (>= 1, <= H_MAX), largeur (>= 1, <= L_MAX), nombre de mines (>= 1, <= hauteur * largeur)
		=> les cases qui participent au jeu sont celles d'indice (i, j) vérifiant 
			0 <= i < hauteur et 0 <= j < largeur
	- divers caractéristiques de la partie :
		- son heure de début (heure de la 1è action de jeu)
		- son heure de fin (heure de l'action de jeu qui a mené au gain ou à la perte de la partie)
		- son état (initialisée, en cours, gagnée ou perdue: type etat_partie)
		- le nombre de cases (non minées) qu'il reste à dévoiler
		- le nombre de cases (non dévoilées) marquées minées par le joueur

	État et heures de la partie:
	- Les fonctions d'initialisation lancent une nouvelle partie en état PARTIE_INIT
	- Une partie passe ensuite à l'état PARTIE_ENCOURS au moment de la 1è action de jeu (dévoiler ou marquer une case). C'est alors que l'heure de début est mise à jour. 
	- Une partie passe ensuite à l'état PARTIE_PERDUE ou PARTIE_GAGNEE au moment où est dévoilée, soit une case minée, soit la dernière case non minée à dévoiler. C'est alors que l'heure de fin est mise à jour.

	On peut à tout moment lancer un nouvelle partie. 
	On peut à tout moment modifier les dimensions du jeu (ce qui a pour conséquence de lancer une nouvelle partie aux dimensions requises).

**** Mode d'emploi ****

	Il faut *toujours* introduire une nouvelle variable demineur par appel à la fonction demineur_construire(), qui alloue dynamiquement la mémoire pour encoder une variable démineur, dont elle renvoie l'adresse. Ex. :
			demineur* d =demineur_construire();
		Après appel à cette fonction, une partie (niveau débutant) est prête à jouer.

	Les dimensions du jeu sont ajustables à tout moment, à l'aide des fonctions 
			demineur_set_dimensions() qui permet de définir une hauteur, une largeur et un nombre de mines personnalisés, 
			et demineur_set_niveau() qui permet d'utilise des jeux de dimensions prédéfinis
		Après appel à l'une comme l'autre de ces fonctions, une nouvelle partie aux dimensions requises est prête à jouer.
		Attention: la hauteur et la largeur de la grille de jeu sont limitées par les constantes symboliques H_MAX et L_MAX 

	On peut à tout moment lancer une nouvelle partie (aux dimensions courantes) par appel à la fonction demineur_nouvelle_partie()

	Dans une partie en cours, on peut dévoiler ou marquer une case par appel aux fonctions
		demineur_case_devoiler() et demineur_case_marquer()

	La mémoire associée aux variables demineur étant allouée dynamiquement, il faut penser à la libérer en utilisant:
			 demineur_detruire()
		Un programme doit contenir autant d'appels à la fonction demineur_detruire() qu'à la fonction demineur_construire()


**** Améliorations possibles :
	- mémoriser les dernières dimensions utilisées (en utilisant un fichier) pour pouvoir les proposer au prochain lancement
	- gérer le pseudo du joueur
	- affiner la gestion des niveaux de difficulté (dimensions les plus pertinentes pour la proportion de mines)
*/

#ifndef DEMINEUR_H

#define DEMINEUR_H

#include <time.h>
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
#define NB_MINES_DEFAUT(h, l) ( ( (h)*(l)*20) / 100 )		/* 20% de mines */

#define DIM_DEBUTANT {9, 9, 10}
#define DIM_MOYEN {16, 16, 40}
#define DIM_EXPERT {16, 30, 99}
#define DIM_DEFAUT DIM_DEBUTANT	/* par défaut, on lance une partie de niveau débutant */

enum e_demineur_niveau {
	DEMINEUR_NIVEAU_DEBUTANT,
	DEMINEUR_NIVEAU_MOYEN,
	DEMINEUR_NIVEAU_EXPERT
};
#define DEMINEUR_NIVEAU_MIN DEMINEUR_NIVEAU_DEBUTANT
#define DEMINEUR_NIVEAU_MAX DEMINEUR_NIVEAU_EXPERT
typedef enum e_demineur_niveau demineur_niveau;

/* __ Etat du jeu : une partie peut être en cours, remportée par le joueur, ou perdue */
enum e_etat_partie {
	PARTIE_INIT =0,
	PARTIE_ENCOURS =1,
	PARTIE_GAGNEE =2,
	PARTIE_PERDUE =3
};
#define PARTIE_MIN PARTIE_INIT
#define PARTIE_MAX PARTIE_PERDUE
typedef enum e_etat_partie etat_partie;

/* __ Démineur  */
struct s_demineur {
	dimension dim;		/* dimensions du jeu : hauteur et largeur de la grille, nombre de mines à trouver */
	case_demineur plateau[H_MAX][L_MAX];	/* cases du jeu */
	unsigned int nb_a_devoiler;	/* nombre de cases non minées qu'il reste à dévoiler pour gagner la partie */
	unsigned int nb_marquees;	/* nombre de cases non dévoilées marquées MARQUE_MINE par le joueur */
	etat_partie etat;	/* état du jeu (partie en cours, gagnée, ou perdue) */
	time_t debut;		/* heure de début de partie */
	time_t fin;			/* heure de fin de partie */
};
typedef struct s_demineur demineur;

/* ________
	Fonctions de manipulation

	Dans celles-ci:
	- le paramètre demineur* d, s'il n'est pas NULL, désigne *toujours* l'adresse d'une variable demineur (mémoire allouée)
	- on dit qu'un jeu {h, l, m} de dimensions est valide s'il vérifie :
		1 <= dim.h <= H_MAX
		1 <= dim.l <= L_MAX
		1 <= dim.m <= dim.h * dim.l
	- on dit qu'une paire (i, j) d'indices est valide pour une variable demineur d si elle vérifie :
		0 <= i < d.dim.h
		0 <= j < d.dim.l
	- par case du jeu, on entend une case d'indice (i, j) valide
	- par marque valide, on entend une marque dont la valeur est MARQUE_AUCUNE, MARQUE_MINE ou MARQUE_INTERRO
*/

/* __ Construction / initialisation / destruction d'un jeu de démineur
*/

/* Renvoie l'adresse d'une variable demineur en cas de succès, NULL en cas d'échec (échec allocation mémoire)
	Postconditions en cas de succès, si d désigne l'adresse retournée :
	- d est l'adresse d'une variable demineur (mémoire dynamiquement allouée)
	- d->dim est initialisé à DIM_DEFAUT
	- s'il s'agit de la 1è entrée dans la fonction, la graine aléatoire est initialisée
	- une nouvelle partie est prête à jouer -> voir postconditions de demineur_nouvelle_partie() */
demineur* demineur_construire();

/* Initialise une nouvelle partie de démineur aux dimensions d->dim dans le jeu passé en paramètre
	Précondition : les dimensions courantes du jeu sont valides
		(la violation de cette condition produit une sortie en erreur du programme)
	Postconditions :
	- les d->dim.hauteur * d->dim.largeur premières cases de d->plateau sont initialisées (non dévoilées, non marquées, non minées), à l'exception de d->dim.nbmines de ces cases, choisies aléatoirement, qui sont minées. Pour toutes ces cases, le nombre de mines voisines est renseigné. 
	- les champs nb_a_devoiler et nb_marquees sont initialisés à respectivement d->dim.h * d->dim.l -d->dim.m et 0
	- la partie est en cours (champ etat de valeur PARTIE_INIT) 
	- le champ debut est NULL */
void demineur_nouvelle_partie(demineur* d);

/* Libère la mémoire éventuellement occupée par une variable demineur, et renvoie NULL
	Précondition : si d <> NULL, alors d est bien l'adresse d'une variable demineur allouee dynamiquement
	Postcondition : si d <> NULL, la mémoire allouée en l'adresse d est libérée */
demineur* demineur_detruire(demineur* d);

/* __ Accesseurs en ecriture / lecture : dimension du jeu
	Précondition pour tous : d est l'adresse d'une variable demineur
*/

/* Met à jour les dimensions d'un jeu aux dimensions transmises, lance une nouvelle partie à ces dimensions et renvoie 0 si les valeurs h, l et m transmises sont corrects, ne fait rien que renvoyer 1 sinon
	Précondition : un appel préalable à la fonction srand() a été fait
	Postconditions si 0 <= h < H_MAX, 0 <= l < L_MAX et m <=0 ou 1 <= m <= h*l:
	- les nouvelles dimensions du jeu sont {h, l, m} si m >=1, {h, l, NB_MINES_DEFAUT(h, l)} sinon
	- une nouvelle partie est prête à jouer -> voir postconditions de demineur_nouvelle_partie() */
int demineur_set_dimensions(demineur* d, unsigned int h, unsigned int l, unsigned int m);

/* Réinitialise les dimensions à un niveau prédéfini, initialise une nouvelle partie et renvoie 0 si le niveau est reconnu, renvoie 1 sinon
	Précondition : un appel préalable à la fonction srand() a été fait
	Postconditions si le niveau est reconnu:
	- les nouvelles dimensions du jeu sont celles qui son spécifiées dans le niveau indiqué
	- une nouvelle partie est prête à jouer -> voir postconditions de demineur_nouvelle_partie() */
int demineur_set_niveau(demineur* d, demineur_niveau niveau);

/* Récupère dans des variables passées par adresse la dimension courante du jeu
	Precondition : h, l et m sont bien les adresses de variables entières (mémoire allouée)
	Postcondition : {*h, *l, *m} vaut (*d).dim */
void demineur_get_dimensions(demineur* d, unsigned int* h, unsigned int* l, unsigned int* m);

/* __ Accesseurs en ecriture / lecture : case
*/

/* Dévoile la case (i, j) du jeu si l'indice est valide, la partie est en cours et la case n'est pas dévoilée et renvoie 0 alors, renvoie 1 sinon 
	Precondition: (i, j) est un indice de case valide 
		(la violation de cette condition produit une sortie en erreur du programme)
	Postconditions si la case (i, j) peut être dévoilée:
	- s'il s'agit de la première action de jeu, le champ debut prend pour valeur l'heure courante
	- la case est dévoilée
	- si la case est minée, l'état de la partie devient PARTIE_PERDUE et le champ fin prend pour valeur l'heure courante
	- sinon, on dévoile toute case non dévoilée du jeu que l'on peut atteindre depuis cette case par un chemin de cases du jeu qui sont toutes non minées et sans mine adjacente dans le jeu, et :
		- le champ nb_a_devoiler est décrémenté du nombre de cases non minées que la fonction a dévoilées
		- si nb_a_devoiler est nul, l'état de la partie devient PARTIE_GAGNEE et le champ fin prend pour valeur l'heure courante
		- sinon, et il s'agissait de la 1è action de jeu, l'état de la partie devient PARTIE_ENCOURS
	- la marque des cases dévoilées est réinitialisée à MARQUE_NONE
	- le champ nb_marquees est mis à jour en conséquence */
int demineur_case_devoiler(demineur* d, int i, int j);

/* Modifie la marque de la case (i, j) du jeu si l'indice est valide, la partie est en cours et la case n'est pas dévoilée et 
renvoie 0 alors, renvoie 1 sinon 
	Preconditions: 
	- (i, j) est un indice de case valide
	- la marque de la case (i, j) est valide 
		(la violation d'une de ces conditions produit une sortie en erreur du programme)
	Postconditions si la case (i, j) peut être marquée: 
	- s'il s'agit de la première action de jeu, le champ debut prend pour valeur l'heure courante, et l'état de la partie devient PARTIE_ENCOURS
	- si la marque initiale est MARQUE_AUCUNE, la nouvelle marque est MARQUE_MINE, et le champ nb_marquees est augmenté de 1
	- si la marque initiale est MARQUE_MINE, la nouvelle marque est MARQUE_INTERRO, et le champ nb_marquees est diminué de 1
	- si la marque initiale est MARQUE_INTERRO, la nouvelle marque est MARQUE_AUCUNE (le champ nb_marquees est inchangé)
	- s'il s'agit de la première action de jeu, le champ debut prend pour valeur l'heure courante (1 échec ce l'allocation mémoire provoque alors une sortie en erreur) */
int demineur_case_marquer(demineur* d, int i, int j);

/* Renvoie 1 si la case (i, j) est minée, 0 sinon 
	Precondition: (i, j) est un indice de case valide 
		(la violation de cette condition produit une sortie en erreur du programme) */
int demineur_case_est_minee(demineur* d, int i, int j);

/* Renvoie 1 si la case (i, j) est dévoilée, 0 sinon 
	Precondition: (i, j) est un indice de case valide 
		(la violation de cette condition produit une sortie en erreur du programme) */
int demineur_case_est_devoilee(demineur* d, int i, int j);

/* Renvoie la marque de la case (i, j)
	Precondition: (i, j) est un indice de case valide 
		(la violation de cette condition produit une sortie en erreur du programme) */
marque demineur_case_get_marque(demineur* d, int i, int j);

/* Renvoie le nombre de mines au voisinage de la case (i, j) (i.e., situées sur des cases du jeu qui lui sont adjacentes)
	Precondition: (i, j) est un indice de case valide 
		(la violation de cette condition produit une sortie en erreur du programme) */
unsigned int demineur_case_get_nbmines_adj(demineur* d, int i, int j);

/* __ Accesseurs en ecriture / lecture*/

/* Renvoie la différence entre le nombre de mines du jeu, et le nombre de cases non dévoilées du jeu que le joueur a marquées MARQUE_MINE */
unsigned int demineur_get_nbmines_a_deviner(demineur* d);

/* Renvoie le nombre de case restant à dévoiler du jeu */
unsigned int demineur_get_nbcases_a_devoiler(demineur* d);

/* Renvoie l'état de la partie */
etat_partie demineur_get_etat(demineur* d);

/* Renvoie le temps écoulé depuis la 1è action de jeu (entre la 1è et la dernière si la partie est finie, 0 si la partie n'a pas commencé) */
long unsigned int demineur_get_temps(demineur* d);

#endif
