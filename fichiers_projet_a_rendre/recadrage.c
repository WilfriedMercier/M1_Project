int bords_Soleil(long **im, long dimx, long dimy, int max, int *yu, int *xl, int *xr, int *yd){
   /****************************************************************/
   /* max : maximum de l'image multiplie par le seuil de tolerance */
   /* *yu : position y du bord haut                                */
   /* *xl : position x du bord gauche                              */
   /* *xr : position x du bord droit                               */
   /* *yd : position y du bord bas                                 */
   /****************************************************************/
   int i=0, j=0;
   long midx = dimx/2;
   long midy = dimy/2;
   int coupure=30;

   /*****************************/
   /* Cherche le y du bord haut */
   /*****************************/
   *yu = -1;
   j=coupure;
   do{
      for (i=midx-40; i<midx+40; i++){
         if (im[i][j]>max){
               *yu = j;
               break;
         }
      }
      j++;
   }while( (*yu<0) && (j<dimy) );

   /*****************************/
   /* Cherche le y du bord bas  */
   /*****************************/
   *yd = -1;
   j=dimy-coupure;
   do{
      for (i=midx-40; i<midx+40; i++){
         if (im[i][j]>max){
               *yd = j;
               break;
         }
      }
      j--;
   }while( (*yd<0) && (j>0) );

   /*******************************/
   /* Cherche le x du bord gauche */
   /*******************************/
   *xl = -1;
   j=coupure;
   do{
      for (i=midy-40; i<midy+40; i++){
         if (im[j][i]>max){
               *xl = j;
               break;
         }
      }
      j++;
   }while( (*xl<0) && (j<dimx) );

   /*******************************/
   /* Cherche le x du bord droit  */
   /*******************************/
   *xr = -1;
   j=dimx-coupure;
   do{
      for (i=midy-40; i<midy+40; i++){
         if (im[j][i]>max){
               *xr = j;
               break;
         }
      }
      j--;
   }while( (*xr<0) && (j>0) );

   return 0;
}

int remplissage_Soleil(long **im, long **im_Sol, long dimx, long dimy, int x_left, int y_top){
   /*********************************************************************************/
   /* dimx : nb de colonnes du tableau im_Sol                                       */
   /* dimy : nb de lignes de im_Sol                                                 */
   /* x_left : position du premier pixel le plus a gauche de im_Sol dans l'image im */
   /* y_top : idem mais pour le premier pixel le plus en haut                       */
   /*********************************************************************************/

   int i=0, j=0;

   for (i=0; i<dimy; i++){
      for (j=0; j<dimx; j++){
         im_Sol[j][i] = im[j+x_left][i+y_top];
      }
   }

   return 0;
}
