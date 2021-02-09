/* contrôleur jeu de trésor 
	clic sur bouton => mettre à jour dans la vue le titre de la fenêtre 
	clic sur bouton => ouvrir dans le modèle la boîte correspondante
*/

#ifndef CTRL_demineur_H

#define CTRL_demineur_H

#include "demineur.h"
#include "vue_demineur.h"

/* ___________ Structure de données 
*/

/* Contrôleur trésor : jonction d'un modèle et d'une vue trésor */
struct s_ctrl_cases {
		int hauteur;
		int largeur;
		struct s_ctrl_demineur* parent;
};
typedef struct s_ctrl_cases ctrl_cases; 

struct s_ctrl_demineur{
	demineur* modele;
	vue_demineur vue;
	struct s_ctrl_cases tab[DIM_LONGUEUR][DIM_LARGEUR];
};
typedef struct s_ctrl_demineur ctrl_demineur; 

/* ___________ Fonctions de manipulation 
*/

/* initialise une variable controleur */
void ctrl_initialiser(ctrl_demineur* controleur, demineur* modele);

/* lance l'interface */
void ctrl_lancer();

/* ___________ Fonctions de rappel 
*/

/* cette fonction ouvre une boîte du jeu */
void cb_ouvrir_cases(GtkButton* b, ctrl_cases* ctrl_b);

/* cette fonction met à jour le titre de la fenêtre en fonction de l'état de la partie */
void cb_etat_partie(ctrl_demineur* ctrl);

void afficher_mines_adj(GtkButton* b, ctrl_cases* ctrl_b);

#endif
