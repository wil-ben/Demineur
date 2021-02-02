/*
	dimension d'un jeu de démineur : programme test
*/

#include "dimension.h"
#include <stdlib.h>
#include <stdio.h>

/* ________________________________ Programme de test
*/

int main() {
	dimension d = {2,4,5};
	int res = 0;
	
	/* Test 1 : accesseurs en lecture */
	printf("\nTest 1 accesseurs en lecture :\n");
	printf("\td.hauteur:%u == dimension_get_hauteur(&d):%u ?\n", d.hauteur, dimension_get_hauteur(&d));
	printf("\td.largeur:%u == dimension_get_largeur(&d):%u ?\n", d.largeur, dimension_get_largeur(&d));
	printf("\td.nbmines:%u == dimension_get_nbmines(&d):%u ?\n", d.nbmines, dimension_get_nbmines(&d));

	/* Test 2 : accesseur en ecriture */
	printf("\nTest 2 accesseur en lecture :\n");

	printf("\tAppel dimension_set sur d.hauteur = %u, d.largeur = %u, (d.hauteur*d.largeur) +1 = %u : nombre de mine KO => dimensions inchangées ? :\n", d.hauteur, d.largeur, (d.hauteur*d.largeur) +1);
	res = dimension_set(&d, d.hauteur, d.largeur, (d.hauteur*d.largeur) +1);
	printf("\tres = %d, (d.hauteur, d.largeur, d.nbmines) == (%u,%u,%u)\n", res, d.hauteur, d.largeur, d.nbmines);

	printf("\n\tAppel dimension_set sur d.hauteur+1 = %u, d.largeur+1 = %u, d.nbmines+1 = %u : nombre de mine OK => dimensions changées ?\n", d.hauteur +1, d.largeur +1, d.nbmines+1);
	res = dimension_set(&d, d.hauteur +1, d.largeur +1, d.nbmines +1);
	printf("\tres = %d, (d.hauteur, d.largeur, d.nbmines) == (%u,%u,%u)\n", res, d.hauteur, d.largeur, d.nbmines);

	/* Fin du programme de test */
	return EXIT_SUCCESS;
}
