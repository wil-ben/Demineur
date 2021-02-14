#include "vue_demineur.h"
#include <stdlib.h>
#include <stdio.h>
#include <gtk/gtk.h>

void vue_demineur_construire(vue_demineur* vue,demineur* d)
{
  
  // initialisation de la vue
  int i,j;
  // Creation de la fenêtre
  vue->fenetre = (GtkWindow*)gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title(vue->fenetre, "Démineur");  
  gtk_window_fullscreen (vue->fenetre);
  
  // Mise en place du container donnees
  vue->conteneur_principal = (GtkBox*)gtk_box_new(GTK_ORIENTATION_HORIZONTAL,0);
  gtk_container_add(GTK_CONTAINER(vue->fenetre),GTK_WIDGET(vue->conteneur_principal));  
  
   // Container qui va contenir les cases
  vue->conteneur_cases = (GtkBox*)gtk_box_new(GTK_ORIENTATION_HORIZONTAL,0);
  gtk_box_pack_start(vue->conteneur_principal,GTK_WIDGET(vue->conteneur_cases),TRUE,TRUE,0);
  
  // Container qui va contenir le menu ainsi que les boutons d'actions Quitter et rejouer
  vue->conteneur_donnees = (GtkBox*)gtk_box_new(GTK_ORIENTATION_VERTICAL,0);
  gtk_box_pack_start(vue->conteneur_principal,GTK_WIDGET(vue->conteneur_donnees),TRUE,TRUE,0);

  // Container qui va contenir le label Menu
  vue->libelle_menu = (GtkLabel*)gtk_label_new("CLIQUEZ POUR JOUER AU DÉMINEUR !!");
  gtk_box_pack_start(vue->conteneur_donnees,GTK_WIDGET(vue->libelle_menu),TRUE,TRUE,0);

  vue->conteneur_tps = (GtkBox*)gtk_box_new(GTK_ORIENTATION_VERTICAL,0);
  gtk_box_pack_start(vue->conteneur_donnees,GTK_WIDGET(vue->conteneur_tps),TRUE,TRUE,0);

//Container qui va contenir l'avancement du jeu (nb de mines, nb de marques posées et nb de cases dévoilées)
  vue->conteneur_evol = (GtkBox*)gtk_box_new(GTK_ORIENTATION_HORIZONTAL,0);
  gtk_box_pack_start(vue->conteneur_donnees,GTK_WIDGET(vue->conteneur_evol),TRUE,TRUE,0);

  //Libelle réprésentant le nb de marques posées
  vue->marques = (GtkLabel*)gtk_label_new("0 🚩");
  gtk_box_pack_start(vue->conteneur_evol,GTK_WIDGET(vue->marques),TRUE,TRUE,0);

//libelle représentant le nb de mines en fonctions du niveau choisi
  if(vue->niveau == 0)
  vue->info_niv[0]=(GtkLabel*)gtk_label_new("10 💣");
  if(vue->niveau == 1)
  vue->info_niv[0]=(GtkLabel*)gtk_label_new("40 💣");
  if(vue->niveau == 2)
  vue->info_niv[0]=(GtkLabel*)gtk_label_new("99 💣");
  gtk_box_pack_start(vue->conteneur_evol,GTK_WIDGET(vue->info_niv[0]),TRUE,TRUE,0);

  //Libelle représentant le nb de cases dévoilées
  vue->info_niv[1]=(GtkLabel*)gtk_label_new("0 ⬜");
  gtk_box_pack_start(vue->conteneur_evol,GTK_WIDGET(vue->info_niv[1]),TRUE,TRUE,0);

  //Bouton qui sert à arrêter ou reprendre le timer
  vue->play_stop = (GtkSwitch*)gtk_switch_new();
  gtk_box_pack_start(vue->conteneur_tps,GTK_WIDGET(vue->play_stop),TRUE,TRUE,0);

  //Libelle qui affiche le temps
  vue->libelle_tps = (GtkLabel*)gtk_label_new("Temps : 0 min et 0 secs");
  gtk_box_pack_start(vue->conteneur_tps,GTK_WIDGET(vue->libelle_tps),TRUE,TRUE,0);

  // Container qui va contenir les deux bouttons d'ctions Quitter et Rejouer
  vue->conteneur_menu = (GtkBox*)gtk_box_new(GTK_ORIENTATION_HORIZONTAL,0);
  gtk_box_pack_start(vue->conteneur_donnees,GTK_WIDGET(vue->conteneur_menu),TRUE,TRUE,0);

  // Container donnees pour le boutton rejouer
  vue->rejouer =(GtkButton*)gtk_button_new_with_mnemonic("Rejouer");
  gtk_box_pack_start(vue->conteneur_menu,GTK_WIDGET(vue->rejouer),TRUE,TRUE,0);
  
  // Container donnees pour le boutton quitter
  vue->quitter =(GtkButton*)gtk_button_new_with_mnemonic("Quitter");
  gtk_box_pack_start(vue->conteneur_menu,GTK_WIDGET(vue->quitter),TRUE,TRUE,0);


  for ( i = 0; i < d->dim.hauteur; i++)
  {
    //création de 9 box verticales pour mettre les bouttons
    vue->tab_box[i] = (GtkBox*)gtk_box_new(GTK_ORIENTATION_VERTICAL,0);
    gtk_box_pack_start(vue->conteneur_cases,GTK_WIDGET(vue->tab_box[i]),TRUE,TRUE,0);
  }

  for (i = 0; i < d->dim.hauteur ; i++)
  {
    for ( j = 0; j < d->dim.largeur; j++)
    {
      //remplissage des boutons à la verticale dans les boîtes
      vue->boutton[i][j] =(GtkToggleButton*)gtk_toggle_button_new_with_label("    ");
      gtk_widget_set_size_request((GtkWidget*)vue->boutton[i][j],75-d->dim.largeur-10,75-d->dim.largeur-10);
      gtk_button_set_relief ((GtkButton*)vue->boutton[i][j], GTK_RELIEF_NORMAL);
      gtk_box_pack_start(vue->tab_box[i],GTK_WIDGET(vue->boutton[i][j]),TRUE,TRUE,0);
     
    }
    
  }
  g_signal_connect(G_OBJECT(vue->fenetre), "destroy", G_CALLBACK(gtk_main_quit), NULL);
  gtk_widget_show_all(GTK_WIDGET(vue->fenetre));
}

//détruit le vue
void vue_demineur_detruire(vue_demineur* vue)
{
	gtk_widget_destroy(GTK_WIDGET(vue->fenetre));
} 

//montre la vue
void vue_demineur_montrer(vue_demineur* vue,gboolean oui_non){
  #if(DEMINEUR_VUE_DEBUG == 1)
    printf("Entree %s(%p, %d)\n", __func__, (void*)vue, oui_non);
    #endif

	if(oui_non == TRUE)
	{
		gtk_widget_show_all(GTK_WIDGET(vue->fenetre));
	}
	else
	{
		gtk_widget_hide(GTK_WIDGET(vue->fenetre));
	}

    #if(DEMINEUR_VUE_DEBUG == 1)
    printf("Sortie %s()\n", __func__);
    #endif
}

//retourne la case au coordonéées passées en argument
GtkWidget* vue_demineur_get_cases(vue_demineur* vue, int i,int j){
  return (GtkWidget*)vue->boutton[i][j];
}

//met à jour le titre de la fenetre
void vue_demineur_set_fenetre_titre(vue_demineur* vue, const char* titre)
{
    #if(DEMINEUR_VUE_DEBUG == 1)
    #endif

	gtk_window_set_title(vue->fenetre, titre);

    #if(DEMINEUR_VUE_DEBUG == 1)
    #endif
}

//Créer une fenetre afin que le joueur puisse choisir son niveau
void vue_ask_niveau(vue_demineur* vue){
  
  vue->fenetre = (GtkWindow*)gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title(vue->fenetre, "Démineur");  
  gtk_window_fullscreen (vue->fenetre);

  // Mise en place du container donnees
  vue->conteneur_principal = (GtkBox*)gtk_box_new(GTK_ORIENTATION_HORIZONTAL,0);
  gtk_container_add(GTK_CONTAINER(vue->fenetre),GTK_WIDGET(vue->conteneur_principal));  

//Contient le logo démineur
  vue->conteneur_donnees = (GtkBox*)gtk_box_new(GTK_ORIENTATION_VERTICAL,0);
  gtk_box_pack_start(vue->conteneur_principal,GTK_WIDGET(vue->conteneur_donnees),TRUE,TRUE,0);

  vue->im_menu = (GtkImage*)gtk_image_new_from_file("demineur.png");
  gtk_box_pack_start(vue->conteneur_donnees,GTK_WIDGET(vue->im_menu),TRUE,TRUE,0);

//contient les informations sur les différents niveaux
  vue->conteneur_cases = (GtkBox*)gtk_box_new(GTK_ORIENTATION_HORIZONTAL,0);
  gtk_box_pack_start(vue->conteneur_donnees,GTK_WIDGET(vue->conteneur_cases),TRUE,TRUE,0);

  vue->info_niv[0]=(GtkLabel*)gtk_label_new("81 Cases  / 10 Mines");
  vue->info_niv[1]=(GtkLabel*)gtk_label_new("256 Cases / 40 Mines");
  vue->info_niv[2]=(GtkLabel*)gtk_label_new("480 Cases / 99 Mines");
  gtk_box_pack_start(vue->conteneur_cases,GTK_WIDGET(vue->info_niv[0]),TRUE,TRUE,0);
  gtk_box_pack_start(vue->conteneur_cases,GTK_WIDGET(vue->info_niv[1]),TRUE,TRUE,0);
  gtk_box_pack_start(vue->conteneur_cases,GTK_WIDGET(vue->info_niv[2]),TRUE,TRUE,0);

  
  //Contient les boutons afin de sélectionner les niveaux
  vue->conteneur_menu = (GtkBox*)gtk_box_new(GTK_ORIENTATION_HORIZONTAL,0);
  gtk_box_pack_start(vue->conteneur_donnees,GTK_WIDGET(vue->conteneur_menu),TRUE,TRUE,0);

//Création des bouttons
  vue->choose_nv[0]=(GtkButton*)gtk_button_new_with_mnemonic("Niveau débutant");
  vue->choose_nv[1]=(GtkButton*)gtk_button_new_with_mnemonic("Niveau intermédiare");
  vue->choose_nv[2]=(GtkButton*)gtk_button_new_with_mnemonic("Niveau expert");
  //Place les boutons dans le conteneur menu
  gtk_box_pack_start(vue->conteneur_menu,GTK_WIDGET(vue->choose_nv[0]),TRUE,TRUE,0);
  gtk_box_pack_start(vue->conteneur_menu,GTK_WIDGET(vue->choose_nv[1]),TRUE,TRUE,0);
  gtk_box_pack_start(vue->conteneur_menu,GTK_WIDGET(vue->choose_nv[2]),TRUE,TRUE,0);
  g_signal_connect(G_OBJECT(vue->fenetre), "destroy", G_CALLBACK(gtk_main_quit), NULL);
  gtk_widget_show_all(GTK_WIDGET(vue->fenetre));
}

//Fonction qui détermine le niveau en fonction du boutton cliqué
void select_nv(GtkButton* b,vue_demineur* vue){
    if(b==vue->choose_nv[0]){
      vue->niveau = DEMINEUR_NIVEAU_DEBUTANT;
    }else if(b==vue->choose_nv[1]){
      vue->niveau = DEMINEUR_NIVEAU_MOYEN;
    }else if(b==vue->choose_nv[2]){
      vue->niveau = DEMINEUR_NIVEAU_EXPERT;
    }
}