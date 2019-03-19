#include <math.h>

int coordonnees_sph(long dimx, long dimy, int x, int y, long *R, float *L, float *l){
   /***********************************************/
   /* dimx, dimy : dimensions de l'image recadree */
   /* x, y       : position de la tache cliquee   */
   /* *R         : rayon du Soleil en pixels      */
   /* *L         : longitude                      */
   /* *l         : lattitude                      */
   /***********************************************/

   int x_c=dimx/2; /*position du centre du Soleil*/
   int y_c=dimy/2;

   /****************************************************************/
   /* Coordonnes de la tache dans le repere centre sur (x_c , y_c) */
   /****************************************************************/
   x -= x_c;
   y = y_c-y;

   *R = (dimx+dimy)/4;

   if (*R != 0){
      *l = asin(1.*y/ *R);
      if (cos(*l) != 0){
         *L = asin(x/(*R*cos(*l)));
      }
      else{
         return -1;
      }
   }
   else{
      return -2;
   }

   return 0;
}

