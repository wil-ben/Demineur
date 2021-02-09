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
CFLAGS = -Wall	# à utiliser pour la compilation de *tout fichier source*
CFLAGS_GTK = `pkg-config gtk+-3.0 --cflags`	# à utiliser pour la compilation de fichiers source *utilisant gtk*
LDFLAGS_GTK = `pkg-config gtk+-3.0 --libs`	# à utiliser pour la génération d'exécutables *utilisant gtk*

# ____ l'exécutable
EXEC = demineur-jeu.exe vue_demineur.exe
all: $(EXEC)

#jeu de démineur
demineur-jeu.exe: demineur-jeu.o demineur.o case-demineur.o dimension.o
	gcc $^ -o $@
demineur-jeu.o: demineur-jeu.c demineur-jeu.h demineur.h case-demineur.h dimension.h
	gcc -c $< -o $@ $(CFLAGS)

# ____ la bibliothèque
demineur.o: demineur.c demineur.h case-demineur.h dimension.h
	gcc -c $< -o $@ $(CFLAGS)

case-demineur.o: case-demineur.c case-demineur.h
	gcc -c $< -o $@ $(CFLAGS)

dimension.o: dimension.c dimension.h
	gcc -c $< -o $@ $(CFLAGS)

# programme GTK
vue_demineur.exe: vue_demineur.o ctrl_demineur.o main.o demineur.o case-demineur.o dimension.o
	gcc $^ -o $@ $(LDFLAGS_GTK)

vue_demineur.o: vue_demineur.c vue_demineur.h
	gcc -c $< -o $@ $(CFLAGS) $(CFLAGS_GTK)

ctrl_demineur.o: ctrl_demineur.c ctrl_demineur.h demineur.h dimension.h case-demineur.h vue_demineur.h
	gcc -c $< -o $@ $(CFLAGS) $(CFLAGS_GTK)

main.o: main.c ctrl_demineur.h demineur.h dimension.h case-demineur.h vue_demineur.h
	gcc -c $< -o $@ $(CFLAGS) $(CFLAGS_GTK)

# ____ nettoyage
.PHONY: clean mrproper

clean:
	rm -rf *.o

mrproper: clean
	rm -rf $(EXEC) $(TESTS)
