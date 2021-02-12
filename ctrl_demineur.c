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
	
	vue_demineur_construire(& controleur->vue, modele);
	vue_demineur_montrer(& controleur->vue, TRUE);

	/* contrôleur "niveau boîte" */
	for(i =0 ; i < modele->dim.hauteur ; i ++) {
        for ( j = 0; j < modele->dim.largeur; j++){
		controleur->tab[i][j].hauteur =i;
        controleur->tab[i][j].largeur =j;
		controleur->tab[i][j].parent =controleur;
        }
	}

	/* attachements */
	for(i =0 ; i < modele->dim.hauteur ; i ++) {
        for (j = 0; j < modele->dim.largeur; j++){
            /* code */
        	widget = vue_demineur_get_cases(& controleur->vue, i,j);
			g_signal_connect(G_OBJECT(widget), "clicked", G_CALLBACK(cb_ouvrir_cases), & (controleur->tab[i][j]));
      		g_signal_connect(G_OBJECT(widget), "button-press-event", G_CALLBACK(marquer_cases), & (controleur->tab[i][j]));
			g_signal_connect_swapped(G_OBJECT(widget), "clicked", G_CALLBACK(cb_etat_partie), controleur);
			g_signal_connect(G_OBJECT(controleur->vue.rejouer),"clicked",G_CALLBACK(gtk_main_quit),NULL);
			g_signal_connect(G_OBJECT(controleur->vue.quitter),"clicked",G_CALLBACK(quitter),NULL);
			//g_signal_connect(G_OBJECT(controleur->vue.libelle_tps),"unclicked",G_CALLBACK(afficher_temps),controleur->modele);
		
	}
}
//afficher_temps( controleur->vue.libelle_tps,controleur->modele,);
}

void ctrl_init_dim(ctrl_demineur* controleur, demineur* modele){
	gtk_init(NULL, NULL);
	vue_ask_niveau(&(controleur->vue));
	for (int i = 0; i < 3; i++){
      g_signal_connect(G_OBJECT(controleur->vue.choose_nv[i]),"clicked",G_CALLBACK(select_nv),&(controleur->vue));
    } 
}
/* lance l'interface */
void ctrl_lancer() {
	gtk_main();
}
void ctrl_detruire(ctrl_demineur* controleur){
	vue_demineur_detruire(&controleur->vue);
}

/* ___________ Fonctions de rappel 
*/

/* cette fonction met à jour le titre de la fenêtre en fonction de l'état de la partie */
void cb_etat_partie(ctrl_demineur* ctrl) {
	etat_partie etat = demineur_get_etat((*ctrl).modele);
	
	switch(etat) {
		case PARTIE_ENCOURS: 
			vue_demineur_set_fenetre_titre(& (*ctrl).vue, "Partie en cours");
			gtk_label_set_label(ctrl->vue.libelle_menu,"PARTIE EN COURS");
			break;
		case PARTIE_GAGNEE: 
			vue_demineur_set_fenetre_titre(& (*ctrl).vue, "Partie gagnee, bravo !");
			gtk_label_set_label(ctrl->vue.libelle_menu,"PARTIE GAGNÉE, BRAVO !");
			break;
		case PARTIE_PERDUE: 
			vue_demineur_set_fenetre_titre(& (*ctrl).vue, "Partie perdue ... dommage :(())");
			gtk_label_set_label(ctrl->vue.libelle_menu,"PARTIE PERDUE ... DOMMAGE :(");
			break;
		default:
			printf("cas non reconnu\n");
	}
}

/* cette fonction ouvre une boîte du jeu */
void cb_ouvrir_cases(GtkButton* b, ctrl_cases* ctrl_b) {
	afficher_temps( ctrl_b->parent->vue.libelle_tps,ctrl_b->parent->modele);
	demineur_case_devoiler(ctrl_b->parent->modele, ctrl_b->hauteur, ctrl_b->largeur);
	if(demineur_case_est_minee(ctrl_b->parent->modele,ctrl_b->hauteur,ctrl_b->largeur)){
		afficher_mines(b,ctrl_b);
	}else{
		afficher_mines_adj((GtkButton*)b,ctrl_b);
	}
	
}

void afficher_mines_adj(GtkButton* b, ctrl_cases* ctrl_b){
	int i = demineur_case_get_nbmines_adj(ctrl_b->parent->modele,ctrl_b->hauteur,ctrl_b->largeur);
	char label[0];
	label[0]= (char)i+48;
	if(i==0){
		gtk_button_set_label((GtkButton*)b," ");
	}else{
	gtk_button_set_label((GtkButton*)b,label);

	}
	for(int f = 0; f < ctrl_b->parent->modele->dim.hauteur ; f ++) {
        for(int g = 0; g < ctrl_b->parent->modele->dim.largeur; g++){
			int w= case_est_devoilee(&ctrl_b->parent->modele->plateau[f][g]);
			if(w == 1){
            	gtk_toggle_button_set_active ((GtkToggleButton *)vue_demineur_get_cases(&ctrl_b->parent->vue,f,g), TRUE);
            }
		}
	}
}

void afficher_mines(GtkButton* b, ctrl_cases* ctrl_b){
	gtk_button_set_label(b,"💣");
	for(int f = 0; f < ctrl_b->parent->modele->dim.hauteur ; f ++) {
        for(int g = 0; g < ctrl_b->parent->modele->dim.largeur; g++){
            	gtk_toggle_button_set_active ((GtkToggleButton *)vue_demineur_get_cases(&ctrl_b->parent->vue,f,g), TRUE);
		}
	}
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

void quitter(GtkButton* b,ctrl_cases* ctrl_b){
    //ctrl_b->parent->a=2;
	exit(0);
	gtk_main_quit();
}

void afficher_temps( GtkLabel* b,demineur* d)
{
 	int x = d->debut;
	int y = time(&d->fin)-x;
	char tab_tps[100000];
	sprintf(tab_tps,"%d",y);
	gtk_label_set_label(b,tab_tps);
}