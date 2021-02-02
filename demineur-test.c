/* Jeu de démineur : programme test
*/

#include "demineur-test.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>	/* gestion des erreurs : allocation mémoire */
#include <time.h>

/* ________________________________
	Fonctions statiques
*/

/* Mode d'emploi lecture de la grille affichée par demineur_afficher() */
static void demineur_afficher_emploi();

/* Affichage détaillé d'une variable demineur 
	Cette fonction procède *par accès directs aux champs des structures* 
		(afin de ne pas introduire de biais lié au fonctionnement des fonctions de manipulation)
	Précondition: d est réellement l'adresse d'une variable demineur (mémoire allouée), aux dimensions valides */
static void demineur_afficher(demineur* d);

/* Mine, dévoile, marque comme minées, attribue 9 mines voisines à toutes les cases du jeu, 
		et met en cohérence les champs atomiques de la structure 
	Cette fonction procède *par accès directs aux champs des structures* 
		(afin de ne pas introduire de biais lié au fonctionnement des fonctions de manipulation)
	Précondition: d est réellement l'adresse d'une variable demineur (mémoire allouée) */
static void demineur_set_init(demineur* d);

/* Scenario de test : redimensionnement par niveau prédéfini
	Préconditions: d est réellement l'adresse d'une variable demineur (mémoire allouée) de dimensions niveau1 */
static void demineur_scenario_niveaux(demineur* d, demineur_niveau niveau1, demineur_niveau niveau2);

/* Scenario de test : redimensionnement manuel 
	Précondition: d est réellement l'adresse d'une variable demineur (mémoire allouée) */
static void demineur_scenario_dimensions(demineur* d, unsigned int h, unsigned int l, unsigned int m);

/* Scenario de test : marquer case 
	Préconditions: d est réellement l'adresse d'une variable demineur (mémoire allouée) fraichement réinitialisée */
static void demineur_scenario_marquer(demineur* d, int pas, int devoile);

/* Scenario de test : dévoiler systématiquement
	- toutes les cases du jeu si mines =1
	- toutes les cases non minées du jeu sinon 
	depuis une partie 
	- fraichement initialisée sur laquelle on marque 1 case sur 3 marquée minée, pê minée, non marquée
	Préconditions: d est réellement l'adresse d'une variable demineur (mémoire allouée) aux dimensions valides */
static void demineur_scenario_devoiler(demineur* d, int mines);

/* Affichage détaillé d'une variable demineur 
	Cette fonction procède *par utilisation systématique des fonctions de manipulation* 
		(afin de tester celles-ci)
	Précondition: d est réellement l'adresse d'une variable demineur (mémoire allouée), aux dimensions valides */
static void demineur_afficher_get(demineur* d);

/*	Fonction permettant de vider le buffer (pour sécuriser les saisies, et assurer les pauses par getchar()) */
static void purger_buffer();

/* ________________________________
	Programme test
*/

/* ____ Programme test */

/* Fonction principale */
int main(int argc, char* argv[]) {
	int choix;

	/* __ Vérification arguments du main */
	if (argc != 2)
		printf("\nNombre d'arguments incorrect. Syntaxe attendue '%s num' où 'num' est un entier de %d à %d\n", argv[0], TEST_MIN, TEST_MAX);
	else if (sscanf(argv[1], "%d", &choix) != 1)
		printf("\nArgument %s incorrect. L'argument attendu est un entier de %d à %d\n", argv[1], TEST_MIN, TEST_MAX);
	else if (choix < TEST_MIN || choix > TEST_MAX)
		printf("\nNuméro de test %d non reconnu car hors des bornes [%d,c%d] proposées\n", choix, TEST_MIN, TEST_MAX);
	else
		demineur_test(choix);

	/* __ retour */
	return EXIT_SUCCESS;
}
	
/* Déroulement d'une fonction de test spécifique */
void demineur_test(demineur_test_choix choix) {
	assert(choix >= TEST_MIN && choix <= TEST_MAX);
	
	switch(choix) {
		case TEST_CONSTRUCTION:
			/* Construction, ré-initialisation, destruction */
			printf("\n________ Test %d :  Construction, re-initialisation, destruction\n", choix);
			demineur_test_init();
			break;

		case TEST_REDIM_AUTO:
			/* Redimensionnement (dont ré-initialisation) automatique */
			printf("\n________ Test %d :  Redimensionnement (incluant reinitialisation) en utilisant des jeus de dimensions predefinis\n", choix);
			demineur_test_niveaux();
			break;

		case TEST_REDIM_MANUEL:
			/* Redimensionnement (dont ré-initialisation) manuel */
			printf("\n________ Test %d :  Redimensionnement (incluant reinitialisation) manuel\n", choix);
			demineur_test_dimensions_manuelles();
			break;

		case TEST_CASE_MARQUER:
			/* Action de jeu marquer case sur case fonctionnellement correcte */
			printf("\n________ Test %d :  Action de jeu marquer case sur cases fonctionnellement correctes\n", choix);
			demineur_test_marquer_case();
			break;

		case TEST_CASE_DEVOILER:
			/* Action de jeu dévoiler case sur case fonctionnellement correcte */
			printf("\n________ Test %d :  Action de jeu devoiler case sur cases fonctionnellement correctes\n", choix);
			demineur_test_devoiler_case();
			break;

		case TEST_GET:
			/* Accesseurs en lecture à la structure et ses sous-structures */
			printf("\n________ Test %d :  Accesseurs en lecture\n", choix);
			demineur_test_get();
			break;

		default:
			printf("\n________ Choix de test %d incompris.\n", choix);
			break;
	}
}

/* ________________________________
	Définition des fonctions de test
*/

/* Construction / destruction / ré-initialisation */
void demineur_test_init() {
	demineur* d;
	printf("____ Modele demineur :: fonction de test %s. Lecture des affichages à venir:\n", __func__);
	demineur_afficher_emploi();

	/* __ construction => initialisation partie niveau débutant */
	printf("\n__ 1. demineur_construire() : verifier hauteur & largeur, nombre de mines, initialisation des cases (dont mines voisines), etat, nombres de cases à dévoiler / marquées minées\n");
	d = demineur_construire();
	assert(d != NULL);

	demineur_afficher(d);
	getchar();	/* pause */

	/* __ réinitialisation */
	printf("\n__ 2. demineur_nouvelle_partie() *après* avoir rendu les cases toutes minées, dévoilées, marquées mines, avec 9 mines voisines (affichage #1) : verifier nombre de mines posées, initialisation des cases (dont mines voisines), nombres de cases à dévoiler / marquées minées, etat (affichage #2)\n");
	demineur_set_init(d);
	demineur_afficher(d);
	demineur_nouvelle_partie(d);
	demineur_afficher(d);
	getchar();	/* pause */

	/* __ destruction */
	printf("\n__ 3. demineur_detruire():\n");
	printf("\n__ 3.1. demineur_detruire(d) où d adresse d'une variable démineur: vérifier l'absence de fuite mémoire (exécution avec valgrind)\n");
	d =demineur_detruire(d);

	printf("\n__ 3.2. demineur_detruire(NULL): cet appel ne doit pas provoquer de sortie en erreur\n");
	demineur_detruire(d);
}

/* Redimensionnement (dont ré-initialisation) pour des jeux de dimension prédéfinis */
void demineur_test_niveaux() {
	demineur* d;
	demineur_niveau niveau1, niveau2;

	printf("____ Modele demineur :: fonction de test %s. Lecture des affichages à venir:\n", __func__);
	demineur_afficher_emploi();

	/* __ construction => initialisation d'une partie de niveau débutant */
	d = demineur_construire();
	assert(d != NULL);

	/* __ niveaux connus: redimensionnement 
				de débutant vers expert 
		puis 	de expert vers moyen
		puis 	de moyen vers débutant */
	niveau1 =DEMINEUR_NIVEAU_DEBUTANT;
	niveau2 =DEMINEUR_NIVEAU_EXPERT;
	demineur_scenario_niveaux(d, niveau1, niveau2);

	niveau1 =niveau2;
	niveau2 =DEMINEUR_NIVEAU_MOYEN;
	demineur_scenario_niveaux(d, niveau1, niveau2);

	niveau1 =niveau2;
	niveau2 =DEMINEUR_NIVEAU_DEBUTANT;
	demineur_scenario_niveaux(d, niveau1, niveau2);

	/* __ niveaux connus: redimensionnement 
				de débutant vers DEMINEUR_NIVEAU_MIN -1 
		puis 	de débutant vers DEMINEUR_NIVEAU_MAX +1 */
	niveau1 =niveau2;
	demineur_scenario_niveaux(d, niveau1, DEMINEUR_NIVEAU_MIN -1);
	demineur_scenario_niveaux(d, niveau1, DEMINEUR_NIVEAU_MAX +1);

	/* __ fin de test (on libère la mémoire avant de sortir) */
	d =demineur_detruire(d);
}

/* Redimensionnement (dont ré-initialisation) manuel */
void demineur_test_dimensions_manuelles() {
	demineur* d = NULL;
	unsigned int h =4, l =8;

	printf("____ Modele demineur :: fonction de test %s. Lecture des affichages à venir:\n", __func__);
	demineur_afficher_emploi();

	/* __ construction => initialisation d'une partie de niveau débutant */
	d = demineur_construire();
	assert(d != NULL);

	/* __ Redimensionnement manuel, dimensions valides, nombre de mines manuel */
	demineur_scenario_dimensions(d, h, l, h*l/2);

	/* __ Redimensionnement manuel, dimensions valides, nombre de mines automatique */
	demineur_scenario_dimensions(d, h, l, 0);

	/* __ Redimensionnement manuel, dimensions invalides */
	demineur_scenario_dimensions(d, 0, l, 0);				/* hauteur trop petite */
	demineur_scenario_dimensions(d, H_MAX +1, l, 0);		/* hauteur trop grande */
	demineur_scenario_dimensions(d, h, 0, 0);				/* largeur trop petite */
	demineur_scenario_dimensions(d, h, L_MAX +1, 0);		/* largeur trop grande */
	demineur_scenario_dimensions(d, h, l, h*l +1);			/* trop de mines */

	/* __ fin de test (on libère la mémoire avant de sortir) */
	d =demineur_detruire(d);
}

/* Action de jeu : marquer une case (indices corrects et indices incorrects) */
void demineur_test_marquer_case() {
	demineur* d;
	unsigned int h =2, l =4, choix;

	printf("____ Modele demineur :: fonction de test %s. Lecture des affichages à venir:\n", __func__);
	demineur_afficher_emploi();

	/* __ Initialisation de la partie */
	d = demineur_construire();
	assert(d != NULL);
	demineur_set_dimensions(d, h, l, 0);
	
	/* __ Choix du test */
	printf("Choix du test:\n\t1 pour i < 0\n\t2 pour i >= d->dim.h\n\t3 pour j < 0\n\t4 pour j >= d->dim.l\n\ttoute autre valeur pour dérouler le test normal (indices corrects)\n");
	scanf("%d", &choix);
	purger_buffer(); /* purge du buffer pour les getchar() derrière */

	/* Indice incorrect (sortie en erreur) */	
	if(choix == 1)
		demineur_case_marquer(d, -1, 0);
	else if(choix == 2)
		demineur_case_marquer(d, h +1, 0);
	else if(choix == 3)
		demineur_case_marquer(d, 0, -1);
	else if(choix == 4)
		demineur_case_marquer(d, 0, l +1);
	else{	/* test normal (indices corrects) */
		/* __ Cases non dévoilées :
			1. on marque toutes les cases (les cases sont toutes marquées minées)
			2. on marque les cases d'une colonne sur 2 (une case sur deux est maintenant marquée interro)
			3. on marque les cases d'une colonne sur 4 (une case sur quatre est non marquée) */
		printf("**** Test 1: on marque toutes les cases. Vérifier:\n");
		printf("\t- que toutes les cases sont bien marquées minées\n");
		printf("\t- que la champ nb_marquees est passé de 0 au nombre %u de cases du jeu\n", h*l);
		demineur_scenario_marquer(d, 1, 0);

		printf("**** Test 2: on marque une case sur 2. Vérifier:\n");
		printf("\t- que la marque d'une case sur 2 est dévenue MARQUE_INTERRO\n");
		printf("\t- que la champ nb_marquees a bien été diminué de moitié (il devient %u)\n", h*l/2);
		demineur_scenario_marquer(d, 2, 0);

		printf("**** Test 3: on marque une case sur 4. Vérifier:\n");
		printf("\t- que la marque d'une case marquée MARQUE_INTERRO sur 2 est devenur MARQUE_AUCUNE\n");
		printf("\t- que la champ nb_marquees est inchangé\n");
		demineur_scenario_marquer(d, 4, 0);

		/* __ Partie finie : 
			4. on modifie l'état de la partie avant de marquer les cases du jeu (la situation doit être inchangée) */
		printf("**** Test 4: demande de marquage de cases dans une partie (artificiellement) finie. Vérifier que rien ne change.\n");
		d->etat =PARTIE_GAGNEE;
		demineur_scenario_marquer(d, 1, 0);
		d->etat =PARTIE_PERDUE;
		demineur_scenario_marquer(d, 1, 0);

		/* __ Cases dévoilées : 
			5. on dévoile puis on marque toutes les cases du jeu (la situation doit être inchangée) */
		printf("**** Test 5: demande de marquage de cases toutes (artificiellement) dévoilées. Vérifier que rien ne change.\n");
		d->etat =PARTIE_ENCOURS;
		demineur_scenario_marquer(d, 1, 1);
	}

	/* __ fin de test (on libère la mémoire avant de sortir) */
	d =demineur_detruire(d);
}

/* Action de jeu : dévoiler une case (indices corrects et indices incorrects) */
void demineur_test_devoiler_case() {
	demineur* d = NULL;
	unsigned int h =5, l =10, m =5, choix;

	printf("____ Modele demineur :: fonction de test %s. Lecture des affichages à venir:\n", __func__);
	demineur_afficher_emploi();

	/* __ Initialisation de la partie */
	d = demineur_construire();
	assert(d != NULL);
	demineur_set_dimensions(d, h, l, m);

	/* __ Choix du test */
	printf("Choix du test:\n\t1 pour i < 0\n\t2 pour i >= d->dim.h\n\t3 pour j < 0\n\t4 pour j >= d->dim.l\n\ttoute autre valeur pour dérouler le test normal (indices corrects)\n");
	scanf("%d", &choix);
	purger_buffer(); /* purge du buffer pour les getchar() derrière */

	/* Indice incorrect (sortie en erreur) */	
	if(choix == 1)
		demineur_case_devoiler(d, -1, 0);
	else if(choix == 2)
		demineur_case_devoiler(d, h +1, 0);
	else if(choix == 3)
		demineur_case_devoiler(d, 0, -1);
	else if(choix == 4)
		demineur_case_devoiler(d, 0, l +1);
	else{	/* test normal (indices corrects) */
		/* __ Cases non dévoilées : on vérifie la bonne mise à jour des informations, dont l'état de la partie
			1. dévoilement systématique de toutes les cases non minées (partie gagnée)  
			2. dévoilement systématique de toutes les cases, y compris les minées (partie perdue)  */
		printf("**** Test 1: on dévoile systématiquemet toutes les cases non minées. Vérifier:\n");
		printf("\t- que chaque fois qu'une nouvelle case est dévoilée, les case voisines des cases dévoilées qui ne sont adjacentes à aucune mine sont bien elles aussi dévoilées, et le compteur nb_a_devoiler est correctement mis à jour\n");
		printf("\t- que les demandes de dévoilement des cases déjà dévoilées sont refusées\n");
		printf("\t- que la partie passe en état PARTIE_GAGNEE dès lors que toutes les cases non minées ont été dévoilées\n");
		printf("\t- que les marques des cases dévoilées sont bien remises à MARQUE_AUCUNE, et le compteur nb_marquees mis à jour en conséquence\n");
		demineur_scenario_devoiler(d, 0);	/* on doit gagner la partie et avoir dévoilé toutes les cases non minées */

		printf("**** Test 2: on dévoile systématiquemet toutes les cases, même les cases minées. Vérifier:\n");
		printf("\t- que la partie passe en état PARTIE_PERDUE dès lors qu'une case minée a été dévoilée\n");
		printf("\t- qu'une fois la partie finie, les demandes de dévoilemen sont toutes refusées\n");
		demineur_scenario_devoiler(d, 1);	/* on doit gagner la partie et avoir dévoilé toutes les cases non minées */
	}

	/* __ fin de test (on libère la mémoire avant de sortir) */
	d =demineur_detruire(d);
}

/* Scenario de test : accesseurs cases 
	- on mine une case sur 2
	- on marque {minée, interro, non minée} une case sur 3
	- on dévoile une case sur 2 (1 minée sur 2, 1 non minée sur 2)
	Le tout par accès direct aux champs des cases
*/
void demineur_test_get() {
	demineur* d = NULL;
	unsigned int i, j;

	printf("____ Modele demineur :: fonction de test %s. Lecture des affichages à venir:\n", __func__);
	demineur_afficher_emploi();

	/* __ Initialisation de la partie */
	d = demineur_construire();
	assert(d != NULL);
	for (i = 0 ; i < d->dim.hauteur ; i ++)
		for (j = 0 ; j < d->dim.largeur; j ++) {
			if((i + j) % 2 == 0)
				d->plateau[i][j].est_minee = BOOLEEN_VRAI;

			if((i + j) % 3 == 0)
				d->plateau[i][j].marque_joueur = MARQUE_MINE;
			else if((i + j) % 3 == 1)
				d->plateau[i][j].marque_joueur = MARQUE_INTERRO;
				
			if((i + j) % 4 <= 1)
				d->plateau[i][j].est_devoilee = BOOLEEN_VRAI;
			}

	/* __ Lecture  */
	printf("__ Affichage par accès direct aux champs:\n");
	demineur_afficher(d);
	printf("__ Affichage par utilisation des accesseurs:\n");
	demineur_afficher_get(d);
	getchar();	/* pause */

	/* __ fin de test (on libère la mémoire avant de sortir) */
	d =demineur_detruire(d);
}

/* Accesseurs case pour des cases fonctionnellement incorrectes :
	- lire (les champs de la case ou l'information du nombre de mines adjacentes à calculer) ou agir (marquer ou devoiler) sur une case d'indice (i,j) hors borne
	- agir (marquer ou devoiler) sur une case d'indice (i,j) déjà dévoilée
	Fonctions testées :
	- int demineur_case_est_minee(demineur* d, int i, int j);
	- int demineur_case_est_devoilee(demineur* d, int i, int j);
	- marque demineur_case_get_marque(demineur* d, int i, int j);
	- int demineur_case_get_nbmines_adj(demineur* d, int i, int j);
	- int demineur_case_devoiler(demineur* d, int i, int j);
	- int demineur_case_marquer(demineur* d, int i, int j);
	Cette fonction fait appel aux fonctions :
	- demineur* demineur_construire();
	- int demineur_set_dimensions(demineur* d, unsigned int h, unsigned int l, int m);
	du modèle, qu'il s'agit alors d'avoir péalablement testées */
void demineur_test_case_ko() {
	demineur* d = NULL;
	unsigned int h = 2, l = 3;
	int i, j;

	printf("____ Modele demineur :: fonction de test %s\n", __func__);

	/* __ construction puis redimensionnement d'une partie de demineur (=> initialisation d'une partie) dont on devoile ensuite toutes les cases du jeu */
	d = demineur_construire();
	assert(d != NULL);

	demineur_set_dimensions(d, h, l, -1);
	demineur_set_init(d);
	printf("\tVariable demineur initiale :\n");
	demineur_afficher(d);

	/* __ Agir sur une case d'indice correct mais la case est dévoilée */
	printf("\n1. __ Test marquer & devoiler case sur une case d'indice correct mais deja devoilee :\n");

	/* on met à BOOLEEN_VRAI 1 case du jeu sur 10, et l'on essaie ensuite de la marquer / la dévoiler */
	for(i=0 ; i < (int) d->dim.hauteur ; i++) {
		for(j=0 ; j < (int) d->dim.largeur ; j++) {
			printf("\tdemineur_case_{marquer,devoiler} sur la case d'indice (%d,%d) devoilee retournent respectivement %d et %d\n", i, j, demineur_case_marquer(d,i,j), demineur_case_devoiler(d,i,j));
		}
	}
	getchar();

	/* __ Accéder à une case d'indice incorrect */
	printf("\n2. __ Test acces a des cases d'indice incorrect, jeu de hauteur %u et de largeur %u :\n", d->dim.hauteur, d->dim.largeur);
	for(i=-1 ; i <= (int) d->dim.hauteur ; i++) {
		for(j=-1 ; j <= (int) d->dim.largeur ; j++) {
			/* on ne s'intéresse ici qu'aux cases hors bornes (fonctionnelles) */
			if (i < 0 || j < 0 || i >= (int) d->dim.hauteur || j >= (int) d->dim.largeur) {
				printf("\tPour l'indice (i,j) == (%d,%d) :\n", i, j);
				printf("\t- demineur_case_{marquer,devoiler} retournent respectivement %d et %d\n", demineur_case_marquer(d,i,j), demineur_case_devoiler(d,i,j));
				printf("\t- demineur_case_{est_minee, est_devoilee, get_marque, get_nbmines_adj} retournent respectivement %d, %d, %d, %d\n", demineur_case_est_minee(d,i,j), demineur_case_est_devoilee(d, i, j), demineur_case_get_marque(d,i,j), demineur_case_get_nbmines_adj(d, i, j));
			}
		}
	}

	/* __ destruction */
	d =demineur_detruire(d);
}

/* ________________________________ Définition des fonctions statiques
*/

/* Mode d'emploi lecture de la grille affichée par demineur_afficher() */
static void demineur_afficher_emploi() {
	printf("\tAffichage pour chaque case des informations [est_minee|est_devoilee|marque_joueur|#mines adjacentes] ou l'on indique :\n");
	printf("\t- pour 'est_minee' : 'X', ' ', '!' si le champ vaut respectivement BOOLEAN_VRAI, BOOLEEN_FAUX, autre chose\n");
	printf("\t- pour 'est_devoilee' : 'D', ' ', '!' si le champ vaut respectivement BOOLEAN_VRAI, BOOLEEN_FAUX, autre chose\n");
	printf("\t- pour 'marque_joueur' : 'M', '?', ' ', '!' si le champ vaut respectivement MARQUE_MINE, MARQUE_INTERRO, MARQUE_AUCUNE, autre chose\n");
	printf("\t- pour 'mines voisines' : le nombre\n");
}

/* Affichage détaillé d'une variable demineur */
static void demineur_afficher(demineur* d) {
	int i, j;
	unsigned int h = d->dim.hauteur, l = d->dim.largeur, m = d->dim.nbmines;

	/* Dimensions & champs atomiques */
	printf("jeu %p de (h =%u x l =%u) = %u cases dont %u minées: \n", d, h, l, h *l, m);
	printf("nb cases restant à dévoiler =%u / marquées minées =%u\tétat =%d \t", d->nb_a_devoiler, d->nb_marquees, d->etat);
	printf("heures début %s / heure fin %s :\n", ctime(& d->debut), ctime(& d->fin));

	/* Cases */
	for(i=0 ; i < (int)h ; i++, printf("\n"))
		for(j=0 ; j < (int)l ; j++) {
			/* case minee ? */
			if (d->plateau[i][j].est_minee == BOOLEEN_VRAI)
				printf("[X|");
			else if (d->plateau[i][j].est_minee == BOOLEEN_FAUX)
				printf("[ |");
			else
				printf("[!|");

			/* case dévoilée ? */
			if (d->plateau[i][j].est_devoilee == BOOLEEN_VRAI)
				printf("D|");
			else if (d->plateau[i][j].est_devoilee == BOOLEEN_FAUX)
				printf(" |");
			else
				printf("!|");

			/* marque */
			if (d->plateau[i][j].marque_joueur == MARQUE_MINE)
				printf("M|");
			else if (d->plateau[i][j].marque_joueur == MARQUE_INTERRO)
				printf("?|");
			else if (d->plateau[i][j].marque_joueur == MARQUE_AUCUNE)
				printf(" |");
			else
				printf("!|");

			/* mines adjacentes */
			printf("%u] ", d->plateau[i][j].nbmines_vois);
		}
}

/* Mine, dévoile, marque comme minées, attribue 9 mines voisines à toutes les cases du jeu, 
		et met en cohérence les champs atomiques de la structure */
static void demineur_set_init(demineur* d) {
	int i, j;
	unsigned int h = d->dim.hauteur, l = d->dim.largeur;

	/* modification de toutes les cases participant au jeu */
	for (i = 0 ; i < h ; i++)
		for (j = 0 ; j < l ; j++) {
			d->plateau[i][j].est_minee = BOOLEEN_VRAI;
			d->plateau[i][j].est_devoilee = BOOLEEN_VRAI;
			d->plateau[i][j].marque_joueur = MARQUE_MINE;
			d->plateau[i][j].nbmines_vois = 9;
		}

	/* mise en cohérence des champs atomiques du jeu */
	d->nb_a_devoiler =0;
	d->nb_marquees =h * l ;
	d->etat =PARTIE_PERDUE;
}

/* Scenario de test : redimensionnement par niveau prédéfini */
static void demineur_scenario_niveaux(demineur* d, demineur_niveau niveau1, demineur_niveau niveau2) {
	int retour;

	printf("__ D'un jeu %d totalement miné, dévoilé, marqué mine, avec 9 mines voisines (affichage #1) vers un jeu %d (affichage #2): vérifier %s\n", niveau1, niveau2, niveau2 >= DEMINEUR_NIVEAU_MIN && niveau2 <= DEMINEUR_NIVEAU_MAX ? "redimensionnement +(ré)initialisation des cases et champs atomiques" : "jeu inchangé");
	demineur_set_init(d);
	demineur_afficher(d);
	retour =demineur_set_niveau(d, niveau2);
	demineur_afficher(d);
	printf("valeur retour %d (doit être %d)\t", retour, niveau2 >= DEMINEUR_NIVEAU_MIN && niveau2 <= DEMINEUR_NIVEAU_MAX ? 0 : 1);
	getchar();	/* pause */
}

/* Scenario de test : redimensionnement manuel */
static void demineur_scenario_dimensions(demineur* d, unsigned int h, unsigned int l, unsigned int m) {
	int retour;

	printf("__ Redimensionnement manuel depuis grille débutante totalement minée, dévoilée, marquée minée, avec 9 mines voisines vers (h =%u, l =%u, m =%u): vérifier ", h, l, m);
	if(h < 1 || h > H_MAX || l < 1 || l > L_MAX || m > h * l)
		printf("jeu inchangé\n");
	else if(m > 0)
		printf("jeu redimensionné et réinitialisé avec %u mines\n", m);
	else
		printf("jeu redimensionné et réinitialisé avec %u mines\n", NB_MINES_DEFAUT(h, l));

	demineur_set_niveau(d, DEMINEUR_NIVEAU_DEBUTANT);
	demineur_set_init(d);
	demineur_afficher(d);
	retour = demineur_set_dimensions(d, h, l, m);
	demineur_afficher(d);
	printf("valeur retour %d (doit être %d)\t", retour, h >= 1 && h <= H_MAX && l >= 1 && l <= L_MAX && m <= h*l ? 0 : 1);
	getchar();
}

/* Scenario de test : marquer case */
static void demineur_scenario_marquer(demineur* d, int pas, int devoile) {
	unsigned int i, j, nb_marquees =d->nb_marquees;
	int retour;

	printf("__ Jeu initial :\n");
	demineur_afficher(d);

	for (i = 0 ; i < d->dim.hauteur ; i ++)
		for (j = 0 ; j < d->dim.largeur; j =j +pas) {
			if(devoile)
				d->plateau[i][j].est_devoilee =BOOLEEN_VRAI;

			retour =demineur_case_marquer(d, i, j);
			printf("__ (i, j) =(%u, %u)\tretour = %d\t", i, j, retour);
			if(d->nb_marquees != nb_marquees) {
				if(d->nb_marquees > nb_marquees)
					printf("case marquée minée\n");
				else
					printf("case DÉmarquée minée\n");
				nb_marquees =d->nb_marquees;
			}
			else
				printf("\n");
		}

	printf("__ Jeu final :\n");
	demineur_afficher(d);
	getchar();	/* pause */
}

/* Scenario de test : dévoiler systématiquement
	- toutes les cases du jeu si mines =1
	- toutes les cases non minées du jeu sinon 
	depuis une partie 
	- fraichement initialisée sur laquelle on marque 1 case sur 3 marquée minée, pê minée, non marquée si init = 1
	- déjà finie sinon */
static void demineur_scenario_devoiler(demineur* d, int mines) {
	unsigned int i, j, nb_a_devoiler =d->nb_a_devoiler;
	etat_partie etat =d->etat;
	int retour;

	/* __ préparation de la partie & son affichage */
	demineur_nouvelle_partie(d);
	for (i = 0 ; i < d->dim.hauteur ; i ++)
		for (j = 0 ; j < d->dim.largeur; j ++) {
			if((i + j)%3 == 0) {
				d->plateau[i][j].marque_joueur =MARQUE_MINE;
				d->nb_marquees ++;
			}
			else if((i + j)%3 == 1)
				d->plateau[i][j].marque_joueur =MARQUE_INTERRO;
		}
	printf("____ Jeu initial: \n");
	demineur_afficher(d);

	/* __ dévoilement systématique des cases */
	for (i = 0 ; i < d->dim.hauteur ; i ++)
		for (j = 0 ; j < d->dim.largeur; j ++)
			if(mines || d->plateau[i][j].est_minee == BOOLEEN_FAUX) {
				retour = demineur_case_devoiler(d, i, j);
				printf("__ (i, j) =(%u, %u)\tretour = %d\t", i, j, retour);
				if(d->etat != etat) {
					etat =d->etat;
					printf("état devenu %d\t", d->etat);
				}
				else
					printf("état inchangé\t");
				if(d->nb_a_devoiler != nb_a_devoiler) {
					nb_a_devoiler =d->nb_a_devoiler;
					printf("au moins 1 case dévoilée. Jeu intermédiaire:\n");
					demineur_afficher(d);
					getchar();	/* pause */
				}
				else
					printf("pas de case dévoilée\n");
			}

	printf("____ Jeu final:\n");
	demineur_afficher(d);
	getchar();	/* pause */
}

/* Affichage détaillé d'une variable demineur *par appel systématique aux accesseurs* */
static void demineur_afficher_get(demineur* d) {
	int i, j;
	unsigned int h, l, m, nb_a_devoiler, nb_marquees;
	etat_partie etat;
	marque ma;
	long unsigned int temps;
	
	/* Dimensions & champs atomiques */
	demineur_get_dimensions(d, &h, &l, &m);
	nb_a_devoiler =demineur_get_nbcases_a_devoiler(d);
	nb_marquees =m -demineur_get_nbmines_a_deviner(d);
	etat =demineur_get_etat(d);
	temps =demineur_get_temps(d);
	
	/* Dimensions & champs atomiques */
	printf("jeu %p de (h =%u x l =%u) = %u cases dont %u minées: \n", d, h, l, h *l, m);
	printf("\tnb cases restant à dévoiler =%u / marquées minées =%u\tétat =%d\t", nb_a_devoiler, nb_marquees, etat);
	printf("\ttemps écoulé depuis le début de partie =%lu :\n", temps);

	/* Cases */
	for(i=0 ; i < (int)h ; i++, printf("\n"))
		for(j=0 ; j < (int)l ; j++) {
			/* case minee ? */
			printf("[%c|", demineur_case_est_minee(d, i, j) ? 'X' : ' ');						/* case minée ? */

			/* case dévoilée ? */
			printf("%c|", demineur_case_est_devoilee(d, i, j) ? 'D' : ' ');

			/* marque */
			ma =demineur_case_get_marque(d, i, j);
			printf("%c|", ma == MARQUE_MINE ? 'M' : (ma == MARQUE_INTERRO ? '?' : ' '));

			/* mines adjacentes */
			printf("%d] ", demineur_case_get_nbmines_adj(d, i, j));
		}
}

/*	Fonction permettant de vider le buffer (pour sécuriser les saisies, et assurer les pauses par getchar()) */
static void purger_buffer() {
	char c;

	do
		c = getchar();
	while(c != '\n' && c != EOF);
}
