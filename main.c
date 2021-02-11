#include <stdio.h>
#include <stdlib.h>
#include "ctrl_demineur.h"
#include <gtk/gtk.h>

int main() {
    ctrl_demineur controleur;
    demineur *modele = demineur_construire();
    demineur_nouvelle_partie(modele);
    ctrl_init_dim(&controleur,modele);
    ctrl_lancer();
    demineur_set_niveau(modele, controleur.vue.niveau);
    ctrl_initialiser(&controleur, modele);
    ctrl_lancer();
    return EXIT_SUCCESS;
}

