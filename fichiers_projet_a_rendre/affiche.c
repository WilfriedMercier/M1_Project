/* ==================================================================================== */
/* ====================== Description du programme affiche.c ========================== */
/* ==================================================================================== */
/* Auteurs & modifications                                                              */
/*    - Florence HENRY  : Novembre 2011                                                 */
/*    - Benjamin GODARD : Fevrier  2016                                                 */
/*    - Wilfried MERCIER : Mars 2018                                                    */
/*                                                                                      */
/* Fonctionnement                                                                       */
/*    - Lit un fichier .fits                                                            */
/*    - Recupere les couleurs min et max pour l'affichage                               */
/*    - Reduit l'image d'un facteur ech                                                 */
/*    - Cherche les bords et recentre l'image                                           */
/*    - Affiche l'image a l'ecran                                                       */
/*    - Attend : * un clic de souris pour renvoyer les coordonnees du pixel             */
/*               * l'appui sur la lettre q pour fermer la fenetre                       */
/*    - Calcule les coordonnes spheriques du point clique                               */
/****************************************************************************************/

/*************************************************************/
/* header contenant les declarations de fonctions et entetes */
/*************************************************************/
#include "fonctions.h"

int affichage (char *nom, float *longitude, float *lattitude, long *nb_sec)
{
   /*****************************************************************/
   /* Fonction appelee par le main pour traiter et afficher l'image */
   /* *nom                  : nom du fichier                        */
   /* *longitude, lattitude : coordonnees de la tache cliquee       */
   /* *nb_sec               : heure d'observation en secondes       */
   /*****************************************************************/

   /* ----------------------------------------------------- */
   /* Variables necessaires a l'affichage de l'image        */
   /* ----------------------------------------------------- */
   long **cube           = NULL  ; /* tab contenant l'image                */
   long   dim1           = 0     ; /* dim de l'image Nx                    */
   long   dim2           = 0     ; /* dim de l'image NY                    */
   int    x              = 0     ; /* coordonnee x du pixel                */
   int    y              = 0     ; /* coordonnee y du pixel                */

   int    c_min          = 0     ; /* Couleur min d'apres l'histo          */
   int    c_max          = 0     ; /* Idem mais en max                     */
   int    im_max         = 0     ; /* Valeur max de l'image                */

   long **im_reduc       = NULL  ; /* image reduite d'un facteur d'echelle */
   int    ech            = 2     ; /* facteur de reduction                 */

   float  seuil          = 0.3   ; /* seuil pour trouver les bords du Soleil */
   int    y_top          = 0     ; /* positions y du bord haut du Soleil   */
   int    x_left         = 0     ; /* position x du bord gauche            */
   int    x_right        = 0     ; /* position x du bord droit             */
   int    y_bottom       = 0     ; /* position y du bas                    */

   long **im_Soleil      = NULL  ; /* image du Soleil apres recadrage      */
   long   new_dim1       = 0     ; /* nouvelle dimension Nx apres recadrage */
   long   new_dim2       = 0     ; /* idem pour Ny                         */

   long   Rayon          = 0     ; /* rayon du Soleil en pixels            */

   /* ----------------------------------------------------- */
   /* Lecture de l'image                                    */
   /* ----------------------------------------------------- */
   printf("%s\n", nom);
   cube = LectureFITS (nom, &dim1, &dim2, nb_sec ) ;
   printf ( "L'image fait %ld (colonnes) par %ld (lignes) pixels\n", dim1, dim2 ) ;

   /********************************************************/
   /* Determination des c_min c_max d'apres un histogramme */
   /********************************************************/
   histogramme(cube, dim1, dim2, &c_min, &c_max, &im_max);

   /****************************/
   /* Allocation image reduite */
   /****************************/
   im_reduc = calloc(dim1/ech, sizeof(long *));
   if (im_reduc==NULL){
      printf("probleme d'allocation pour l'image reduite (1er niveau)\n");
      return -1;
   }
   for (x=0; x<dim1/ech; x++){
      im_reduc[x] = calloc(dim2/ech, sizeof(long));
      if (im_reduc[x]==NULL){
         printf("Probleme d'allocation pour l'image reduite (2nd niveau)\n");
         return -2;
      }
   }

   /************************************************************/
   /* Reduction du cube d'un facteur ech dans chaque direction */
   /************************************************************/
   reduction(cube, dim1, dim2, im_reduc, ech);

   /*********************************/
   /* Recherche des bords du Soleil */
   /*********************************/
   bords_Soleil(im_reduc, dim1/ech, dim2/ech, seuil*im_max, &y_top, &x_left, &x_right, &y_bottom);
   new_dim1 = x_right - x_left +1;
   new_dim2 = y_bottom - y_top +1;

   printf("Rayon Soleil = %ld\n", (new_dim1+new_dim2)/2);

   /*****************************/
   /* Allocation tableau Soleil */
   /*****************************/
   im_Soleil  = calloc(new_dim1, sizeof(long *));
   if (im_Soleil==NULL){
      printf("Probleme d'allocation pour l'image reduite (1er niveau)\n");
      return -1;
   }
   for (x=0; x<new_dim1; x++){
      im_Soleil[x] = calloc(new_dim2, sizeof(long));
      if (im_Soleil[x]==NULL){
         printf("Probleme d'allocation pour l'image reduite (2nd niveau)\n");
         return -2;
      }
   }

   /******************************/
   /* Remplissage tableau Soleil */
   /******************************/
   remplissage_Soleil(im_reduc, im_Soleil, new_dim1, new_dim2, x_left, y_top);

   /* ----------------------------------- */
   /* liberation de memoire               */
   /* ----------------------------------- */
   for (x = 0; x < dim1; ++x)
   {
       free(cube[x]) ;
       cube[x] = NULL ;
   }
   free(cube) ;
   cube = NULL ;

   for (x=0; x<dim1/ech; x++){
      free(im_reduc[x]);
      im_reduc[x] = NULL;
   }
   free(im_reduc);
   im_reduc = NULL;

   /* ----------------------------------------------------- */
   /* nbsec contient le nb de secondes depuis le 1/1/1970   */
   /* verifier la coherence avec la commande unix           */
   /* date -r <contenu de nbsec>                            */
   /* ----------------------------------------------------- */
   printf ( "Un clic sur l'image affiche les coordonnees du pixel et sa valeur.\n" ) ;
   printf ( "Appuyer sur la touche 'q' pour sortir.\n" ) ;

   AfficheFITSEtLitPixel ( im_Soleil, new_dim1, new_dim2, c_min, c_max, &x, &y ) ;

   /************************************/
   /* Calcul des coordonnes spheriques */
   /************************************/
   coordonnees_sph(new_dim1, new_dim2, x, y, &Rayon, longitude, lattitude);

   /* ----------------------------------------------------- */
   /* Fin de fonction + liberation de memoire               */
   /* ----------------------------------------------------- */
   for (x=0; x<new_dim1; x++){
      free(im_Soleil[x]);
      im_Soleil[x] = NULL;
   }
   free(im_Soleil);
   im_Soleil = NULL;


   return 0 ;
}



/* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
/* La fonction suivante permet de vider le tampon associe a un flot                     */
/*    -> l'adresse du flot est donnee en argument par l'utilisateur                     */
/*    -> la fonction ne renvoie rien. Aucune erreur n'est prise en compte               */
/* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
void viderbuffer ( FILE *stream )
{
   char cc = 0 ;
   while (cc != '\n' && cc != EOF)
   {
      cc = fgetc(stream) ;
   }
   return ;
}



/* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
/* La fonction suivante permet de remplacer '\n' par '\0' dans une chaine de caractere  */
/* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
void supp_saut_ligne ( char *chaine )
{
   char *c = NULL ;
   if( (c = strchr(chaine,'\n')) != NULL )
   {
      *c = '\0' ;
   }
   return ;
}
