<<<<<<< HEAD
/******************************************************************************
 * vue_demineur.c
 *
 * Création de la vue du jeu Demineur.
 *
 * cree le     : 19/01/2020
 * par         : Nicolas LE MOINE <necko.lm@gmail.com>
 *
 * Compilation : gcc vue_demineur.c -Wall `pkg-config gtk+-3.0 --cflags --libs`
 *
 * modifie le  :
 * par         :
 * description :  
 ******************************************************************************/

#include <stdio.h>
#include <math.h>
#include <gtk/gtk.h>
#include "vue_demineur.h"

/*---------------------------------------------------- constructeur */



/*----------------------------------------------------- destructeur */


=======
#include "vue_demineur.h"
#include <stdlib.h>
#include <stdio.h>
#include <gtk/gtk.h>

void vue_tresor_construire(vue_tresor* vue, int dim)
{
  vue->fenetre = (GtkWindow*)gtk_window_new(GTK_WINDOW_TOPLEVEL);
  vue->conteneur_principal = (GtkBox*)gtk_box_new(GTK_ORIENTATION_HORIZONTAL,0);
  vue->conteneur_cases = (GtkBox*)gtk_box_new(GTK_ORIENTATION_HORIZONTAL,0);
  vue->conteneur_donnees = (GtkBox*)gtk_box_new(GTK_ORIENTATION_HORIZONTAL,0);
  gtk_container_add(GTK_CONTAINER(vue->fenetre),GTK_WIDGET(vue->conteneur_principal));

  gtk_box_pack_start(vue->conteneur_principal,GTK_WIDGET(vue->conteneur_cases),TRUE,TRUE,0);
  gtk_box_pack_start(vue->conteneur_principal,GTK_WIDGET(vue->conteneur_donnees),TRUE,TRUE,0);

  gtk_widget_show_all(GTK_WIDGET(vue->fenetre));
  g_signal_connect(G_OBJECT(vue->fenetre), "destroy", G_CALLBACK(gtk_main_quit), NULL);

}

int main()
{
  vue_tresor vue;
  gtk_init(NULL,NULL);
  int dim = 1;


  vue_tresor_construire(&vue,dim);
  gtk_main();

  return EXIT_SUCCESS;
}
>>>>>>> a619cb6031e5ab60f5856f59f0304f0bf84198f3
