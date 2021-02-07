/* contrôleur jeu de trésor 
	clic sur bouton => mettre à jour dans la vue le titre de la fenêtre 
	clic sur bouton => ouvrir dans le modèle la boîte correspondante
*/

#ifndef CTRL_DEMINEUR_H

#define CTRL_DEMINEUR_H

#include "demineur.h"
#include "vue_demineur.h"

/* ___________ Structure de données 
*/
typedef struct ctrl_demineur{
	int mines[DIM_LONGUEUR][DIM_LARGEUR];
}