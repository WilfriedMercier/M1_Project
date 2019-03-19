#include "affiche.h"
#include <math.h>

#define PI (4*atan(1.))

float moy(float *tab, int size);

int reglin_Mercier(float *x, float *y, int nb, float *a, float *b, float *err, float *cor);
float variance(float *x, int n);
float covariance(float *x, float *y, int n);
float moyenne_x(float *x, int n);

int main(){

   int nb=0;
   int i=0;
   long temps=0;
   float latt=0., vit_rot=0., rot_origine=0., err=0., correlation=0.;

   char nom[100] = {};

   float *sec=NULL;
   float *lattitude=NULL;
   float *longitude=NULL;

/*   FILE *f=NULL;*/

   scanf("%d", &nb);

   /***********************/
   /* Allocation tableaux */
   /***********************/
   sec = calloc(nb, sizeof (long));
   if (sec == NULL){
      printf("Probleme d'allocation tableau sec\n");
   }
   lattitude = calloc(nb, sizeof (float));
   if (lattitude == NULL){
      printf("Probleme d'allocation tableau lattitude\n");
   }
   longitude = calloc(nb, sizeof (float));
   if (longitude == NULL){
      printf("Probleme d'allocation tableau sec\n");
   }

   /***********************/
   /*Remplissage tableaux */
   /***********************/
   for (i=0; i<nb; i++){
      scanf("%s", nom);
      affichage(nom, &longitude[i], &lattitude[i], &temps);
      longitude[i] *= 180/PI; /*angles en degres*/
      lattitude[i] *= 180/PI;
      sec[i] = (float) temps /3600; /*pour avoir le temps en h*/
   }

   /*lattitude moyenne*/
   latt = moy(lattitude, nb);

   /*Regression lineaire*/
   reglin_Mercier( sec, longitude, nb, &vit_rot, &rot_origine, &err, &correlation);
   printf("lattitude = %f\nordonnee origine = %f\nvit rotation = %f °/h\ncorrelation = %f\nerreur = %f\n", latt, rot_origine, vit_rot, correlation, err);

   /*************************/
   /* Sauvegarde des donnes */
   /*************************/
/*   f = fopen("rotation.dat", "w");

   fprintf(f, "%f\n", latt);
   for (i=0; i<nb; i++){
      fprintf(f, "%f %f\n", sec[i], longitude[i]);
   }

   fclose(f);
   f = NULL;*/

   /**************************/
   /* Desallocation tableaux */
   /**************************/
   free(sec);
   sec = NULL;
   free(lattitude);
   lattitude = NULL;
   free(longitude);
   longitude = NULL;

   return 0;
}

int reglin_Mercier(float *x, float *y, int nb, float *a, float *b, float *err, float *cor){

   int i=0;
   float var_x = variance(x, nb), cov = covariance(x, y, nb);

   *a = cov/var_x;
   *b = moy(y, nb) - *a*moy(x, nb);
   for (i=0; i<nb; i++)
      *err += (y[i]-*a*x[i]-*b)*(y[i]-*a*x[i]-*b);
   *err = sqrt(*err / (nb*(nb-2)*var_x));
   *cor = cov/sqrt(var_x*variance(y, nb));

   return 0;
}

float moy(float *x, int n){

   int i=0;
   float sum=0.;

   for (i=0; i<n; i++)
      sum += x[i];

   return sum/n;
}

float covariance(float *x, float *y, int n){

   int i=0;
   float x_moy=0, y_moy=0, sum=0;

   x_moy = moy(x, n);
   y_moy = moy(y, n);
   for (i=0; i<n; i++)
      sum += (x[i]-x_moy)*(y[i]-y_moy);

   return sum/n;
}

float variance(float *x, int n){
   int i=0;
   float x_moy=0, sum=0;

   x_moy = moy(x, n);
   for (i=0; i<n; i++)
      sum += (x[i]-x_moy)*(x[i]-x_moy);

   return sum/n;
}
