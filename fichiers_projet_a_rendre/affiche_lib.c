/* ==================================================================================== */
/* =============================== En tetes necessaires =============================== */
/* ==================================================================================== */
#include "affiche_lib.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <err.h>
#define __USE_XOPEN
#include <time.h>
#include <fitsio.h>
#include <math.h>
#include <float.h>
#include <X11/keysym.h>
#include <X11/XKBlib.h>
#include <X11/Xutil.h>
#include <X11/cursorfont.h>



/* ==================================================================================== */
/* ============================= Définitions de fonctions ============================= */
/* ==================================================================================== */



/* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
/* La fonction suivante definit une nouvelle table de couleurs contenant numcol couleurs*/
/*    -> Le noir  est associe a la couleur img_min                                      */
/*    -> Le blanc est associe a la couleur img_max                                      */
/* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
XColor *EchelleGris ( Display * display, int numcol )
{
   int       i        = 0 ;
   long      gris     = 0 ;
   XColor   *tabcolor = NULL;
   Colormap  colormap ;

   colormap = DefaultColormap ( display, DefaultScreen(display) ) ;
   tabcolor = (XColor *) malloc ( numcol * sizeof(XColor) ) ;
   if (tabcolor == NULL)
   {
      return NULL ;
   }

   for (i = 0 ; i < numcol ; i++)
   {
      gris = (float) i * 65535 / numcol ;

      tabcolor[i].red   = gris ;
      tabcolor[i].green = gris ;
      tabcolor[i].blue  = gris ;
      if ( XAllocColor ( display, colormap, &tabcolor[i] ) == 0 )
      {
         printf("XAllocColor - allocation of %ld color failed.\n", gris) ;
      }
   }

   return tabcolor ;
}



/* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
/* La fonction suivante affiche la table de couleurs sur toute la largeur de la fenêtre */
/* sur un rectangle de coordonnées (0, dim_y) , (dim_x, dim_y+h_echelle)                */
/* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
void AfficheEchelleGris( int h_echelle, int dim_x, int dim_y, int numcol, Display * display,
                         XColor * tabcolor, Window window )
{
   int i   = 0 ;
   int val = 0 ;
   GC  gc ;
   
   gc = DefaultGC ( display, DefaultScreen(display) )  ;
   
   for (i = 0 ; i < dim_x ; i++)
   {
      val = (float) i * numcol / dim_x ;
      XSetForeground ( display, gc, tabcolor[val].pixel ) ;
      XDrawLine ( display, window, gc, i, dim_y, i, dim_y + h_echelle ) ;
   }
   return ;
}



/* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
/* La fonction suivante affiche une image selon la table de couleurs tabcolor           */
/* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
void AfficheImage( long **cube, int dim_x, int dim_y, int nb_col, int img_min, int img_max,
                   Display *display, XColor *tabcolor, Window window )
                   
{
   int i     = 0 ; 
   int j     = 0 ;
   int color = 0 ;
   GC  gc ;

   gc = DefaultGC ( display, DefaultScreen(display) ) ;

   for (i = 0 ; i < dim_x ; i++)
   {
      for (j = 0 ; j < dim_y ; j++)
      {
         color = cube[i][j] ;

         /* ----------------------------------------------- */
         /* Seuillage des couleurs entre img_min et img_max */
         /* ----------------------------------------------- */
         if ( color < img_min ) color = img_min ;
         if ( color > img_max ) color = img_max ;

         /* ----------------------------------------------- */
         /* transformation de la couleur en un nombre entre */
         /* 0 et nb_col-1                                   */
         /* ----------------------------------------------- */
         color = (float) ((color - img_min) * (nb_col-1)) / (img_max - img_min) ;

         /* ----------------------------------------------- */
         /* modification de la couleur de dessin            */
         /* ----------------------------------------------- */
         XSetForeground ( display, gc, tabcolor[color].pixel ) ;

         /* ----------------------------------------------- */
         /* afficher le pixel avec la couleur choisie       */
         /* ----------------------------------------------- */
         XDrawPoint ( display, window, gc, i, j ) ;
      }
   }
   return ;
}



/* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
/* La fonction suivante lit l'image et renvoie le tableau                               */
/* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
long **LectureFITS (char *filename, long *dim1, long *dim2, long *nbsec )
{
   long       **cube         = NULL ;
   int         i             = 0    ;
   int         num           = 0    ;
   fitsfile   *fptr          = NULL ;
   int         statut        = 0    ;
   int         bitpix        = 0    ;
   int         naxis         = 0    ;
   int         nullval       = 0    ;
   int         anynull       = 0    ;
   long        firstpix[3]   = {0}  ;
   long        naxes[3]      = {0}  ;
   struct tm   time ;
   char        date_obs[256] = {'\0'} ;
   char        comment[256]  = {'\0'} ;
   char        nbsec_s[20]   = {'\0'} ;
   char        mois[12][20]  = {"Janvier", "Fevrier", "Mars", "Avril", "Mai", "Juin",
                                "Juillet", "Aout", "Septembre", "Octobre","Novembre", 
                                "Decembre"} ;

   /* ----------------------------------------------------- */
   /* Ouverture du fichier                                  */
   /* ----------------------------------------------------- */
   statut = 0;
   printf ( "Ouverture de l'image '%s' ...\n", filename ) ;
   fits_open_file ( &fptr, filename, READONLY, &statut ) ;
   if (statut != 0)
   {
      fits_report_error(stderr, statut);
      return NULL ;
   }

   /* ----------------------------------------------------- */
   /* Lecture de la date d'observation                      */
   /* ----------------------------------------------------- */
   statut = 0;
   fits_read_keyword ( fptr, "DATE_OBS", date_obs, comment, &statut ) ;
   if ( statut )
   {
      fits_report_error ( stderr, statut ) ;
   }

   /* ----------------------------------------------------- */
   /* Conversion de la chaine de caracteres en structure tm */
   /* ----------------------------------------------------- */
   strptime ( date_obs, "'%Y-%m-%dT%H:%M:%S.000'", &time ) ;
   printf ( "L'image a ete prise le %d %s %d, a  %02d:%02d:%02d\n",
            time.tm_mday, mois[time.tm_mon], 1900 + time.tm_year,
            time.tm_hour, time.tm_min, time.tm_sec ) ;

   /* ----------------------------------------------------- */
   /* Conversion de la structure tm en nb de secondes       */
   /* ----------------------------------------------------- */
   strftime ( nbsec_s, 20, "%s", &time ) ;
   *nbsec = strtol ( nbsec_s, NULL, 10 ) ;

   /* ----------------------------------------------------- */
   /* Lecture des parametres de l'image                     */
   /*  -> naxis : nb de dimensions dans le FITS             */
   /*  -> et lecture des valeurs de naxis                   */
   /* ----------------------------------------------------- */
   statut = 0;
   fits_get_img_param ( fptr, 3,  &bitpix, &naxis, naxes, &statut ) ;
   if ( statut )
   {
      fits_report_error ( stderr, statut ) ;
   }

   if ( naxis == 3 )
   {
      printf ( "Il y a %ld images dans le fichier, laquelle prendre ?\n", naxes[2] ) ;
      /* -------------------------------------------------- */
      /* boucle qui tourne tant que l'on ne donne pas un    */
      /* numero d'image correct (entre 1 et le nb d'images) */
      /* -------------------------------------------------- */
      while (1)
      {
         scanf ( "%d", &num ) ;
         /* num = 1; */
         if ( (num < 1) || (num > naxes[2]) )
         {
            printf ( "valeur incorrecte (entre 1 et %ld)\n", naxes[2] ) ;
         }
         else
         {
            break ;
         }
      }
   }
   else if ( naxis == 2 )
   {
      num = 1 ;
   }

   /* ----------------------------------------------------- */
   /* Allocation du tableau pouvant contenir toute l'image  */
   /* ----------------------------------------------------- */
   cube = (long **) malloc ( naxes[1] * sizeof(long *) ) ;
   if ( cube == NULL )
   {
      err ( -1, "%s:%d malloc failed", __FILE__, __LINE__ ) ;
   }

   for (i = 0 ; i < naxes[1] ; i++)
   {
      cube[i] = (long *) malloc ( naxes[0] * sizeof(long) ) ;
      if ( cube[i] == NULL )
      {
         err ( -1, "%s:%d malloc failed", __FILE__, __LINE__ ) ;
      }
   }

   /* ----------------------------------------------------- */
   /* On positionne le curseur sur le 1er pixel a lire      */
   /* ----------------------------------------------------- */
   firstpix[0] = 1 ;
   firstpix[1] = 1 ;
   firstpix[2] = num ;

   /* ----------------------------------------------------- */
   /* Lecture de l'image ligne par ligne                    */
   /* ----------------------------------------------------- */
   for (i = naxes[1] - 1 ; i >=0 ; i--)
   {
      firstpix[1] = naxes[1] - i;
      statut      = 0;
      nullval     = -999;
      fits_read_pix ( fptr, TLONG, firstpix,  naxes[0], &nullval, cube[i], &anynull, &statut ) ;
      if ( statut )
      {
         fits_report_error ( stderr, statut ) ;
      }
   }

   /* ----------------------------------------------------- */
   /* Transposition de l'image car la librairie ctisio      */
   /* permute les 2 dimensions                              */
   /* ----------------------------------------------------- */
   cube = transpose ( cube, naxes[1], naxes[0] ) ;

   *dim1 = naxes[0] ;
   *dim2 = naxes[1] ;

   fits_close_file ( fptr, &statut ) ;
   if ( statut )
   {
      fits_report_error ( stderr, statut ) ;
   }

   return cube;
}



/* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
/* La fonction suivante affiche une image de dimension (dim1, dim2), en effectuant un   */
/* seuillage des couleurs entre les valeurs min et max données en entrée                */
/* -> Elle attend un clic pour renvoyer dans *x et *y les coordonnées cliquées          */
/* -> La sortie de la procédure s'effectue en appuyant sur la touche 'q'                */
/* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
void AfficheFITSEtLitPixel ( long **cube, long dim1, long dim2, int min, int max, int *x, int *y )
{
   int       screen_num  = 0 ;
   int       numcol      = 0 ;
   int       h_echelle   = 0 ;
   char     *key_pressed = NULL ;
   Display  *display     = NULL ;
   Window    window ;
   XEvent    event ;
   XColor   *tabcolor    = NULL ;
   KeySym    key_symbol ;
   Cursor    cross_cursor ;

   /* ----------------------------------------------------- */
   /* Ouvrir la connexion avec le serveur                   */
   /* ----------------------------------------------------- */
   display = XOpenDisplay ( NULL ) ;
   if ( display == NULL )
   {
      printf ( "Impossible d'ouvrir le Display\n" ) ;
      exit ( 1 ) ;
   }
   screen_num = DefaultScreen ( display ) ;

   /* ----------------------------------------------------- */
   /* Hauteur réservée pour afficher l'échelle des gris     */
   /* ----------------------------------------------------- */
   h_echelle = 30 ;

   /* ----------------------------------------------------- */
   /* Creer la fenetre                                      */
   /* ----------------------------------------------------- */
   window = XCreateSimpleWindow (
               display,
               RootWindow(display, screen_num),
               0, 0,                               /* Position de la fenetre */
               dim1, dim2 + h_echelle,             /* Taille de la fenetre   */
               1,                                  /* Bordure de la fenetre  */
               BlackPixel(display, screen_num),    /* Couleur de la bordure  */
               WhitePixel(display, screen_num) ) ; /* Couleur du fond        */

   /* ----------------------------------------------------- */
   /* Choisir les evenements qui nous interessent           */
   /* Imperativement avant l'affichage !                    */
   /* ----------------------------------------------------- */
   XSelectInput ( display, window, ExposureMask | KeyPressMask | ButtonReleaseMask | ButtonPressMask ) ;

   /* ----------------------------------------------------- */
   /* Choisir le type de curseur                            */
   /* ----------------------------------------------------- */
   cross_cursor = XCreateFontCursor( display, XC_pencil ) ;
   XDefineCursor ( display, window, cross_cursor ) ;

   /* ----------------------------------------------------- */
   /* Afficher la fenetre                                   */
   /* ----------------------------------------------------- */
   XMapWindow ( display, window ) ;

   /* ----------------------------------------------------- */
   /* Allocation d'une table comportant 32768 couleurs      */
   /* le noir  correspondant a la valeur de pixel 0         */
   /* le blanc correspondant a la valeur de pixel 32767     */
   /* ----------------------------------------------------- */
   numcol = 256 ;
   tabcolor = EchelleGris ( display, numcol ) ;
   if ( tabcolor == NULL )
   {
      printf( "Impossible d'allouer la table des couleurs\n" ) ;
      exit ( 1 ) ;
   }

   /* ----------------------------------------------------- */
   /* Boucle des evenements                                 */
   /* ----------------------------------------------------- */
   while (1)
   {
      XNextEvent ( display, &event ) ;

      /* -------------------------------------------------- */
      /* Dessiner ou redessiner la fenetre                  */
      /* -------------------------------------------------- */
      if (event.type == Expose)
      {
         AfficheImage ( cube, dim1, dim2, numcol, min, max, display, tabcolor, window ) ;
         AfficheEchelleGris ( h_echelle, dim1, dim2, numcol, display, tabcolor, window ) ;
      }

      /* -------------------------------------------------- */
      /* Bouton de la souris presse et relache              */
      /* -------------------------------------------------- */
      if ( event.type == ButtonRelease )
      {
         *x = event.xbutton.x ;
         *y = event.xbutton.y ;
         printf ( "Coordonnees (%d,%d) - Valeur du pixel : %ld\n", *x, *y, cube[*x][*y] ) ;
      }

      if ( event.type == KeyPress )
      {
         /* traduire le "key code" en "key symbol"                                */
         /* since XKeycodeToKeysym is deprecated, replace with XkbKeycodeToKeysym */
         /* key_symbol = XKeycodeToKeysym ( display, event.xkey.keycode, 0 ) ;    */
         key_symbol = XkbKeycodeToKeysym ( display, event.xkey.keycode, 0, 0 ) ;
         key_pressed = XKeysymToString ( key_symbol ) ;

         /* Sortir si la touche "q" est pressee. */
         if ( strcmp(key_pressed, "q") == 0 )
         {
            break ;
         }
      }
   }

   /* ----------------------------------------------------- */
   /* Couper la connexion avec le serveur                   */
   /* ----------------------------------------------------- */
   XCloseDisplay ( display ) ;

   return ;
}



/* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
/* La fonction suivante transpose une matrice donnée en entrée                          */
/* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
long **transpose(long **mat, int dim1, int dim2)
{
   int    i    = 0 ;
   int    j    = 0 ;
   long   tmp  = 0 ;
   long **tmat = NULL ;

   if ( dim1 == dim2 )
   {
      /* -------------------------------------------------- */
      /* Si la matrice est carrée, on permute simplement    */
      /* les éléments de part et d'autre de la diagonale    */
      /* -------------------------------------------------- */
      for (i = 0 ; i < dim1 ; i++)
      {
         for (j = 0 ; j < i ; j++)
         {
            tmp = mat[i][j] ;
            mat[i][j] = mat[j][i] ;
            mat[j][i] = tmp ;
         }
      }
      return mat;
   }
   else
   {
      /* -------------------------------------------------- */
      /* Sinon, on alloue une nouvelle matrice de           */
      /* dimensions (dim2, dim1)                            */
      /* -------------------------------------------------- */
      tmat = (long **) malloc ( dim2 * sizeof(long *) ) ;
      if ( tmat == NULL )
      {
         err ( -1, "%s:%d malloc failed", __FILE__, __LINE__ ) ;
      }

      for (i = 0 ; i < dim2 ; i++)
      {
         tmat[i] = (long *) malloc ( dim1 * sizeof(long) ) ;
         if ( tmat[i] == NULL )
         {
            err ( -1, "%s:%d malloc failed", __FILE__, __LINE__ ) ;
         }
      }

      /* -------------------------------------------------- */
      /* Remplissage de la transposée                       */
      /* -------------------------------------------------- */
      for (i = 0 ; i < dim1 ; i++)
      {
         for (j = 0 ; j < dim2 ; j++)
         {
            tmat[j][i] = mat[i][j] ;
         }
      }

      /* -------------------------------------------------- */
      /* Désallocation de la matrice originale              */
      /* -------------------------------------------------- */
      for (i = 0 ; i < dim1 ; i++)
      {
         free ( mat[i] ) ;
         mat[i] = NULL ;
      }
      free ( mat ) ;
      mat = NULL ;
   }

   return tmat ;
}
