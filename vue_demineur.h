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
#include "demineur.h"

#ifndef VUE_DEMINEUR_H
#define VUE_DEMINEUR_H

#define DEMINEUR_VUE_DEBUG 1	/* mode de compilation */



// Structure de donnée, voire le fichiers vue_demineur.c pour plus de précision sur les variables
typedef struct s_vue_demineur
{
  demineur_niveau niveau;
  GtkWindow* fenetre;
  GtkBox* conteneur_principal;
  GtkBox* conteneur_cases;
  GtkBox* conteneur_donnees;
  GtkToggleButton* boutton[H_MAX][L_MAX];	/* pour i = 0 ,..., hauteur-1 et j = 0 ,..., largeur -1, la case d'indice ij du tableau cases modélise la case de la ligne n°i+1 et de la colonne n°j+1 du jeu  */
  GtkBox* conteneur_menu;
  GtkButton* rejouer;
  GtkButton* quitter;
  GtkBox* tab_box[H_MAX];
  GtkLabel* libelle_menu;
  GtkLabel* libelle_tps;
  GtkSwitch* play_stop;
  GtkImage* drapeau[H_MAX][L_MAX];
  GtkButton* choose_nv[3];
  GtkLabel* info_niv[3];
  GtkLabel* marques;
  GtkBox* conteneur_tps;
  GtkBox* conteneur_evol;
  GtkImage* im_menu;
}vue_demineur;


/*-------------------------------------------- declarations de fonction */


//Voire le fichier vue_demineur.c pour plus de précision sur ces fonctions
void vue_demineur_construire(vue_demineur* vue,demineur*);

void vue_demineur_montrer(vue_demineur*,gboolean);

void vue_demineur_set_fenetre_titre(vue_demineur* vue, const char* titre);

void vue_demineur_detruire(vue_demineur* vue);

GtkWidget* vue_demineur_get_cases(vue_demineur* vue, int i,int j);

void sortir();

void vue_ask_niveau(vue_demineur *vue);

void select_nv(GtkButton* b,vue_demineur *vue);

#endif
