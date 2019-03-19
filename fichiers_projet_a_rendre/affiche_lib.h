/* ==================================================================================== */
/* =============================== En têtes nécessaires =============================== */
/* ==================================================================================== */
#include <X11/Xlib.h>

/* ==================================================================================== */
/* ============================== Prototypes de fonctions ============================= */
/* ==================================================================================== */
XColor *EchelleGris ( Display * display, int num) ;

void    AfficheImage ( long **cube, int dim_x, int dim_y, int nb_col, int img_min, int img_max,
                       Display * display, XColor * tabcolor, Window window ) ;

long **LectureFITS ( char *filename, long *dim1, long *dim2, long *nbsec ) ;
    
void    AfficheFITSEtLitPixel ( long **cube, long dim1, long dim2, int img_min, int img_max, int *x, int *y ) ;
  
long  **transpose ( long **mat, int dim1, int dim2 ) ;
