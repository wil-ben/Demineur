#include "vue_demineur.h"
#include <stdlib.h>
#include <stdio.h>
#include <gtk/gtk.h>

int i,j;

void vue_demineur_construire(vue_demineur* vue)
{
  
  // initialisation de la vue
  int i,j;
  // Creation de la fenêtre
  vue->fenetre = (GtkWindow*)gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title(vue->fenetre, "Démineur");  
  
  // Mise en place du container principal
  vue->conteneur_principal = (GtkBox*)gtk_box_new(GTK_ORIENTATION_HORIZONTAL,0);
  gtk_container_add(GTK_CONTAINER(vue->fenetre),GTK_WIDGET(vue->conteneur_principal));  
  
   // Container qui va contenir les cases
  vue->conteneur_cases = (GtkBox*)gtk_box_new(GTK_ORIENTATION_HORIZONTAL,0);
  gtk_box_pack_start(vue->conteneur_principal,GTK_WIDGET(vue->conteneur_cases),TRUE,TRUE,0);
  
  // Container qui va contenir le menu ainsi que les boutons d'actions Quitter et rejouer
  vue->conteneur_donnees = (GtkBox*)gtk_box_new(GTK_ORIENTATION_VERTICAL,0);
  gtk_box_pack_start(vue->conteneur_principal,GTK_WIDGET(vue->conteneur_donnees),TRUE,TRUE,0);

  // Container qui va contenir le label Menu
  vue->libelle_menu = (GtkLabel*)gtk_label_new("Menu");
  gtk_box_pack_start(vue->conteneur_donnees,GTK_WIDGET(vue->libelle_menu),TRUE,TRUE,0);

  // Container qui va contenir les deux bouttons d'ctions Quitter et Rejouer
  vue->conteneur_menu = (GtkBox*)gtk_box_new(GTK_ORIENTATION_HORIZONTAL,0);
  gtk_box_pack_start(vue->conteneur_donnees,GTK_WIDGET(vue->conteneur_menu),TRUE,TRUE,0);

  // Container principal pour le boutton rejouer
  vue->rejouer =(GtkButton*)gtk_button_new_with_mnemonic("_rejouer");
  gtk_box_pack_start(vue->conteneur_menu,GTK_WIDGET(vue->rejouer),TRUE,TRUE,0);
  
  // Container principal pour le boutton quitter
  vue->quitter =(GtkButton*)gtk_button_new_with_mnemonic("_quitter");
  gtk_box_pack_start(vue->conteneur_menu,GTK_WIDGET(vue->quitter),TRUE,TRUE,0);


  for ( i = 0; i < DIM_LONGUEUR; i++)
  {
    //création de 9 box verticales pour mettre les bouttons
    vue->tab_box[i] = (GtkBox*)gtk_box_new(GTK_ORIENTATION_VERTICAL,0);
    gtk_box_pack_start(vue->conteneur_cases,GTK_WIDGET(vue->tab_box[i]),TRUE,TRUE,0);
  }

  for (i = 0; i <DIM_LONGUEUR ; i++)
  {
    for ( j = 0; j < DIM_LARGEUR; j++)
    {
      //remplissage des boutons à la verticale dans les boîtes
      vue->boutton[i][j] =(GtkToggleButton*)gtk_toggle_button_new();
      gtk_box_pack_start(vue->tab_box[i],GTK_WIDGET(vue->boutton[i][j]),TRUE,TRUE,0);
     
      //détection du clic droit pour chaque bouton
      gtk_widget_set_events((GtkWidget*)vue->boutton[i][j], GDK_BUTTON_PRESS_MASK);
      g_signal_connect(G_OBJECT(vue->boutton[i][j]), "button-press-event", G_CALLBACK(fonctionTest), NULL);
    }
    
  }
  gtk_widget_show_all(GTK_WIDGET(vue->fenetre));
  g_signal_connect(G_OBJECT(vue->quitter),"clicked",G_CALLBACK(gtk_main_quit),NULL);
  g_signal_connect(G_OBJECT(vue->fenetre), "destroy", G_CALLBACK(gtk_main_quit), NULL);
}


gboolean fonctionTest(GtkWidget *widget,GdkEvent * unionCompliquee, gpointer data){
 /* Variables */
    guint typeClic=unionCompliquee->button.button ; // récupération du type de clic à partir de l'argument de la fonction
    if( typeClic ==3){  /* Cas du clic droit */
      gtk_button_set_image((GtkButton*)widget,GTK_WIDGET((GtkImage*)gtk_image_new_from_file("littleflag.png")));
    
    }else{// clic gauche (typeClic == 1)
      gtk_toggle_button_set_active ((GtkToggleButton *)widget, TRUE);
    }
    return TRUE;
    
}

void vue_demineur_detruire(vue_demineur* vue)
{
  
}


int main()
{
  vue_demineur vue;
  gtk_init(NULL,NULL);
  //int dim = 1;
  vue_demineur_construire(&vue);
   gtk_main();
  return EXIT_SUCCESS;
}
