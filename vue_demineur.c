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
  vue->conteneur_donnees = (GtkBox*)gtk_box_new(GTK_ORIENTATION_HORIZONTAL,0);

  gtk_window_set_title(vue->fenetre, "Démineur");
  gtk_container_add(GTK_CONTAINER(vue->fenetre),GTK_WIDGET(vue->conteneur_principal));  

  gtk_box_pack_start(vue->conteneur_principal,GTK_WIDGET(vue->conteneur_cases),TRUE,TRUE,0);
  gtk_box_pack_start(vue->conteneur_principal,GTK_WIDGET(vue->conteneur_donnees),TRUE,TRUE,0);

  for ( i = 0; i < DIM_LONGUEUR; i++)
  {
    vue->tab_box[i] = (GtkBox*)gtk_box_new(GTK_ORIENTATION_VERTICAL,0);
    gtk_box_pack_start(vue->conteneur_cases,GTK_WIDGET(vue->tab_box[i]),TRUE,TRUE,0);
  }
  
  for (i = 0; i <DIM_LONGUEUR ; i++)
  {
    for ( j = 0; j < DIM_LARGEUR; j++)
    {
      vue->boutton[i][j] =(GtkToggleButton*)gtk_toggle_button_new_with_label(".");
      gtk_box_pack_start(vue->tab_box[i],GTK_WIDGET(vue->boutton[i][j]),TRUE,TRUE,0);
    }
    
  }
  

  gtk_widget_show_all(GTK_WIDGET(vue->fenetre));
  g_signal_connect(G_OBJECT(vue->fenetre), "destroy", G_CALLBACK(gtk_main_quit), NULL);
}

void vue_demineur_detruire(vue_demineur* vue)
{
  
}



int main()
{
  vue_demineur vue;
  gtk_init(NULL,NULL);
  int dim = 1;


  vue_demineur_construire(&vue,dim);
  gtk_main();

  return EXIT_SUCCESS;
}