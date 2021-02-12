#include <stdio.h>
#include <stdlib.h>
#include "ctrl_demineur.h"
#include <gtk/gtk.h>

int main() {
    ctrl_demineur controleur;
    controleur.a=0;
    demineur *modele = demineur_construire();
    while(controleur.a!=2){ 
        demineur_nouvelle_partie(modele);
        ctrl_init_dim(&controleur,modele);
        ctrl_lancer();
        ctrl_detruire(&controleur);
        demineur_set_niveau(modele, controleur.vue.niveau);
        ctrl_initialiser(&controleur, modele);
        ctrl_lancer();
        ctrl_detruire(&controleur);
    }
    ctrl_detruire(&controleur);
    return EXIT_SUCCESS;
}
