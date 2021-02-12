#include "ctrl_demineur.h"

/* ___________ Fonctions de manipulation */
int secs =0;

/* Determines if the timer has started */
static gboolean start_timer = FALSE;
/* initialise une variable controleur */
void ctrl_initialiser(ctrl_demineur* controleur, demineur* modele) {
	int i;
    int j;
	GtkWidget* widget;

	/* modele */
	controleur->modele =modele;
	controleur->vue.timer =0;
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
			g_signal_emit_by_name(widget,"pressed",0);
			//g_signal_connect(G_OBJECT(widget), "clicked", G_CALLBACK(_start_timer),NULL);
			g_signal_connect(G_OBJECT(widget), "clicked", G_CALLBACK(cb_ouvrir_cases), & (controleur->tab[i][j]));
      		g_signal_connect(G_OBJECT(widget), "button-press-event", G_CALLBACK(marquer_cases), & (controleur->tab[i][j]));
			g_signal_connect_swapped(G_OBJECT(widget), "clicked", G_CALLBACK(cb_etat_partie), controleur);
			g_signal_connect(G_OBJECT(controleur->vue.rejouer),"clicked",G_CALLBACK(gtk_main_quit),NULL);
			g_signal_connect(G_OBJECT(controleur->vue.quitter),"clicked",G_CALLBACK(quitter),NULL);
		
	}
}
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
			_reset_timer();
			break;
		case PARTIE_PERDUE: 
			vue_demineur_set_fenetre_titre(& (*ctrl).vue, "Partie perdue ... dommage :(())");
			gtk_label_set_label(ctrl->vue.libelle_menu,"PARTIE PERDUE ... DOMMAGE :(");
			_reset_timer();
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
		_start_timer();
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
	exit(0);
	gtk_main_quit();
}

gboolean afficher_temps(gpointer data)
{
	if(start_timer == 1){
		GtkLabel *label =(GtkLabel*)data;
		int x,z;
		char tab_tps[10000];
		x=secs/60;
		z=secs%60;
		sprintf(tab_tps,"Temps : %d min et %d secs",x,z);
		secs=secs+1;
		gtk_label_set_label(label,tab_tps);
	}else{
		secs=0;
	}
	return TRUE;
}

void _start_timer (){
	start_timer=TRUE;
}

void _reset_timer (){
	start_timer=FALSE;

}