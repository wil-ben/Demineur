/* Dimension d'un jeu de démineur : implémentation des fonctions de manipulation
*/

#include "dimension.h"
#include <stdio.h>	/* pour printf() */

/* __ Accesseurs en ecriture / lecture */

/* Initialise les composantes de la dimension aux valeurs transmises en paramètre si celles-ci sont valides (i.e., si m <= h*l) et renvoie 0 alors, renvoie 1 sinon */
int dimension_set(dimension* d, unsigned int h, unsigned int l, unsigned int m) {
	int res = 1;

	if (m <= l*h) {
		res = 0;

		d->hauteur = h;
		d->largeur = l;
		d->nbmines = m;
	}

	return res;
}

/* Renvoie la hauteur */
unsigned int dimension_get_hauteur(dimension* d) {
	return d->hauteur;
}

/* Renvoie la largeur */
unsigned int dimension_get_largeur(dimension* d) {
	return d->largeur;
}

/* Renvoie le nombre de mines */
unsigned int dimension_get_nbmines(dimension* d) {
	return d->nbmines;
}
