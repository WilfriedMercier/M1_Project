#include <stdio.h>

int correction_histogramme(int *hist, int borne_inf, int n, int im_max, int *c_min, int *c_max);

int histogramme(long **im, long dimx, long dimy, int *c_min, int *c_max, int *max){
   /********************************************************************************************************************/
   /* *c_min : couleur minimum ayant une occurence n fois inférieure à la couleur apparaissant le plus (voir plus bas) */
   /* *c_max : idem mais de valeur superieur                                                                           */
   /********************************************************************************************************************/
   int hist[100] = {0};
   int i=0, j=0;

   FILE *f=NULL;

   /*Initilisation de l'histogramme à 0*/
   for (i=0; i<100; i++){
      hist[i] = 0;
   }

   /*Recuperation du max de l'image*/
   *max=0;
   for (i=0; i<dimx; i++){
      for (j=0; j<dimy; j++){
         if (im[i][j] > *max){
            *max = im[i][j];
         }
         /*printf("%d %d %d\n", i, j, *max);*/
      }
   }

   /*remplissage de l'histogramme*/
   for (i=0; i<dimx; i++){
      for (j=0; j<dimy; j++){
         hist[im[i][j]*100/ *max]++;
      }
   }

/********************************************************************
   f = fopen("histogramme.dat", "w");
   if (f==NULL){
      printf("Erreur d'ouverture fichier histogramme_corr");
      return -1;
   }
   for (i=0; i<100; i++){
      fprintf(f, "%d %d\n", i* *max/100, hist[i]);
   }
   fclose(f);
   f = NULL;
******************************************************************/

   /*Correction de l'histogramme*/
   correction_histogramme(hist, 1000, 5, *max, c_min, c_max);

   /*Ouverture fichier*/
   f = fopen("histogramme_corr.dat", "w");
   if (f==NULL){
      printf("Erreur d'ouverture fichier histogramme_corr");
      return -1;
   }

   /*ecriture donnes dans histogramme_cor.dat*/
   for (i=0; i<100; i++){
      fprintf(f, "%d %d\n", i* *max/100, hist[i]);
   }

   /*Fermeture fichier*/
   fclose(f);
   f = NULL;

   return 0;
}

int correction_histogramme(int *hist, int borne_inf, int n, int im_max, int *c_min, int *c_max){
   /*******************************************************************************************/
   /* borne_inf : borne inferieure en dessous de laquelle les valeurs sont mises a 0          */
   /* n         : coefficient qui permet de trouver les valeurs des couleurs qui apparaissent */
   /*             max/n fois dans l'histogramme avec max la valeur maximale de l'histogramme  */
   /* im_max    : maximum de l'image (pas de l'histogramme)                                   */
   /*******************************************************************************************/

   int i=0;
   int max=0; /* max de l'histogramme */
   int cnt=0;

   /*Ajoute une contrainte pour enlever le pic du au fond et cherche la valeur la plus frequente*/
   for (i=0; i<100; i++){
      if (i*im_max/100 <= borne_inf){
         hist[i]=0;
      }
      else{
         if (hist[i] > max){
            max = hist[i];
         }
      }
   }

   cnt=0;
   *c_min = 0;
   *c_max = 0;
   for (i=0; i<100; i++){
      if ( (hist[i]>max/n) && (cnt==0) ){
         *c_min = i*im_max/100;
         cnt = 1;
      }
      else if ( (hist[i]<max/n) && (cnt==1) ){
         *c_max = i*im_max/100;
         return 0;
      }
   }

   return 1;
}

