# Rappels Makfile :
#
# cible:	 liste des dependances
# 	commande permettant de générer la cible
#
# $@		nom de la cible
# $^		liste des dépendances
# $<		nom de la première dépendance
#
# VARENV = valeur
# $(VARENV)

# _________________
# jeu de demineur /
# _______________/

# ____ variables d'environnement
CFLAGS = -Wall

GTK_VERSION = gtk+-3.0
CFLAGS_GTK = $(CFLAGS) `pkg-config $(GTK_VERSION) --cflags`
LDFLAGS_GTK = `pkg-config $(GTK_VERSION) --libs`

VUE_DIR = ../vue_jeu_grille/

# ____ exécutables
all: demineur-jeu.exe demineur-gtk.exe

# exécutables
demineur-jeu.exe: demineur-jeu.o demineur.o case-demineur.o dimension.o
	gcc $^ -o $@

demineur-gtk.exe: demineur-gtk.o demineur_ctrl.o $(VUE_DIR)vue_jeu_grille.o demineur.o case-demineur.o dimension.o
	gcc $^ -o $@	 $(LDFLAGS_GTK)

# objets programmes
demineur-jeu.o: demineur-jeu.c demineur-jeu.h demineur.h
	gcc -c $< -o $@ $(CFLAGS)

demineur-gtk.o: demineur-gtk.c demineur_ctrl.h demineur.h
	gcc -c $< -o $@ $(CFLAGS_GTK)

# objets modèle
demineur.o: demineur.c demineur.h case-demineur.h dimension.h
	gcc -c $< -o $@ $(CFLAGS)

case-demineur.o: case-demineur.c case-demineur.h
	gcc -c $< -o $@ $(CFLAGS)

dimension.o: dimension.c dimension.h
	gcc -c $< -o $@ $(CFLAGS)

# objets vue
$(VUE_DIR)vue_jeu_grille.o: $(VUE_DIR)vue_jeu_grille.c $(VUE_DIR)vue_jeu_grille.h
	gcc -c $< -o $@ $(CFLAGS_GTK)

# objets contrôleur
demineur_ctrl.o: demineur_ctrl.c demineur_ctrl.h $(VUE_DIR)vue_jeu_grille.h demineur.h
	gcc -c $< -o $@ $(CFLAGS_GTK)

# ____ programmes de test (contrôleur, vue, modèle)
tests: demineur-test.exe case-demineur-test.exe dimension-test.exe $(VUE_DIR)vue_jeu_grille-test.exe

# test controleur demineur
demineur_ctrl-test.exe: demineur_ctrl-test.o demineur_ctrl.o $(VUE_DIR)vue_jeu_grille.o demineur.o case-demineur.o dimension.o
	gcc $^ -o $@ $(LDFLAGS_GTK)

demineur_ctrl-test.o: demineur_ctrl-test.c demineur_ctrl.h $(VUE_DIR)vue_jeu_grille.h demineur.h
	gcc -c $< -o $@ $(CFLAGS_GTK)

# test vue grille
$(VUE_DIR)vue_jeu_grille-test.exe: $(VUE_DIR)vue_jeu_grille-test.o $(VUE_DIR)vue_jeu_grille.o
	gcc $^ -o $@ $(LDFLAGS_GTK)

$(VUE_DIR)vue_jeu_grille-test.o: $(VUE_DIR)vue_jeu_grille-test.c $(VUE_DIR)vue_jeu_grille.h
	gcc -c $< -o $@ $(CFLAGS_GTK)

# test modele demineur
demineur-test.exe: demineur-test.o demineur.o case-demineur.o dimension.o
	gcc $^ -o $@

demineur-test.o: demineur-test.c demineur-test.h demineur.h
	gcc -c $< -o $@ $(CFLAGS)

# test modele case demineur
case-demineur-test.exe: case-demineur-test.o case-demineur.o
	gcc $^ -o $@

case-demineur-test.o: case-demineur-test.c case-demineur.h
	gcc -c $< -o $@ $(CFLAGS)

# test modele dimension demineur
dimension-test.exe: dimension-test.o dimension.o
	gcc $^ -o $@

dimension-test.o: dimension-test.c dimension.h
	gcc -c $< -o $@ $(CFLAGS)

# ____ nettoyage
clean: 
	rm *.o *~ *.exe $(VUE_DIR)*.o $(VUE_DIR)*~ $(VUE_DIR)*.exe

