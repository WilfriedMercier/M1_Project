# ====================================================================================== #
#                                   Notes de l'auteur                                    #
# ====================================================================================== #
# ! Makefile pour le projet "rotation du soleil" du M1 de l'Observatoire.
# ! Ce Makefile a été adapté afin de fonctionner sur tous les systèmes
# ! GNU/Linux pour lesquels les librairies X11 et cfitsio ont été
# ! installées en passant par le gestionnaire de paquets de la distribution.

# ====================================================================================== #
#                                 Variables du Makefile                                  #
# ====================================================================================== #

# ------------------------------------------
# 1 - variable du compilateur à utiliser
# 2 - choix des options de compilation
# 3 - liste des répertoires d'en-têtes
# 4 - liste des répertoires de librairies
# 5 - librairies à lier
# ------------------------------------------
CC     = /usr/bin/gcc
CFLAGS = -ansi -Wall -Wextra -Werror
IFLAGS = -I /usr/include
LFLAGS = -L /usr/lib
LINK   = -lX11 -lcfitsio -lm

# ====================================================================================== #
#                                   Cibles du Makefile                                   #
# nom_cible : dependance                                                                 #
# <tabulation> commande                                                                  #
# ====================================================================================== #

# ------------------------------------------
# ! - all : Compile tous les exécutables
# ! - help : Affiche cette aide
# ! - clean : efface les fichiers objet *.o
# !           et les exécutables
# ------------------------------------------
all : main

help :
	@grep -E "^# !" Makefile | sed -e 's/# !/ /g'

clean : 
	rm -f *.o affiche

# ------------------------------------------
# Exécutables
# ------------------------------------------
main : affiche.o affiche_lib.o histogramme.o reduction.o recadrage.o coord_sph.o Mercier_programme.o
	$(CC) $(LFLAGS) affiche.o affiche_lib.o histogramme.o reduction.o recadrage.o coord_sph.o Mercier_programme.o -o programme $(LINK)

# ------------------------------------------
# Fichiers intermédiaires (*.o)
# ------------------------------------------
Mercier_programme.o : Mercier_programme.c affiche.h
	$(CC) $(CFLAGS) $(IFLAGS) -c Mercier_programme.c

affiche.o : affiche.c affiche_lib.h fonctions.h
	$(CC) $(CFLAGS) $(IFLAGS) -c affiche.c

affiche_lib.o : affiche_lib.c affiche_lib.h
	$(CC) $(CFLAGS) $(IFLAGS) -c affiche_lib.c

histogramme.o : histogramme.c fonctions.h
	$(CC) $(CFLAGS) $(IFLAGS) -c histogramme.c

reduction.o : reduction.c fonctions.h
	$(CC) $(CFLAGS) $(IFLAGS) -c reduction.c

recadrage.o : recadrage.c fonctions.h
	$(CC) $(CFLAGS) $(IFLAGS) -c recadrage.c

coord_sph.o : coord_sph.c fonctions.h
	$(CC) $(CFLAGS) $(IFLAGS) -c coord_sph.c
