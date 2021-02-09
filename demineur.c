/* Jeu de démineur : fichier source 
*/

#include "demineur.h"
#include <stdlib.h>	/* pour l'allocation dynamique */
#include <time.h>	/* pour la génération aléatoire */
#include <assert.h>	/* gestion des erreurs */

#define DEMINEUR_TRACE 0	/* constante symbolique pour activer ou désactiver les traces */
#if(DEMINEUR_TRACE == 1)
#include <stdio.h>	/* pour l'affichage des traces */
#endif

#define TEMPS_TRACE 1	/* constante symbolique pour activer ou désactiver les traces */
#if(TEMPS_TRACE == 1)
#include <stdio.h>	/* pour l'affichage des traces */
#endif

/* ________ Déclarations (fonctions statiques)
*/

/* Sous-routine de la méthode demineur_get_niveau() qui renvoie une adresse à laquelle est décrite la dimension du niveau demandé 
	Précondition: niveau >= DEMINEUR_NIVEAU_MIN && niveau <= DEMINEUR_NIVEAU_MAX */
static dimension* demineur_get_niveau(demineur_niveau niveau);

/* Sous-routine de la méthode demineur_nouvelle_partie() qui incrémente le compteur de mines voisines des cases qui se situent au voisinage d'une case donnée 
	Préconditions: d est bien l'adresse d'une variable démineur dans laquelle l'indice (i, j) est valide 
	Postconditions: pour tout indice (i', j') <> (i, j) valide de la forme (i+-1, j+-1), le champ nbmines_vois de la case (i', j') du jeu est incrémenté de 1
*/
static void demineur_maj_voisinage_case_minee(demineur* d, int i, int j);

/* Sous-routine de la méthode demineur_case_devoiler() qui dévoile les cases voisines d'une case donnée, et le fait récursivement pour chaque case qu'elle dévoile qui n'a pas de mine dans son voisinage
	Préconditions : 
	- d est bien l'adresse d'une variable démineur
	- les dimensions (*d).dim.h et (*d).dim.l de cette variable n'excèdent pas H_MAX et L_MAX
	- (i, j) est l'indice d'une case du jeu, non minée, et sans mine adjacente, qui vient d'être dévoilée
	Postconditions :
	- toute case non minée et non dévoilée du jeu que l'on peut atteindre depuis la case (i, j) par un chemin de cases du jeu qui sont toutes non minées, non dévoilée et sans mine adjacente dans le jeu est dévoilée
	- le champ nb_a_devoiler est mis à jour en conséquence */
static void demineur_devoiler_depuis_case(demineur* d, int i, int j);

/* ________ Fonctions de manipulation
*/

/* __ Construction / initialisation / destruction d'un jeu de démineur
*/

/* Renvoie l'adresse d'une variable demineur en cas de succès, NULL en cas d'échec (échec allocation mémoire) */
demineur* demineur_construire() {
	demineur* d = NULL;
	static int prem =1;	/* variable static: la déclaration (et l'initialisation qui l'accompagne) ne vaut que pour la 1è exécution de la fonction ; contrairement aux variables locales usuelles, la variable persiste en mémoire tant que le programme est en cours d'exécution */
	
	#if(DEMINEUR_TRACE == 1)
	printf("%s() IN\n", __func__);
	#endif

	/* __ allocation dynamique */
	d = malloc(sizeof(demineur));
	if (d) {	/* __ initialisation des champs */
		/* dimensions (valeurs par défaut) */
		dimension dim = DIM_DEFAUT;
		dimension_set(& d->dim, dim.hauteur, dim.largeur, dim.nbmines);

		/* initialisation de la partie */
		if(prem) {	/* à la 1è entrée dans la fonction, on initialise la graine aléatoire */
			srand(time(NULL));
			prem =0;
		}

		demineur_nouvelle_partie(d);

		#if(TEMPS_TRACE == 1)
		printf("************ %s : d->debut vaut %p\n", __func__, NULL);
		#endif
	}
	
	#if(DEMINEUR_TRACE == 1)
	printf("%s() OUT : retour %p\n", __func__, (void*)d);
	#endif

	return d;
}

/* Initialise une nouvelle partie de démineur */
void demineur_nouvelle_partie(demineur* d) {
	int i, j, indice, cpt;
	unsigned int nbmines_posees=0, h, l, m;
	
	#if(DEMINEUR_TRACE == 1)
	printf("%s(%p) IN\n", __func__, (void*)d);
	#endif

	/* __ initialisation des cases */

	/* Récupération des dimensions */
	h = dimension_get_hauteur(& d->dim);
	l = dimension_get_largeur(& d->dim);
	m = dimension_get_nbmines(& d->dim);

	/* on vérifie la validité des dimensions courantes du jeu */
	assert(1 <= h && h <= H_MAX && 1 <= l && l <= L_MAX && m <= h*l);
	
	/* Initialisation des cases du jeu */
	for (i = 0 ; i < (int)h ; i ++)
		for (j = 0 ; j < (int)l ; j ++)
			case_initialiser(& d->plateau[i][j]);

	/* __ placement des mines */

	/* tant que toutes les mines n'ont pas été posées */
	while (nbmines_posees < m) {
		/* on tire un indice de case au hasard parmi (h *l -nbmines_posees) cases disponibles */
		indice = rand() % (h * l - nbmines_posees);

		/* on parcourt les cases du jeu en tenant le compte des cases non minées
			on s'arrête quand on arrive à la (indice +1)ème telle case */
		cpt =0;
		for (i = 0 ; i < (int)h ; i ++) {
			for (j = 0 ; j < (int)l; j ++) {
				if (case_est_minee(& d->plateau[i][j]) == BOOLEEN_FAUX) {
					cpt++;

					if (cpt == indice +1)	/* si cpt == indice +1, la case (i, j) est la case recherchee */
						break;
				}
			}

			if (cpt == indice +1)	/* si cpt == indice +1, la case (i, j) est la case recherchee */
				break;
		}

		/* on mine la case et met à jour en conséquence la structure */
		case_miner(& d->plateau[i][j]);
		nbmines_posees ++;
		demineur_maj_voisinage_case_minee(d, i, j);
	}

	/* __ champs atomiques */
	d->nb_a_devoiler =h * l - m;
	d->nb_marquees =0;
	d->etat =PARTIE_INIT;

	#if(TEMPS_TRACE == 1)
	printf("************ %s : d->debut vaut %p\n", __func__, NULL);
	#endif
	
	#if(DEMINEUR_TRACE == 1)
	printf("%s() OUT\n", __func__);
	#endif
}

/* Libère la mémoire éventuellement occupée par une variable demineur, et renvoie NULL */
demineur* demineur_detruire(demineur* d) {
	#if(DEMINEUR_TRACE == 1)
	printf("%s(%p) IN\n", __func__, (void*)d);
	#endif

	if (d != NULL)
		free(d);
	
	#if(DEMINEUR_TRACE == 1)
	printf("%s() OUT retour NULL\n", __func__);
	#endif

	return NULL;
}

/* __ Accesseurs en écriture / lecture : dimension du jeu
*/

/* Redéfinit les dimensions et initialise une nouvelle partie */
int demineur_set_dimensions(demineur* d, unsigned int h, unsigned int l, unsigned int m) {
	int statut =1;
	
	#if(DEMINEUR_TRACE == 1)
	printf("%s(%p, %u, %u, %u) IN\n", __func__, (void*)d, h, l, m);
	#endif
	
	if (1 <= h && h <= H_MAX && 1 <= l && l <= L_MAX && m <= (int)(h*l)) {
		statut =0;
		
		/* le cas échéant, définition du nombre de mines par défaut étant données une hauteur et une largeur */
		if (m <= 0)
			m = NB_MINES_DEFAUT(h, l);

		/* affectation des nouvelles dimensions */
		dimension_set(& d->dim, h, l, m);

		/* initialisation de la partie */
		demineur_nouvelle_partie(d);
	}
	
	#if(DEMINEUR_TRACE == 1)
	printf("%s() OUT retour %d\n", __func__, statut);
	#endif

	/* __ Retour du statut de réalisation de la fonction */
	return statut;
}

/* Réinitialise les dimensions à un niveau prédéfini, initialise une nouvelle partie et renvoie 0 si le niveau est reconnu, renvoie 1 sinon */
int demineur_set_niveau(demineur* d, demineur_niveau niveau) {
	int statut =1;
	
	#if(DEMINEUR_TRACE == 1)
	printf("%s(%p, %d) IN\n", __func__, (void*)d, niveau);
	#endif

	/* vérification du niveau */
	if(niveau >= DEMINEUR_NIVEAU_MIN && niveau <= DEMINEUR_NIVEAU_MAX) {
		statut =0;

		/* récupération puis application du niveau demandé */
		dimension* dim =demineur_get_niveau(niveau);
		demineur_set_dimensions(d, dimension_get_hauteur(dim), dimension_get_largeur(dim), (int)dimension_get_nbmines(dim));
	}
	
	#if(DEMINEUR_TRACE == 1)
	printf("%s() OUT retour %d\n", __func__, statut);
	#endif
	
	return statut;
}

/* Récupère dans des variables passées par adresse la dimension courante du jeu */
void demineur_get_dimensions(demineur* d, unsigned int* h, unsigned int* l, unsigned int* m) {
	*h = dimension_get_hauteur(& d->dim);
	*l = dimension_get_largeur(& d->dim);
	*m = dimension_get_nbmines(& d->dim);
}

/* __ Accesseurs en ecriture / lecture : case
*/

/* Dévoile la case (i, j) du jeu (et en domino toute case non dévoilée du jeu que l'on peut atteindre depuis cette case par un chemin de cases du jeu qui sont toutes non minées et sans mine adjacente dans le jeu) si l'indice est valide, la partie est en cours et la case n'est pas dévoilée et renvoie 0 alors, renvoie 1 sinon */
int demineur_case_devoiler(demineur* d, int i, int j) {
	int retour =1;
	
	#if(DEMINEUR_TRACE == 1)
	printf("%s(%p, %d, %d) IN\n", __func__, (void*)d, i, j);
	#endif

	/* vérification des indices */
	assert(i >= 0 && i < (int)dimension_get_hauteur(& d->dim) && j >= 0 && j < (int)dimension_get_largeur(& d->dim));

	/* __ s'il s'agit de la 1è action de jeu, on met l'heure de début à jour et on lance la partie */
	if(d->etat == PARTIE_INIT) {
		time(& d->debut);
		d->etat = PARTIE_ENCOURS;

		#if(TEMPS_TRACE == 1)
		printf("************ %s 1er coup joué: d->debut vaut %s\n", __func__, ctime(& d->debut));
		#endif
	}
	
	/* on vérifie que la partie est en cours et la case n'est pas déjà dévoilée */
	if(d->etat == PARTIE_ENCOURS && case_est_devoilee(& d->plateau[i][j]) == BOOLEEN_FAUX) {
		retour =0;

		/* _ on dévoile la case, ce qui a pour effet de réinitialiser sa marque */
		if (case_get_marque(& d->plateau[i][j]) == MARQUE_MINE)
			d->nb_marquees --;
		case_devoiler(& d->plateau[i][j]);

		/* _ si la case est minée, la partie est perdue */
		if (case_est_minee(& d->plateau[i][j]) == BOOLEEN_VRAI)
			d->etat =PARTIE_PERDUE;
		else {
			/* _ sinon */

			/* une case non minée de moins reste à dévoiler */
			d->nb_a_devoiler --;

			/* on dévoile automatiquement les cases voisines de cases dévoilées sans mine adjacente */
			if(! demineur_case_get_nbmines_adj(d, i, j))
				demineur_devoiler_depuis_case(d, i, j);

			/* s'il ne reste plus de case non minée à dévoiler, la partie est gagnée */
			if(d->nb_a_devoiler == 0)
				d->etat =PARTIE_GAGNEE;
		}

		/* __ si la partie est finie, on met à jour l'heure de fin */
		if(d->etat != PARTIE_ENCOURS) {
			time(& d->fin);

			#if(TEMPS_TRACE == 1)
			printf("************ %s partie gagnée: d->fin vaut %s\n", __func__, ctime(& d->fin));
			#endif
		}
	}
	
	#if(DEMINEUR_TRACE == 1)
	printf("%s() OUT retour %d\n", __func__, retour);
	#endif
	
	return retour;
}

/* Modifie la marque de la case (i, j) du jeu si l'indice est valide, la partie est en cours et la case n'est pas dévoilée et renvoie 0 alors, renvoie 1 sinon */
int demineur_case_marquer(demineur* d, int i, int j) {
	int retour =1;
	
	#if(DEMINEUR_TRACE == 1)
	printf("%s(%p, %d, %d) IN\n", __func__, (void*)d, i, j);
	#endif

	/* vérification des indices */
	assert(i >= 0 && i < (int)dimension_get_hauteur(& d->dim) && j >= 0 && j < (int)dimension_get_largeur(& d->dim));

	/* __ s'il s'agit de la 1è action de jeu, on met l'heure de début à jour et on lance la partie */
	if(d->etat == PARTIE_INIT) {
		time(& d->debut);
		d->etat = PARTIE_ENCOURS;

		#if(TEMPS_TRACE == 1)
		printf("************ %s 1er coup joué: d->debut vaut %s\n", __func__, ctime(& d->debut));
		#endif
	}

	/* on vérifie que la partie est en cours et la case n'est pas dévoilée */
	if(d->etat == PARTIE_ENCOURS && case_est_devoilee(& d->plateau[i][j]) == BOOLEEN_FAUX) {
		retour =0;

		/* modification de la marque de la case (i, j)
			NB si la marque initiale de la case est invalide, case_marquer() provoque une sortie en erreur du programme */
		case_marquer(& d->plateau[i][j]);

		/* mise à jour éventuelle du nombre de mines marquées MARQUE_MINE */
		if (case_get_marque(& d->plateau[i][j]) == MARQUE_MINE)
			d->nb_marquees ++;
		else if (case_get_marque(& d->plateau[i][j]) == MARQUE_INTERRO)
			d->nb_marquees --;
	}
	
	#if(DEMINEUR_TRACE == 1)
	printf("%s() OUT retour %d\n", __func__, retour);
	#endif
	
	return retour;
}

/* Renvoie 1 si la case (i, j) est minée, 0 sinon */
int demineur_case_est_minee(demineur* d, int i, int j) {
	/* vérification des indices */
	assert(i >= 0 && i < (int)dimension_get_hauteur(& d->dim) && j >= 0 && j < (int)dimension_get_largeur(& d->dim));

	/* retour de l'information demandée */
	return (case_est_minee(& d->plateau[i][j]) == BOOLEEN_VRAI);
}

/* Renvoie 1 si la case (i, j) est dévoilée, 0 sinon */
int demineur_case_est_devoilee(demineur* d, int i, int j) {
	/* vérification des indices */
	assert(i >= 0 && i < (int)dimension_get_hauteur(& d->dim) && j >= 0 && j < (int)dimension_get_largeur(& d->dim));

	/* retour de l'information demandée */
	return (case_est_devoilee(& d->plateau[i][j]) == BOOLEEN_VRAI);
}

/* Renvoie la marque de la case (i, j) */
marque demineur_case_get_marque(demineur* d, int i, int j) {
	/* vérification des indices */
	assert(i >= 0 && i < (int)dimension_get_hauteur(& d->dim) && j >= 0 && j < (int)dimension_get_largeur(& d->dim));

	/* retour de l'information demandée */
	return case_get_marque(& d->plateau[i][j]);
}

/* Renvoie le nombre de mines au voisinage de la case (i, j) (i.e., situées sur des cases du jeu qui lui sont adjacentes) */
unsigned int demineur_case_get_nbmines_adj(demineur* d, int i, int j) {
	/* vérification des indices */
	assert(i >= 0 && i < (int)dimension_get_hauteur(& d->dim) && j >= 0 && j < (int)dimension_get_largeur(& d->dim));

	/* retour de l'information demandée */
	return case_get_nbmines(& d->plateau[i][j]);
}

/* __ Accesseurs en ecriture / lecture : partie
*/

/* Renvoie la différence entre le nombre de mines du jeu, et le nombre de cases non dévoilées du jeu que le joueur a marquées MARQUE_MINE */
unsigned int demineur_get_nbmines_a_deviner(demineur* d) {
	return dimension_get_nbmines(& d->dim) -d->nb_marquees;
}

/* Renvoie le nombre de case restant à dévoiler du jeu */
unsigned int demineur_get_nbcases_a_devoiler(demineur* d) {
	return d->nb_a_devoiler;
}

/* Renvoie l'état de la partie */
etat_partie demineur_get_etat(demineur* d) {
	return d->etat;
}

/* Renvoie le temps écoulé depuis la 1è action de jeu (entre la 1è et la dernière si la partie est finie) */
long unsigned int demineur_get_temps(demineur* d) {
	long unsigned int interval =0;

	if (d->etat == PARTIE_ENCOURS)
		interval =(long unsigned int)difftime(time(NULL), d->debut);
	else if (d->etat == PARTIE_GAGNEE || d->etat == PARTIE_PERDUE)
		interval =(long unsigned int)difftime(d->fin, d->debut);

	#if(TEMPS_TRACE == 1)
	printf("************ %s : interval vaut %lu\n", __func__, interval);
	#endif
	
	return interval;
}

/* ________ Sous-routines (fonctions statiques)
*/

/* Sous-routine de la méthode demineur_nouvelle_partie() qui incrémente le compteur de mines voisines des cases qui se situent au voisinage d'une case donnée */
static void demineur_maj_voisinage_case_minee(demineur* d, int i, int j){ 
	unsigned int iprime, jprime;
	unsigned int h = dimension_get_hauteur(& d->dim);
	unsigned int l = dimension_get_largeur(& d->dim);
	
	#if(DEMINEUR_TRACE == 1)
	printf("%s(%p, %d, %d) IN\n", __func__, (void*)d, i, j);
	#endif

	/* parcours indices valides de {i -1, i, i +1} x {j -1, j, j +1} \ {(i, j)} */
	for(iprime = (i -1 >= 0 ? i -1 : 0) ; iprime <= (i +1 <= (int)h -1 ? i +1 : (int)h -1) ; iprime ++)
		for(jprime = (j-1 >= 0 ? j-1 : 0) ; jprime <= (j+1 <= (int)l -1 ? j+1 : (int)l -1) ; jprime ++) {
			if (iprime == i && jprime == j)	/* on passe l'indice (i, j) */
				continue;

			case_ajouter_mine(& d->plateau[iprime][jprime]);
		}
	
	#if(DEMINEUR_TRACE == 1)
	printf("%s() OUT\n", __func__);
	#endif
}

/* Sous-routine de la méthode demineur_get_niveau() qui renvoie une adresse à laquelle est décrite la dimension du niveau demandé */
static dimension* demineur_get_niveau(demineur_niveau niveau) {
	static dimension niveaux[DEMINEUR_NIVEAU_MAX -DEMINEUR_NIVEAU_MIN +1] ={DIM_DEBUTANT, DIM_MOYEN, DIM_EXPERT};
	
	/* retour de la dimension demandée */
	return niveaux +(niveau -DEMINEUR_NIVEAU_MIN);
}

/* Sous-routine de la méthode demineur_case_devoiler() qui dévoile les cases voisines d'une case donnée, et le fait récursivement pour chaque case qu'elle dévoile qui n'a pas de mine dans son voisinage */
static void demineur_devoiler_depuis_case(demineur* d, int i, int j) {
	int iprime, jprime;
	unsigned int h = dimension_get_hauteur(& d->dim);
	unsigned int l = dimension_get_largeur(& d->dim);
	
	#if(DEMINEUR_TRACE == 1)
	printf("%s(%p, %d, %d) IN\n", __func__, (void*)d, i, j);
	#endif

	/* parcours des cases du jeu qui sont adjacentes à la case d'indice (i, j) */
	for(iprime = (i -1 >= 0 ? i -1 : 0) ; iprime <= (i +1 <= (int)h -1 ? i +1 : (int)h -1) ; iprime ++)
		for(jprime = (j -1 >= 0 ? j -1 : 0) ; jprime <= (j +1 <= (int)l -1 ? j +1 : (int)l -1) ; jprime ++) {
			if (iprime == i && jprime == j)	/* la case (i, j) ne nous intéresse pas */
				continue;

			/* une case (iprime, jprime) ne nous intéresse que si elle n'a pas déjà été dévoilée */
			if (case_est_devoilee(& d->plateau[iprime][jprime]) == BOOLEEN_FAUX) {
				/* _ on dévoile la case, ce qui a pour effet de réinitialiser sa marque */
				if (case_get_marque(& d->plateau[iprime][jprime]) == MARQUE_MINE)
					d->nb_marquees --;
				case_devoiler(& d->plateau[iprime][jprime]);
				d->nb_a_devoiler --;

				/* __ le cas échéant, on dévoile aussi les cases voisines */
				if(demineur_case_get_nbmines_adj(d,iprime,jprime) == 0)
					demineur_devoiler_depuis_case(d,iprime,jprime);
			}
		}
	
	#if(DEMINEUR_TRACE == 1)
	printf("%s() OUT\n", __func__);
	#endif
}
