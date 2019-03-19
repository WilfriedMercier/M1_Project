#define CAR 100

/* ==================================================================================== */
/* =============================== En tetes necessaires =============================== */
/* ==================================================================================== */
#include "affiche_lib.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void viderbuffer ( FILE *stream );
void supp_saut_ligne ( char *chaine );

/**********************/
/* Dans histogramme.c */
/**********************/
int histogramme(long **im, long dimx, long dimy, int *c_min, int *c_max, int *max);

/********************/
/* Dans reduction.c */
/********************/
int reduction(long **cube, long dim1, long dim2, long **im_redu, int ech);

/********************/
/* Dans recadrage.c */
/********************/
int bords_Soleil(long **im, long dimx, long dimy, int max, int *yu, int *xl, int *xr, int *yd);
int remplissage_Soleil(long **im, long **im_Sol, long dimx, long dimy, int x_left, int y_top);

/********************/
/* Dans coord_sph.c */
/********************/
int coordonnees_sph(long dimx, long dimy, int x, int y, long *R, float *L, float *l);


