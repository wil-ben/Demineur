/*
	modèle du jeu de démineur : implémentation des fonctions
*/

#include "demineur.h"
#include <stdlib.h>	/* pour allocation dynamique */
#include <time.h>	/* pour génération aléatoire */

/* ________
	Fonctions statiques
*/

/* Sous-routine de la méthode 'demineur_case_devoiler' du modele
	Préconditions : 
	- d adresse d'une variable démineur vérifiant en outre (*d).dim.hauteur <= H_MAX, (*d).dim.largeur <= L_MAX (contraintes techniques)
	- (i,j) indice d'une case non minée et sans mine adjacente du jeu qui vient d'être dévoilée
	Postconditions :
	- toute case non minée du jeu que l'on peut atteindre depuis la case (i,j) par un chemin de cases du jeu qui sont toutes non minées et sans mine adjacente dans le jeu est dévoilée
	Cette fonction récursive fait également appel à la fonction demineur_case_get_nb_mines_adj du modèle */
static void demineur_devoiler_depuis_case(demineur* d, int i, int j);

/* Sous-routine des méthodes 'demineur_get_nb_mines_restant_a_deviner', 'demineur_est_encours', 'demineur_est_gagnee', 'demineur_est_perdue' du modele
	Préconditions : 
	- d adresse d'une variable démineur vérifiant en outre (*d).dim.hauteur <= H_MAX, (*d).dim.largeur <= L_MAX (contraintes techniques)
	- nb_non_minees_devoilees, nb_minees_devoilees, nb_mines_restant_a_deviner adresses de variables entières
	Postconditions :
	- nb_non_minees_devoilees vaut le nombre de cases du jeu qui sont non minées et dévoilées
	- nb_minees_devoilees vaut le nombre de cases du jeu qui sont minées et dévoilées
	- nb_mines_restant_a_deviner vaut le nombre de mines du jeu moins le nombre de cases non dévoilées marquées MARQUE_MINE */
static void demineur_get_stat(demineur* d, int* nb_non_minees_devoilees, int* nb_minees_devoilees, int* nb_mines_restant_a_deviner);

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
demineur* demineur_construire()
{
	demineur* d = NULL;

	/* allocation dynamique */
	d = malloc(sizeof(demineur));

	/* initialisation des champs */
	if (d)
	{
		/* dimensions par défaut */
		dimension dim = DIM_DEFAUT;
		dimension_set(& d->dim, dim.hauteur, dim.largeur, dim.nb_mines);

		/* initialisation de la partie */
		srand(time(NULL));
		demineur_nouvelle_partie(d);
	}

	return d;
}

/*	Préconditions : 
		- d adresse d'un variable demineur vérifiant (*d).dim.hauteur <= H_MAX, (*d).dim.largeur <= L_MAX et (*d).dim.nb_mines <= (*d).dim.hauteur x (*d).dim.largeur
			(contraintes techniques d'accès aux champs de d)
		- un appel préalable à la fonction srand a été fait
	Postconditions :
		- les d->dim.hauteur * d->dim.largeur premières cases de d->plateau sont initialisées
		- toutefois parmi ces cases, d->dim.nb_mines cases sont minées */
void demineur_nouvelle_partie(demineur* d)
{
	int i, j, indice, cpt;
	unsigned int nb_mines_posees=0, h, l, nbm;

	/* __ initialisation des cases */

	/* Récupération des dimensions */
	h = dimension_get_hauteur(& d->dim);
	l = dimension_get_largeur(& d->dim);
	nbm = dimension_get_nb_mines(& d->dim);
	
	/* Initialisation des cases du jeu */
	for (i = 0 ; i < (int)h ; i ++)
	{
		for (j = 0 ; j < (int)l ; j ++)
		{
			case_initialiser(& d->plateau[i][j]);
		}
	}

	/* __ positionnement des mines */

	/* tant que toutes les mines n'ont pas été posées */
	while (nb_mines_posees < nbm)
	{
		/* on tire un indice de case au hasard parmi h*l -nb_mines_posees cases disponibles */
		indice = rand() % (h * l - nb_mines_posees);

		/* on parcourt les cases d'indice (i,j) du jeu non encore minées partant de i=0 et de j=0, jusqu'à tomber jusqu'à la (indice+1)ème telle case */
		cpt=-1;
		for (i = 0 ; i < (int)h ; i ++)
		{
			for (j = 0 ; j < (int)l ; j ++)
			{
				if (case_est_minee(& d->plateau[i][j]) == BOOLEEN_FAUX)
				{
					cpt++;

					/* si cpt == indice, la case d'indice (i,j) est la case recherchee */
					if (cpt == indice)
					{
						break;
					}
				}
			}

			/* si cpt == indice, la case d'indice (i,j) est la case recherchee */
			if (cpt == indice)
			{
				break;
			}
		}

		case_miner(& d->plateau[i][j]);
		nb_mines_posees ++;
	}

	while (nb_mines_posees < nbm)
	{
		/* on tire un indice de case au hasard dans {0,...,d->dim.hauteur * d->dim.largeur -1} */
		indice = rand() % (h * l);

		/* indice dont on déduit les coordonées i = floor(indice/d->dim.largeur) et j = indice % d->dim.largeur */
		j = (int) (indice % l);
		i = (int) ( (indice - j) / l);

		/* si la case de coordonnées (i,j) n'a pas déjà été minée, on pose une nouvelle mine */
		if (case_est_minee(& d->plateau[i][j]) == BOOLEEN_FAUX)
		{
			case_miner(& d->plateau[i][j]);
			nb_mines_posees ++;
		}
	}

	/* __ initialisation de la partie */

	/* ICI : mise à jour du champ etat_partie si ajout d'un champ etat_partie */
	/* ICI : mise à jour du champ nb_cases_nonminees_devoilees si ajout d'un champ nb_cases_nonminees_devoilees */
	/* ICI : mise à jour du champ heure_debut / heure_fin si ajout d'un champ heure_debut / heure_fin */
}

/*	Préconditions :
		- d adresse d'une variable pointeur
		- si *d <> NULL, alors *d est l'adresse d'une variable demineur allouee dynamiquement
	Postcondition : la mémoire éventuellement allouée en l'adresse *d est libérée tandis que la variable *d vaut NULL */
void demineur_detruire(demineur** d)
{
	if (*d != NULL)
	{
		free(*d);
		*d = NULL;
	}
}

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
int demineur_set_dimensions(demineur* d, unsigned int h, unsigned int l, int nbm)
{
	if (1 <= h && h <= H_MAX && 1 <= l && l <= L_MAX && nbm <= (int)(h*l))
	{
		/* le cas échéant, définition du nombre de mines par défaut étant données une hauteur et une largeur */
		if (nbm < 0)
		{
			nbm = NB_MINES_DEFAUT(h,l);
		}

		/* affectation des nouvelles dimensions */
		dimension_set(& d->dim, h, l, nbm);

		/* initialisation de la partie */
		demineur_nouvelle_partie(d);

		return 0;
	}

	return 1;
}

/* La fonction renvoie 0 si 	DEMINEUR_NIVEAU_NONE +1 <= niveau <= DEMINEUR_NIVEAU_NB -1 et 1 sinon
	Précondition : un appel préalable à la fonction srand a été fait
	Postcondition en cas de succès :
	- d->dim vaut DIM_DEBUTANT, DIM_MOYEN, DIM_EXPERT si demineur_niveau vaut respectivement DEMINEUR_NIVEAU_DEBUTANT, DEMINEUR_NIVEAU_MOYEN, DEMINEUR_NIVEAU_EXPERT
	- les d->dim.hauteur * d->dim.largeur premières cases de d->plateau sont initialisées
	- toutefois parmi ces cases, d->dim.nb_mines cases sont minées
	Cette fonction fait appel à la fonction demineur_set_dimensions */
int demineur_set_niveau(demineur* d, demineur_niveau niveau)
{
	int retour = 0;

	/* interprétation du paramètre demineur_niveau :
		le cas échéant, mise à jour de la variable demineur *d (affectation des nouvelles dimensions & initialisation de la partie) */
	if (niveau <= DEMINEUR_NIVEAU_NONE || niveau >= DEMINEUR_NIVEAU_NB)
	{
		retour = 1;
	}
	else if (niveau == DEMINEUR_NIVEAU_DEBUTANT)
	{
		dimension dim = DIM_DEBUTANT;
		demineur_set_dimensions(d, dimension_get_hauteur(& dim), dimension_get_largeur(& dim), (int)dimension_get_nb_mines(& dim));
	}
	else if (niveau == DEMINEUR_NIVEAU_MOYEN)
	{
		dimension dim = DIM_MOYEN;
		demineur_set_dimensions(d, dimension_get_hauteur(& dim), dimension_get_largeur(& dim), (int)dimension_get_nb_mines(& dim));
	}
	else if (niveau == DEMINEUR_NIVEAU_EXPERT)
	{
		dimension dim = DIM_EXPERT;
		demineur_set_dimensions(d, dimension_get_hauteur(& dim), dimension_get_largeur(& dim), (int)dimension_get_nb_mines(& dim));
	}

	return retour; 
}

/*	Precondition : les paramètres h, l et nbm sont les adresses de variables entières
	Postcondition : les variables *h, *l et *nbm valent le champ respectivement hauteur, largeur et nb_mines de la variable (*d).dim */
void demineur_get_dimensions(demineur* d, unsigned int* h, unsigned int* l, unsigned int* nbm)
{
	*h = dimension_get_hauteur(& d->dim);
	*l = dimension_get_largeur(& d->dim);
	*nbm = dimension_get_nb_mines(& d->dim);
}

/* __ Accesseurs en ecriture / lecture : case
	Préconditions pour tous : d est l'adresse d'un variable demineur vérifiant en outre
								(*d).dim.hauteur <= H_MAX, (*d).dim.largeur <= L_MAX (contraintes techniques)
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
int demineur_case_devoiler(demineur* d, int i, int j)
{
	/* __ vérification de la validité (fonctionelle) des paramètres */
	if (i < 0 || i >= (int)dimension_get_hauteur(& d->dim) || j < 0 || j >= (int)dimension_get_largeur(& d->dim))
	{
		return STATUT_KO_INDICE;
	}

	if (case_est_devoilee(& d->plateau[i][j]) == BOOLEEN_VRAI)
	{
		return STATUT_KO_DEVOILEE;
	}

	/* __ case fonctionnellement correcte : l'action est traitée */

	/* ICI : si première case dévoilée du jeu, mise à jour du champ heure_debut si ajout d'un champ heure_debut */

	/* _ mise à jour de la case */
	case_devoiler(& d->plateau[i][j]);

	/* _ mise à jour de la partie et du plateau */

	/* cas d'une case minée : la partie est perdue */
	if (case_est_minee(& d->plateau[i][j]) == BOOLEEN_VRAI)
	{
		/* ICI : mise à jour champ etat_partie si ajout d'un champ etat_partie */
	}
	/* cas d'une case non minée */
	else
	{
		/* ICI : mise à jour champ nb_cases_nonminees_devoilees si ajout d'un champ nb_cases_nonminees_devoilees */

		/* si la case n'est adjacente à aucune mine, on dévoile (récursivement) ses cases adjacentes */
		if(! demineur_case_get_nb_mines_adj(d,i,j))
		{
			demineur_devoiler_depuis_case(d,i,j);
		}

		/* ICI : le cas échéant (toutes les cases non minées du jeu sont dévoilées), mise à jour du champ etat_partie si ajout d'un champ etat_partie */
	}

	/* ICI : si partie finie, mise à jour du champ heure_fin si ajout d'un champ heure_fin */

	return STATUT_OK;
}

/*	Cette fonction renvoie :
		- STATUT_KO_INDICE si la case d'indice (i,j) est hors borne ssi (i,j) not in {0,...,d->dim.hauteur -1} x {0,...,d->dim.largeur -1}
		- STATUT_KO_DEVOILEE si la case d'indice (i,j) participe au jeu mais est déjà dévoilée
		- STATUT_OK sinon (la case d'indice (i,j) peut alors être marquée)
	Postconditions si la case d'indice (i,j) peut être marquée :
		le champ marque_joueur de la case d'indice (i,j) vaut :
		MARQUE_MINE 			si sa marque est initialement MARQUE_AUCUNE,
		MARQUE_MINE_INTERRO si sa marque est initialement MARQUE_MINE,
		MARQUE_AUCUNE		si sa marque est initialement MARQUE_MINE_INTERRO	*/
int demineur_case_marquer(demineur* d, int i, int j)
{
	/* __ vérification de la validité (fonctionelle) des paramètres */
	if (i < 0 || i >= (int)dimension_get_hauteur(& d->dim) || j < 0 || j >= (int)dimension_get_largeur(& d->dim))
	{
		return STATUT_KO_INDICE;
	}

	if (case_est_devoilee(& d->plateau[i][j]) == BOOLEEN_VRAI)
	{
		return STATUT_KO_DEVOILEE;
	}

	/* __ case fonctionnellement correcte : l'action est traitée */
	case_marquer(& d->plateau[i][j]);

	return STATUT_OK;
}

/*	Cette fonction renvoie :
		- STATUT_KO_INDICE si la case d'indice (i,j) est hors borne ssi (i,j) not in {0,...,d->dim.hauteur -1} x {0,...,d->dim.largeur -1}
		- sinon, 1 si la case d'indice (i,j) est minée et 0 sinon */
int demineur_case_est_minee(demineur* d, int i, int j)
{
	if (i < 0 || i >= (int)dimension_get_hauteur(& d->dim) || j < 0 || j >= (int)dimension_get_largeur(& d->dim))
	{
		return STATUT_KO_INDICE;
	}

	return (case_est_minee(& d->plateau[i][j]) == BOOLEEN_VRAI);
}

/*	Cette fonction renvoie :
		- STATUT_KO_INDICE si la case d'indice (i,j) est hors borne ssi (i,j) not in {0,...,d->dim.hauteur -1} x {0,...,d->dim.largeur -1}
		- sinon, 1 si la case d'indice (i,j) est dévoilée et 0 sinon */
int demineur_case_est_devoilee(demineur* d, int i, int j)
{
	if (i < 0 || i >= (int)dimension_get_hauteur(& d->dim) || j < 0 || j >= (int)dimension_get_largeur(& d->dim))
	{
		return STATUT_KO_INDICE;
	}

	return (case_est_devoilee(& d->plateau[i][j]) == BOOLEEN_VRAI);
}

/*	Cette fonction renvoie :
		- STATUT_KO_INDICE si la case d'indice (i,j) est hors borne ssi (i,j) not in {0,...,d->dim.hauteur -1} x {0,...,d->dim.largeur -1}
		- la marque de la case d'indice (i,j) sinon */
marque demineur_case_get_marque_joueur(demineur* d, int i, int j)
{
	if (i < 0 || i >= (int)dimension_get_hauteur(& d->dim) || j < 0 || j >= (int)dimension_get_largeur(& d->dim))
	{
		return STATUT_KO_INDICE;
	}

	return case_get_marque_joueur(& d->plateau[i][j]);
}

/*	Cette fonction renvoie :
		- STATUT_KO_INDICE si la case d'indice (i,j) est hors borne ssi (i,j) not in {0,...,d->dim.hauteur -1} x {0,...,d->dim.largeur -1}
		- le nombre de cases minées parmi les cases adjacentes à la case d'indice (i,j) qui participent à la partie sinon */
int demineur_case_get_nb_mines_adj(demineur* d, int i, int j)
{
	int nb_mines_adj = 0, iprime, jprime;
	unsigned int h = dimension_get_hauteur(& d->dim);
	unsigned int l = dimension_get_largeur(& d->dim);

	if (i < 0 || j < 0 || i >= (int)h || j >= (int)l)
	{
		return STATUT_KO_INDICE;
	}

	/* __ parcours des cases adjacentes à la case d'indice (i,j) qui participent au jeu */
	for(iprime = (i-1 >= 0 ? i-1 : 0) ; iprime <= (i+1 <= (int)h -1 ? i+1 : (int)h -1) ; iprime ++)
	{
		for(jprime = (j-1 >= 0 ? j-1 : 0) ; jprime <= (j+1 <= (int)l -1 ? j+1 : (int)l -1) ; jprime ++)
		{
			/* pour l'indice (iprime,jprime) == (i,j), on passe à l'itération suivante */
			if (iprime == i && jprime == j)
			{
				continue;
			}

			/* __ si la case d'indice (iprime,jprime) est minee : on la comptabilise */
			if (case_est_minee(& d->plateau[iprime][jprime]) == BOOLEEN_VRAI)
			{
				nb_mines_adj ++;
			}
		}
	}

	return nb_mines_adj;
}

/* __ Accesseurs en ecriture / lecture : partie
	Préconditions pour tous : d est l'adresse d'un variable demineur vérifiant en outre (*d).dim.hauteur <= H_MAX, (*d).dim.largeur <= L_MAX (contraintes techniques)
*/

/*	Cette fonction renvoie la différence entre le nombre de mines du jeu et le nombre de cases du jeu qui sont non dévoilées et marquées MARQUE_MINE par le joueur */
int demineur_get_nb_mines_restant_a_deviner(demineur* d)
{
	int nb_non_minees_devoilees, nb_minees_devoilees, nb_mines_restant_a_deviner;

	/* récupération des statistiques du jeu */
	demineur_get_stat(d, &nb_non_minees_devoilees, &nb_minees_devoilees, &nb_mines_restant_a_deviner);

	/* retour */
	return nb_mines_restant_a_deviner;
}

/*	Renvoie 1 si aucune case minée du jeu n'est dévoilée tandis que toutes les cases non minées du jeu sont dévoilées, 0 sinon */
int demineur_est_gagnee(demineur* d)
{
	int nb_non_minees_devoilees, nb_minees_devoilees, nb_mines_restant_a_deviner;

	/* récupération des statistiques du jeu */
	demineur_get_stat(d, &nb_non_minees_devoilees, &nb_minees_devoilees, &nb_mines_restant_a_deviner);

	/* retour */
	return (nb_non_minees_devoilees == (int)(dimension_get_hauteur(& d->dim)*dimension_get_largeur(& d->dim) -dimension_get_nb_mines(& d->dim)));
}

/*	Renvoie 1 si une case minée du jeu est dévoilée, 0 sinon */
int demineur_est_perdue(demineur* d)
{
	int nb_non_minees_devoilees, nb_minees_devoilees, nb_mines_restant_a_deviner;

	/* récupération des statistiques du jeu */
	demineur_get_stat(d, &nb_non_minees_devoilees, &nb_minees_devoilees, &nb_mines_restant_a_deviner);

	/* retour */
	return (nb_minees_devoilees > 0);
}

/*	Renvoie 1 si la partie est entamée (1 case dévoilée au moins) mais n'est ni gagnée, ni perdue, 0 sinon
	Cette fonction fait appel aux fonctions demineur_est_gagnee et demineur_est_perdue  */
int demineur_est_encours(demineur* d)
{
	int nb_non_minees_devoilees, nb_minees_devoilees, nb_mines_restant_a_deviner;

	/* récupération des statistiques du jeu */
	demineur_get_stat(d, &nb_non_minees_devoilees, &nb_minees_devoilees, &nb_mines_restant_a_deviner);

	/* retour */
	return (nb_non_minees_devoilees > 0 && nb_minees_devoilees == 0 && nb_non_minees_devoilees < (int)(dimension_get_hauteur(& d->dim)*dimension_get_largeur(& d->dim) -dimension_get_nb_mines(& d->dim)));
}

/* ________
	Implémentation des fonctions statiques
*/

/* Sous-routine de la méthode 'demineur_case_devoiler' du modele
	Préconditions : 
	- d adresse d'une variable démineur vérifiant en outre (*d).dim.hauteur <= H_MAX, (*d).dim.largeur <= L_MAX (contraintes techniques)
	- (i,j) indice d'une case non minée et sans mine adjacente du jeu qui vient d'être dévoilée
	Postconditions :
	- toute case non minée du jeu que l'on peut atteindre depuis la case (i,j) par un chemin de cases du jeu qui sont toutes non minées et sans mine adjacente dans le jeu est dévoilée
	Cette fonction récursive fait également appel à la fonction demineur_case_get_nb_mines_adj du modèle */
static void demineur_devoiler_depuis_case(demineur* d, int i, int j)
{
	int iprime, jprime;
	unsigned int h = dimension_get_hauteur(& d->dim);
	unsigned int l = dimension_get_largeur(& d->dim);

	/* parcours des cases du jeu qui sont adjacentes à la case d'indice (i,j) */
	for(iprime = (i-1 >= 0 ? i-1 : 0) ; iprime <= (i+1 <= (int)h -1 ? i+1 : (int)h -1) ; iprime ++)
	{
		for(jprime = (j-1 >= 0 ? j-1 : 0) ; jprime <= (j+1 <= (int)l -1 ? j+1 : (int)l -1) ; jprime ++)
		{
			if (iprime == i && jprime == j)
			{
				continue;
			}

			/* traitement de la case du jeu adjacente d'indice (iprime,jprime), si la case n'est pas déjà dévoilée */
			if (case_est_devoilee(& d->plateau[iprime][jprime]) == BOOLEEN_FAUX)
			{
				/* __ mise à jour de la case */
				case_devoiler(& d->plateau[iprime][jprime]);

				/* __ mise à jour de la partie et du plateau de cases */

				/* ICI : mise à jour champ nb_cases_nonminees_devoilees si ajout d'un champ nb_cases_nonminees_devoilees */

				/* si la case n'est adjacente à aucune mine, on dévoile (récursivement) ses cases adjacentes */
				if(! demineur_case_get_nb_mines_adj(d,iprime,jprime))
				{
					demineur_devoiler_depuis_case(d,iprime,jprime);
				}
			}
		}
	}
}

/* Sous-routine des méthodes 'demineur_get_nb_mines_restant_a_deviner', 'demineur_est_encours', 'demineur_est_gagnee', 'demineur_est_perdue' du modele
	Préconditions : 
	- d adresse d'une variable démineur vérifiant en outre (*d).dim.hauteur <= H_MAX, (*d).dim.largeur <= L_MAX (contraintes techniques)
	- nb_non_minees_devoilees, nb_minees_devoilees, nb_mines_restant_a_deviner adresses de variables entières
	Postconditions :
	- nb_non_minees_devoilees vaut le nombre de cases du jeu qui sont non minées et dévoilées
	- nb_minees_devoilees vaut le nombre de cases du jeu qui sont minées et dévoilées
	- nb_mines_restant_a_deviner vaut le nombre de mines du jeu moins le nombre de cases non dévoilées marquées MARQUE_MINE */
static void demineur_get_stat(demineur* d, int* nb_non_minees_devoilees, int* nb_minees_devoilees, int* nb_mines_restant_a_deviner)
{
	int i,j;

	*nb_non_minees_devoilees=0;
	*nb_minees_devoilees=0;
	*nb_mines_restant_a_deviner=(int)dimension_get_nb_mines(& d->dim);

	/* parcours des cases participant au jeu pour dénombrer les cases dévoilées minées et non minées */
	for (i = 0 ; i < (int)dimension_get_hauteur(& d->dim) ; i ++)
	{
		for (j = 0 ; j < (int)dimension_get_largeur(& d->dim) ; j ++)
		{
			if (case_est_devoilee(& d->plateau[i][j]) == BOOLEEN_VRAI)
			{
				if (case_est_minee(& d->plateau[i][j]) == BOOLEEN_VRAI)
				{
					(*nb_minees_devoilees)++;
				}
				else
				{
					(*nb_non_minees_devoilees)++;
				}
			}
			else if (case_get_marque_joueur(& d->plateau[i][j]) == MARQUE_MINE)
			{
				(*nb_mines_restant_a_deviner)--;
			}
		}
	}
}

