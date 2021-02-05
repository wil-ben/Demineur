#include "vue_demineur.h"
#include <stdlib.h>
#include <stdio.h>
#include <gtk/gtk.h>


void vue_demineur_construire(vue_demineur* vue, int dim)
{
  // initialisation de la vue
  int i,j;
  vue->fenetre = (GtkWindow*)gtk_window_new(GTK_WINDOW_TOPLEVEL);
  vue->conteneur_principal = (GtkBox*)gtk_box_new(GTK_ORIENTATION_HORIZONTAL,0);
  vue->conteneur_cases = (GtkBox*)gtk_box_new(GTK_ORIENTATION_HORIZONTAL,0);
  vue->conteneur_donnees = (GtkBox*)gtk_box_new(GTK_ORIENTATION_VERTICAL,0);
  vue->conteneur_menu = (GtkBox*)gtk_box_new(GTK_ORIENTATION_HORIZONTAL,0);
  vue->libelle_menu = (GtkLabel*)gtk_label_new("Menu");
  vue->rejouer =(GtkButton*)gtk_button_new_with_mnemonic("_rejouer");
  vue->quitter =(GtkButton*)gtk_button_new_with_mnemonic("_quitter");
  
  gtk_window_set_title(vue->fenetre, "Démineur");
  gtk_container_add(GTK_CONTAINER(vue->fenetre),GTK_WIDGET(vue->conteneur_principal));  
  

  gtk_box_pack_start(vue->conteneur_principal,GTK_WIDGET(vue->conteneur_cases),TRUE,TRUE,0);
  gtk_box_pack_start(vue->conteneur_principal,GTK_WIDGET(vue->conteneur_donnees),TRUE,TRUE,0);
  gtk_box_pack_start(vue->conteneur_donnees,GTK_WIDGET(vue->libelle_menu),TRUE,TRUE,0);
  gtk_box_pack_start(vue->conteneur_donnees,GTK_WIDGET(vue->conteneur_menu),TRUE,TRUE,0);
  gtk_box_pack_start(vue->conteneur_menu,GTK_WIDGET(vue->rejouer),TRUE,TRUE,0);
  gtk_box_pack_start(vue->conteneur_menu,GTK_WIDGET(vue->quitter),TRUE,TRUE,0);
  

  for ( i = 0; i < DIM_LONGUEUR; i++)
  {
    vue->tab_box[i] = (GtkBox*)gtk_box_new(GTK_ORIENTATION_VERTICAL,0);
    gtk_box_pack_start(vue->conteneur_cases,GTK_WIDGET(vue->tab_box[i]),TRUE,TRUE,0);
  }

  for (i = 0; i <DIM_LONGUEUR ; i++)
  {
    for ( j = 0; j < DIM_LARGEUR; j++)
    {
      vue->drapeau[i][j] =(GtkImage*)gtk_image_new_from_file("littleflag.png");
      vue->boutton[i][j] =(GtkToggleButton*)gtk_toggle_button_new();
      gtk_button_set_image((GtkButton*)vue->boutton[i][j],GTK_WIDGET(vue->drapeau[i][j]));
      gtk_box_pack_start(vue->tab_box[i],GTK_WIDGET(vue->boutton[i][j]),TRUE,TRUE,0);
      
    }
    
  }
  
  
  gtk_widget_show_all(GTK_WIDGET(vue->fenetre));
  g_signal_connect(G_OBJECT(vue->quitter),"clicked",G_CALLBACK(gtk_main_quit),NULL);
  g_signal_connect(G_OBJECT(vue->fenetre), "destroy", G_CALLBACK(gtk_main_quit), NULL);
}

void vue_demineur_detruire(vue_demineur* vue)
{
  
}
/*void button_mark(vue_demineur* vue)
{
  int i=0,j=1;
  vue->drapeau =(GtkImage*)gtk_image_new_from_file("flag.png");
  gtk_button_set_image((GtkButton*)vue->boutton[i][j],GTK_WIDGET(vue->drapeau));

}*/


int main()
{
  vue_demineur vue;
  gtk_init(NULL,NULL);
  int dim = 1;


  vue_demineur_construire(&vue,dim);
  gtk_main();

  return EXIT_SUCCESS;
}