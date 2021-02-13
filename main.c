#include <stdio.h>
#include <stdlib.h>
#include "ctrl_demineur.h"

int main() {
    ctrl_demineur controleur;
    demineur *modele;
debut:
    modele = demineur_construire();
    demineur_nouvelle_partie(modele);
    ctrl_init_dim(&controleur,modele);
    ctrl_lancer();
    ctrl_detruire(&controleur);
    demineur_set_niveau(modele, controleur.vue.niveau);
    ctrl_initialiser(&controleur, modele);
    _start_timer();
    g_timeout_add_seconds(1,afficher_temps,(gpointer) controleur.vue.libelle_tps);
    ctrl_lancer();
    _reset_timer();
    ctrl_detruire(&controleur);
    goto debut;
    ctrl_detruire(&controleur);
    return EXIT_SUCCESS;
}
