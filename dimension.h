/* Dimension d'un jeu de démineur

	Un jeu de dimensions est caractérisé par
	- une hauteur (>= 0)
	- une largeur (>= 0)
	- un nombre de mines (>= 0) 

	Contraintes d'intégrité :
	- nombre de mines <= hauteur * largeur
*/

#ifndef DIMENSION_H

#define DIMENSION_H

/* ________ Structures de données
*/

struct s_dimension {
	unsigned int hauteur;
	unsigned int largeur;
	unsigned int nbmines;	/* nbmines <= largeur * hauteur */
};

typedef struct s_dimension dimension;

/* ________ Fonctions de manipulation
		Pour toutes: d est supposée être l'adresse mémoire d'une variable dimension (mémoire allouée)
*/

/* __ Accesseurs en ecriture / lecture
*/

/* Initialise les composantes de la dimension aux valeurs transmises en paramètre si celles-ci sont valides (i.e., si m <= h*l) et renvoie 0 alors, renvoie 1 sinon
	Postcondition si m <= h*l: *d vaut {h, l, m} */
int dimension_set(dimension* d, unsigned int h, unsigned int l, unsigned int m);

/* Renvoie la hauteur */
unsigned int dimension_get_hauteur(dimension* d);

/* Renvoie la largeur */
unsigned int dimension_get_largeur(dimension* d);

/* Renvoie le nombre de mines */
unsigned int dimension_get_nbmines(dimension* d);

#endif
