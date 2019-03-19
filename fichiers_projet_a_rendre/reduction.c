long moyenne(long **c, int x, int y, int len);

int reduction(long **cube, long dim1, long dim2, long **im_redu, int ech){
   /*************************************/
   /* ech        : facteur d'echelle    */
   /* dim1, dim2 : idem au main         */
   /* **im_redu  : image reduite de ech */
   /*************************************/

   int i=0, j=0;

   for (i=ech-1; i<dim1; i+=ech){
      for (j=ech-1; j<dim2; j+=ech){
         /* (i+1)/ech -1 car on souhaite obtenir une suite 0,1,2,3 pour les coordonnées de l'image reduite*/
         /* on prend cette formule pour faire la moyenne a partir du pixel le plus a droite et en bas     */
         /* pour eviter de faire une moyenne sur un bord ou il y auraient des problemes                   */
         im_redu[(i+1)/ech -1][(j+1)/ech -1] = moyenne(cube, j, i, ech);
      }
   }

   return 0;
}

long moyenne(long **c, int x, int y, int len){
   /********************************************************************************/
   /* x, y : position (dans l'image non reduite) du pixel en bas a droite du carre */
   /* len  : taille verticale/horizontal du carre pour faire la moyenne            */
   /********************************************************************************/

   long res=0;
   int i=0, j=0;

   for (i=0; i<len; i++){
      for (j=0; j<len; j++){
         res += c[y-i][x-j];
      }
   }

   return res/(len*len);
}

