 /******************************************************************************

 *vue_demineur.h
 *
 * Vue pour le jeu demineur : fichier d'entête

	Fonctions de manipulation du type vue_demineur :

		Construction / destruction :
		- void vue_demineur_construire(vue_demineur* vue, int dim);
		- void vue_demineur_detruire(vue_demineur* vue);

		Accesseurs aux composants de la vue
		- int vue_demineur_get_dim(vue_demineur* vue);
		- GtkWindow* vue_demineur_get_fenetre(vue_demineur* vue);
		- GtkButton* vue_demineur_get_rejouer(vue_demineur* vue);
		- GtkButton* vue_demineur_get_quitter(vue_demineur* vue);
		- GtkButton* vue_demineur_get_(vue_demineur* vue, int i);

		Modification de l'apparence de la vue
		- void vue_demineur_set_fenetre_titre(vue_demineur* vue, const char* titre);
		- void vue_demineur_set__libelle(vue_demineur* vue, int i, const char* texte);
		- void vue_demineur_set__relief(vue_demineur* vue, int i, int oui_non);
 *
 * cree le     : 19/01/2020
 * par         : Nicolas LE MOINE <necko.lm@gmail.com>
 *
 * modifie le  :
 * par         :
 * description :  
 ******************************************************************************/
#include <gtk/gtk.h>

#ifndef VUE_DEMINEUR_H
#define VUE_DEMINEUR_H

#define DEMINEUR_VUE_DEBUG 1	/* mode de compilation */


#define DIM_LONGUEUR 9 /* vs. jeu de demineur : pour NB_	 */
#define DIM_LARGEUR 9 /* vs. jeu de demineur : pour NB_	 */
#define NB_CASES DIM_LONGUEUR*DIM_LARGEUR

/* ___________________________________
	Structure de données				*/

typedef struct s_vue_demineur
{
  int a;
  GtkWindow* fenetre;
  GtkBox* conteneur_principal;
  GtkBox* conteneur_cases;
  GtkBox* conteneur_donnees;
  GtkToggleButton* boutton[DIM_LONGUEUR][DIM_LARGEUR];	/* pour i = 0 ,..., DIM_LONGUEUR -1 et j = 0 ,..., DIM_LARGEUR -1, la case d'indice ij du tableau cases modélise la case de la ligne n°i+1 et de la colonne n°j+1 du jeu  */
  GtkBox* conteneur_menu;
  GtkButton* rejouer;
  GtkButton* quitter;
  GtkBox* tab_box[DIM_LONGUEUR];
  GtkLabel* libelle_menu;
  GtkImage* drapeau[DIM_LONGUEUR][DIM_LARGEUR];
}vue_demineur;


/*-------------------------------------------- declarations de type */


/*---------------------------------------------------- constructeur */
gboolean fonctionTest(GtkWidget *widget, GdkEvent *unionCompliquee, gpointer);
void vue_demineur_construire(vue_demineur* vue,int);
void vue_demineur_detruire(vue_demineur* vue);
void vue_demineur_montrer(vue_demineur*,gboolean);
GtkWidget* vue_demineur_get_cases(vue_demineur* vue, int i,int j);
void vue_demineur_set_fenetre_titre(vue_demineur* vue, const char* titre);
/*----------------------------------------------------- destructeur */

/*------------------------------------------------------ accesseurs */

/*------------------------------------------------------ modifieurs */

#endif
