#include "vue_demineur.h"
#include <stdlib.h>
#include <stdio.h>
#include <gtk/gtk.h>

int i,j;

void vue_demineur_construire(vue_demineur* vue,int dim)
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
      gtk_widget_set_size_request(vue->boutton[i][j],75,75);
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

GtkWidget* vue_demineur_get_cases(vue_demineur* vue, int i,int j){
  return (GtkWidget*)vue->boutton[i][j];
}

void vue_demineur_set_fenetre_titre(vue_demineur* vue, const char* titre)
{
    #if(DEMINEUR_VUE_DEBUG == 1)
    printf("Entree %s(%p, %s)\n", __func__, (void*)vue, titre);
    #endif

	gtk_window_set_title(vue->fenetre, titre);

    #if(DEMINEUR_VUE_DEBUG == 1)
    printf("Sortie %s()\n", __func__);
    #endif
}

/*int main()
{
  vue_demineur vue;
  int dim=1;
  gtk_init(NULL,NULL);
  //int dim = 1;
  vue_demineur_construire(&vue,dim);
   gtk_main();
  return EXIT_SUCCESS;
}*/
