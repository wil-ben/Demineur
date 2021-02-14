# Demineur

Réalisé par Carré Gaëtan, Benrejdal Wilhan et Le Moine Nicolas le 14 février 2021.

Pour jouer, il vous faudra d'abord compiler les fonction et générer un executable du jeu. Il vous suffit pour cela de taper "make" dans votre terminale à l'emplacement des fichier. Vous devrez finalement taper "./jeu.exe" au même endroit dans votre terminale pour lancer le jeu.

Règles du jeu:

Le but du jeu est simple: RESTER EN VIE.

Vous devez dévoiler les cases du jeu par un clique gauche, mais il ne faut surtout pas dévoiler de cases contenant une mine. Pour gagner, vous devez donc dévoiler toutes les cases ne contenant pas de mines, et ce le plus rapidement possible. Lorsque vous dévoilez une case, le cas échéant elle vous indiquera le nombre de mines se trouvant sur les 8 cases voisines à celle-ci. Si elle ne contient pas de mine, alors les cases lui étant voisines sont également dévoilées. En revanche, si cette case contient une mine, vous perdez.

Si vous connaissez l'emplacement d'une mine, il vous est possible de l'indiquer par un clique droit. Un drapeau sera alors mis en place à cet emplacement. Un second clique gauche vous permettra d'inscrire un point d'interrogation sur cette case dans le cas ou vous vous intérrogez sur son contenu.

Il vous est possible, au début de la partie, de choisir parmi trois niveau de difficulté (niveau débutant, moyen ou expert). Pas d'inquiétude, vous pouvez jouer autant de partie que vous désirez en cliquant sur rejouer et vous pourrez choisir la difficulté pour chacune de ces partie.

Il vous est finalement possible de quitter le jeu à n'importe quel moment à l'aide du bouton quitter.

Bonne chance !