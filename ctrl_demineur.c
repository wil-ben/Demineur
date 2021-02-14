#include "ctrl_demineur.h"

/*Varibales globales pour le timer* voire les fonctions concernées pour plus de précision sur ces variables*/
static int secs =0;//variable qui représente les secondes
static gboolean start_timer = FALSE;//permet de maitriser le lancement et l'arrêt du timer
int a=0;//permet d'arreter le timer tout en continuant à appeler la fonction lorsqu'il est mis en pause
int b=1;//permet de continuer d'appeler le timer après le premeir appel car le timer se déclenche lorsqu'on clique sur une case

int marques=0;//permet de mettre à jour le commpteur de marques

/* initialise une variable controleur */
void ctrl_initialiser(ctrl_demineur* controleur, demineur* modele) {
	int i;
    int j;
	GtkWidget* widget;

	/* modele */
	controleur->modele =modele;
	secs=0;
	b=1;
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
			//les boutons s'enfoncent lorsqu'on passe la souris dessus
			g_signal_emit_by_name(widget,"pressed",0);
			//devoile les cases lorsqu'on effectue un clique gauche
			g_signal_connect(G_OBJECT(widget), "clicked", G_CALLBACK(cb_ouvrir_cases), & (controleur->tab[i][j]));
			//marque les cases lorqu'on effectue un clique droit
      		g_signal_connect(G_OBJECT(widget), "button-press-event", G_CALLBACK(marquer_cases), & (controleur->tab[i][j]));
			 //renseigne sur l'état de la aprtie
			g_signal_connect_swapped(G_OBJECT(widget), "clicked", G_CALLBACK(cb_etat_partie), controleur);
			//appelle la fonction qui met en pause le timer lorsqu'on active le bouton "switch"
			g_signal_connect(G_OBJECT(controleur->vue.play_stop), "state-set", G_CALLBACK(on_off), controleur);
			//Sort du programme lorsqu'on appuie sur quitter
			g_signal_connect(G_OBJECT(controleur->vue.quitter),"clicked",G_CALLBACK(quitter),NULL);
	}
}
}

void ctrl_init_dim(ctrl_demineur* controleur, demineur* modele){
	gtk_init(NULL, NULL);
	vue_ask_niveau(&(controleur->vue));
	for (int i = 0; i < 3; i++){
		//le joueur selectionne le niveau
      g_signal_connect(G_OBJECT(controleur->vue.choose_nv[i]),"clicked",G_CALLBACK(select_nv),&controleur->vue);
	  // la fenetre est detruite lorsqu'on chosit un niveau
      g_signal_connect(G_OBJECT(controleur->vue.choose_nv[i]),"clicked",G_CALLBACK(gtk_window_close),&controleur->vue.fenetre);
    } 
}
/* lance l'interface */
void ctrl_lancer() {
	gtk_main();
}
void ctrl_detruire(ctrl_demineur* controleur){
	vue_demineur_detruire(&controleur->vue);
	//demineur_detruire(controleur->modele);
}

/* ___________ Fonctions de rappel 
*/

/* cette fonction met à jour le titre de la fenêtre et le label du menu en fonction de l'état de la partie */
void cb_etat_partie(ctrl_demineur* ctrl) {
	etat_partie etat = demineur_get_etat((*ctrl).modele);
	char label[NB_MAX];
	switch(etat) {
		case PARTIE_ENCOURS: 
			vue_demineur_set_fenetre_titre(& (*ctrl).vue, "Partie en cours");
			gtk_label_set_label(ctrl->vue.libelle_menu,"PARTIE EN COURS");
			_start_timer();//lance le timer lorsque la partie commence 
			b++;
			sprintf(label,"%d ⬜",b-1); // converti un int en char*
			gtk_label_set_label(ctrl->vue.info_niv[1],label);//affiche le nb de cases dévoilées
			break;
		case PARTIE_GAGNEE: 
			vue_demineur_set_fenetre_titre(& (*ctrl).vue, "Partie gagnee, bravo !");
			gtk_label_set_label(ctrl->vue.libelle_menu,"PARTIE GAGNÉE, BRAVO !");
			_reset_timer();//reset le timer lorsque la partie se termine
			break;
		case PARTIE_PERDUE: 
			vue_demineur_set_fenetre_titre(& (*ctrl).vue, "Partie perdue ... dommage :(())");
			gtk_label_set_label(ctrl->vue.libelle_menu,"PARTIE PERDUE ... DOMMAGE :(");
			_reset_timer();//reset le timer lorsque la partie se termine
			break;
		default:
			printf("cas non reconnu\n");
	}
}

/* cette fonction dévoile les cases du plateau */
void cb_ouvrir_cases(GtkButton* b, ctrl_cases* ctrl_b) {
	demineur_case_devoiler(ctrl_b->parent->modele, ctrl_b->hauteur, ctrl_b->largeur);
	if(demineur_case_est_minee(ctrl_b->parent->modele,ctrl_b->hauteur,ctrl_b->largeur)){
		afficher_mines(b,ctrl_b);//affiche la mine
	}else{
		afficher_mines_adj((GtkButton*)b,ctrl_b);//affiche le nb de mines adjacentes
	}
	
}

void afficher_mines_adj(GtkButton* b, ctrl_cases* ctrl_b){
	int i = demineur_case_get_nbmines_adj(ctrl_b->parent->modele,ctrl_b->hauteur,ctrl_b->largeur);
	char label[0];
	label[0]= (char)i+48;
	if(i==0){
		gtk_button_set_label((GtkButton*)b," "); //affiche le nb mines a proximité " " veut dire qu'il n'y a pas de mines
	}else{
		gtk_button_set_label((GtkButton*)b,label);//affiche le nb de mines à proximité

	}
	//Affiche les cases adjacentes lorsuq'il n'ya pas de mines à proximité
	for(int f = 0; f < ctrl_b->parent->modele->dim.hauteur ; f ++) {
        for(int g = 0; g < ctrl_b->parent->modele->dim.largeur; g++){
			int w = case_est_devoilee(&ctrl_b->parent->modele->plateau[f][g]);
			if(w == 1){
            	gtk_toggle_button_set_active ((GtkToggleButton *)vue_demineur_get_cases(&ctrl_b->parent->vue,f,g), TRUE);
            }
		}
	}
}

void afficher_mines(GtkButton* b, ctrl_cases* ctrl_b){
	//affiche une bombe lorsque la case est minée
	gtk_button_set_label(b,"💣");

	//Dévoile toutes les cases du plateau lorsque qu'une mine à été dévoilée
	for(int f = 0; f < ctrl_b->parent->modele->dim.hauteur ; f ++) {
        for(int g = 0; g < ctrl_b->parent->modele->dim.largeur; g++){
            	gtk_toggle_button_set_active ((GtkToggleButton *)vue_demineur_get_cases(&ctrl_b->parent->vue,f,g), TRUE);
		}
	}
}


gboolean marquer_cases(GtkWidget *widget,GdkEvent * unionCompliquee, ctrl_cases* ctrl_b){
 /* Variables */
	char label[NB_MAX];
    guint typeClic=unionCompliquee->button.button ; // récupération du type de clic à partir de l'argument de la fonction
	int i = demineur_case_get_marque(ctrl_b->parent->modele,ctrl_b->hauteur,ctrl_b->largeur);
	int j = demineur_case_marquer(ctrl_b->parent->modele,ctrl_b->hauteur,ctrl_b->largeur);
    if( typeClic == 3){  /* Cas du clic droit */
		if(j == 0 && i == MARQUE_AUCUNE){	//affiche un drapeau s'il n'y a pas de marques
			gtk_button_set_label((GtkButton*)widget,"🚩");
			marques++;
			sprintf(label,"%d 🚩",marques);
			gtk_label_set_label(ctrl_b->parent->vue.marques,label); // affiche le nb de marques posées
		}
		else if(j == 0 && i == MARQUE_MINE){
			gtk_button_set_label((GtkButton*)widget,"?");//affiche un ? si la case à déjà été marquée
			marques--;
			sprintf(label,"%d 🚩",marques);
			gtk_label_set_label(ctrl_b->parent->vue.marques,label);// enlève la marque retiré du compteur du nb de marque
		}
		else if(j == 0 && i == MARQUE_INTERRO){
			gtk_button_set_label((GtkButton*)widget," ");//enlève le ?
		}
	}else{// clic gauche (typeClic == 1)
    	gtk_toggle_button_set_active ((GtkToggleButton *)widget, TRUE);//enfonce les boutons lors d'un clique gauche
    }
    return TRUE;
    
}

//sort du programme lorsqu'on appuie sur quitter
void quitter(){
	exit(0);
	gtk_main_quit();
}

//affiche le temps si start_timer vaut TRUE, si il vaut FALSE le temps s'arrête.
gboolean afficher_temps(gpointer data)
{
	if(start_timer == TRUE){
		char label[NB_MAX];
		int x=secs/60;//converti les secondes en minutes
		int z=secs%60;//converti les secondes en secondes
		sprintf(label,"Temps : %d min et %d secs",x,z);//converti un int ou plusieurs en char*
		secs++;
		gtk_label_set_label((GtkLabel*)data,label);
	}else {
		if(a==1) // permet de continuer d'appeler la fonction même quand le timer est en pause car si la fonciton renvoie FALSE elle ne sera plus appelée
		return TRUE;
		if(b==1) //permet de continuer d'appeler la fonction après le premier appel pour les mêmes raisons, puisque que start_timer vaut FALSE avant qu'on clique sur une case
		return TRUE;
		return FALSE;// la fonction n'est plus appelé quand la partie se termine
	}
	return TRUE;
}

//déclenche le timer
void _start_timer (){
	start_timer=TRUE;
}

//arrête le timer ; pour des raisons pratiques les variables globales sont mises a jour dans cette fonction
void _reset_timer (){
	start_timer=FALSE;
	a=0;
	b=0;
	marques=0;
}
//Cette fonction arrête le timer lorqu'on active le boutton "switch"
gboolean on_off(GtkSwitch* sw,ctrl_demineur* controleur){
	int i = gtk_switch_get_state(sw);
	if (i == FALSE && start_timer== TRUE){
		start_timer=FALSE; //arrêt du timer

		//affiche un effet de flou pour dire que le jeu est en pause
		GtkWindow* window =(GtkWindow*)gtk_window_new(GTK_WINDOW_TOPLEVEL);
		gtk_window_set_decorated(window,FALSE);//enlève les bords de la fenre
		gtk_window_set_attached_to(window,(GtkWidget*)sw);//rattache la fenetre au bouton "switch"
		GtkLabel* label = (GtkLabel*)gtk_label_new("PAUSE");//affiche le label PAUSE
		gtk_container_add(GTK_CONTAINER(window),GTK_WIDGET(label));
		gtk_widget_show_all((GtkWidget*)window);//affiche la fenetre

		a=1;//a est mis à 1 pour pas que la fonction afficher_temps retourne FALSE
		return FALSE;
	}else if(i==TRUE && start_timer == FALSE){
		_start_timer(); //reprise du timer
		return FALSE;
	}
	return FALSE;
}

//cette fonction fait la même chose que le main, elle est appelée losrqu'on clique sur rejouer
void replay (){
	ctrl_demineur controleur;
    demineur *modele;

	//remet le timer à 0
	_reset_timer();

	modele = demineur_construire();
    demineur_nouvelle_partie(modele);

	//Demande le niveau de la partie au joueurs
    ctrl_init_dim(&controleur,modele);
	//gtk_main
    ctrl_lancer();
    ctrl_detruire(&controleur);

	//ajuste les dimensions en fonction du boutons
    demineur_set_niveau(modele, controleur.vue.niveau);
    ctrl_initialiser(&controleur, modele);

	//appelle la fonction afficher temps toutes les secondes si elle renvoie TRUE
    g_timeout_add_seconds(1,afficher_temps,(gpointer) controleur.vue.libelle_tps);

	//relance le jeu quand on clique sur "Rejouer"
	g_signal_connect(G_OBJECT(controleur.vue.rejouer),"clicked",G_CALLBACK(gtk_window_close),controleur.vue.fenetre);;
    g_signal_connect(G_OBJECT(controleur.vue.fenetre),"destroy",G_CALLBACK(replay),NULL);

	//gtk_main
    ctrl_lancer();
    ctrl_detruire(&controleur);
}