 /******************************************************************************

 *vue_demineur.h
 *
 * Vue pour le jeu demineur : fichier d'entête

	Fonctions de manipulation du type vue_tresor :

		Construction / destruction :
		- void vue_tresor_construire(vue_tresor* vue, int dim);
		- void vue_tresor_detruire(vue_tresor* vue);

		Accesseurs aux composants de la vue
		- int vue_demineur_get_dim(vue_tresor* vue);
		- GtkWindow* vue_demineur_get_fenetre(vue_tresor* vue);
		- GtkButton* vue_demineur_get_rejouer(vue_tresor* vue);
		- GtkButton* vue_demineur_get_quitter(vue_tresor* vue);
		- GtkButton* vue_demineur_get_boite(vue_tresor* vue, int i);

		Modification de l'apparence de la vue
		- void vue_tresor_set_fenetre_titre(vue_tresor* vue, const char* titre);
		- void vue_tresor_set_boite_libelle(vue_tresor* vue, int i, const char* texte);
		- void vue_tresor_set_boite_relief(vue_tresor* vue, int i, int oui_non);
 *
 * cree le     : 19/01/2020
 * par         : Nicolas LE MOINE <necko.lm@gmail.com>
 *
 * modifie le  :
 * par         :
 * description :  
 ******************************************************************************/
#ifndef VUE_DEMINEUR__H
#define VUE_DEMINEUR__H

#define DEMINEUR_VUE_DEBUG 1	/* mode de compilation */

#include <gtk/gtk.h> /* on inclut la biliothèque d'objets graphiques	*/

#define DIM_LONGUEUR 8 /* vs. jeu de tresor : pour NB_BOITE	 */
#define DIM_LARGEUR 8 /* vs. jeu de tresor : pour NB_BOITE	 */


/* ___________________________________
	Structure de données				*/

typedef struct s_vue_demineur
{
    GtkWindow* fenetre;
    GtkBox* conteneur_principal;
    GtkBox* conteneur_cases;
    GtkBox* conteneur_donnees;
    GtkButton* cases[DIM_LONGUEUR][DIM_LARGEUR];	/* pour i = 0 ,..., DIM_LONGUEUR -1 et j = 0 ,..., DIM_LARGEUR -1, la case d'indice ij du tableau cases modélise la case de la ligne n°i+1 et de la colonne n°j+1 du jeu  */
    GtkBox* conteneur_menu;
    GtkButton* rejouer;
    GtkButton* quitter;
} vue_tresor;


/*-------------------------------------------- declarations de type */


/*---------------------------------------------------- constructeur */

void vue_tresor_construire(vue_tresor* vue, int dim);
void vue_tresor_detruire(vue_tresor* vue);

/*----------------------------------------------------- destructeur */

/*------------------------------------------------------ accesseurs */

/*------------------------------------------------------ modifieurs */


#endif