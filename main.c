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
    printf("%d\n",controleur.vue.niveau);
    printf("%d\n",modele->dim.hauteur);
    ctrl_initialiser(&controleur, modele);
    ctrl_lancer();
    return EXIT_SUCCESS;
}

