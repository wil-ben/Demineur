

#ifndef CTRL_demineur_H

#define CTRL_demineur_H
#define NB_MAX 10000
#include "demineur.h"
#include "vue_demineur.h"

/*Structure de données */

/* Contrôleur démineur : jonction d'un modèle et d'une vue démineur */
struct s_ctrl_cases {
	int hauteur;
	int largeur;
	struct s_ctrl_demineur* parent;
};
typedef struct s_ctrl_cases ctrl_cases; 

struct s_ctrl_demineur{
	demineur* modele;
	vue_demineur vue;
	struct s_ctrl_cases tab[H_MAX][L_MAX];
};
typedef struct s_ctrl_demineur ctrl_demineur; 

/*Fonctions de manipulation */

/* initialise une variable controleur */
void ctrl_initialiser(ctrl_demineur* controleur, demineur* modele);

/*initialise les dimensions en fonction du choix du joueur*/
void ctrl_init_dim(ctrl_demineur* controleur, demineur* modele);


/* lance l'interface */
void ctrl_lancer();

/*fonction de destruction*/
void ctrl_detruire(ctrl_demineur* controleur);

/*Fonctions de rappel */

gboolean on_off(GtkSwitch *sw, ctrl_demineur *controleur);

/* cette fonction dévoile les cases du plateau */
void cb_ouvrir_cases(GtkButton* b, ctrl_cases* ctrl_b);

/* cette fonction met à jour le titre de la fenêtre en fonction de l'état de la partie */
void cb_etat_partie(ctrl_demineur* ctrl);

/*cette fonction affiche le nbr de mines étants voisines d'une case donnée*/
void afficher_mines_adj(GtkButton* b, ctrl_cases* ctrl_b);

/*cette fonction affiche la mine le cas échéant*/
void afficher_mines(GtkButton*b, ctrl_cases* ctrl_b);

/*cette fonction entraine la fermeture du jeu*/
void quitter();

/*cette fonction marque la case avec la marque adéquate*/
gboolean marquer_cases(GtkWidget *widget,GdkEvent * unionCompliquee, ctrl_cases *ctrl_d);

/*cette fonction affiche le temps ecoulé depuis le début de la partie*/
gboolean afficher_temps(gpointer data);
gboolean afficher_temps2(gpointer data);

void _start_timer ();
void _reset_timer();

#endif
