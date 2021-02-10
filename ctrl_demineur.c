#include "ctrl_demineur.h"

/* ___________ Fonctions de manipulation 
*/
/* initialise une variable controleur */
void ctrl_initialiser(ctrl_demineur* controleur, demineur* modele) {
	int i;
    int j;
	GtkWidget* widget;

	/* modele */
	controleur->modele =modele;

	/* vue */
	gtk_init(NULL, NULL);
	vue_demineur_construire(& controleur->vue, NB_CASES);
	vue_demineur_montrer(& controleur->vue, TRUE);

	/* contrôleur "niveau boîte" */
	for(i =0 ; i < DIM_LONGUEUR ; i ++) {
        for ( j = 0; j < DIM_LARGEUR; j++)
        {
		controleur->tab[i][j].hauteur =i;
        controleur->tab[i][j].largeur =j;
		controleur->tab[i][j].parent =controleur;
        }
	}

	/* attachements */
	for(i =0 ; i < DIM_LONGUEUR ; i ++) {
        for (j = 0; j < DIM_LARGEUR; j++)
        {
            /* code */
        	widget = vue_demineur_get_cases(& controleur->vue, i,j);
			g_signal_connect(G_OBJECT(widget), "clicked", G_CALLBACK(cb_ouvrir_cases), & (controleur->tab[i][j]));
      		g_signal_connect(G_OBJECT(widget), "button-press-event", G_CALLBACK(marquer_cases), & (controleur->tab[i][j]));
			g_signal_connect_swapped(G_OBJECT(widget), "clicked", G_CALLBACK(cb_etat_partie), controleur);
			/*if(demineur_case_est_devoilee(controleur->modele,i,j)== 1){
				printf("hello\n");
				gtk_toggle_button_set_active ((GtkToggleButton *)widget, TRUE);
				*/
			}
		}	
	}
}

/* lance l'interface */
void ctrl_lancer() {
	gtk_main();
}

/* ___________ Fonctions de rappel 
*/

/* cette fonction met à jour le titre de la fenêtre en fonction de l'état de la partie */
void cb_etat_partie(ctrl_demineur* ctrl) {
	etat_partie etat = demineur_get_etat((*ctrl).modele);
	
	switch(etat) {
		case PARTIE_ENCOURS: 
			vue_demineur_set_fenetre_titre(& (*ctrl).vue, "Partie en cours");
			break;
		case PARTIE_GAGNEE: 
			vue_demineur_set_fenetre_titre(& (*ctrl).vue, "Partie gagnee, bravo !");
			break;
		case PARTIE_PERDUE: 
			vue_demineur_set_fenetre_titre(& (*ctrl).vue, "Partie perdue ... dommage :(())");
			break;
		default:
			printf("cas non reconnu\n");
	}
}

/* cette fonction ouvre une boîte du jeu */
void cb_ouvrir_cases(GtkButton* b, ctrl_cases* ctrl_b) {
	demineur_case_devoiler(ctrl_b->parent->modele, ctrl_b->hauteur, ctrl_b->largeur);
	if(demineur_case_est_minee(ctrl_b->parent->modele,ctrl_b->hauteur,ctrl_b->largeur)){
		afficher_mines(b,ctrl_b);
	}else{
		afficher_mines_adj((GtkButton*)b,ctrl_b);
	}
}

void afficher_mines_adj(GtkButton* b, ctrl_cases* ctrl_b){
	int i = demineur_case_get_nbmines_adj(ctrl_b->parent->modele,ctrl_b->hauteur,ctrl_b->largeur);
	char label= i+48;
	gtk_button_set_label((GtkButton*)b,&label);
}

void afficher_mines(GtkButton* b, ctrl_cases* ctrl_b){
	gtk_button_set_label(b,"💣");
}

gboolean marquer_cases(GtkWidget *widget,GdkEvent * unionCompliquee, ctrl_cases* ctrl_b){
 /* Variables */
    guint typeClic=unionCompliquee->button.button ; // récupération du type de clic à partir de l'argument de la fonction
	int i = demineur_case_get_marque(ctrl_b->parent->modele,ctrl_b->hauteur,ctrl_b->largeur);
	int j = demineur_case_marquer(ctrl_b->parent->modele,ctrl_b->hauteur,ctrl_b->largeur);
    if( typeClic == 3){  /* Cas du clic droit */
		if(j == 0 && i == MARQUE_AUCUNE){	
			gtk_button_set_label((GtkButton*)widget,"🚩");
		}
		else if(j == 0 && i == MARQUE_MINE){
			gtk_button_set_label((GtkButton*)widget,"?");
		}
		else if(j == 0 && i == MARQUE_INTERRO){
			gtk_button_set_label((GtkButton*)widget," ");
		}
	}else{// clic gauche (typeClic == 1)
    	gtk_toggle_button_set_active ((GtkToggleButton *)widget, TRUE);
    }
    return TRUE;
    
}