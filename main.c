#include <stdio.h>
#include <stdlib.h>
#include "ctrl_demineur.h"

int main() {
   ctrl_demineur controleur;
    demineur *modele;

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
    return EXIT_SUCCESS;
}