// ************************************************
// Definition des types
// ************************************************

// Le type tPixel
typedef struct pixel {
    int r;
    int v;
    int b;
} tPixel;


// Le type representant une image en memoire
typedef struct image {
    int hauteur;        // Hauteur en pixels
    int largeur;        // Largeur en pixels
    char type[3];       // Type de l'image pnm P1 P2 P3 P4 ou P6
    int maxval;         // Valeur maximale de l'intensite d'une couleur
    tPixel** img;       // Le tableau des pixels
} tImage;

tImage initImage(int haut, int larg);

tImage copieImage(tImage im);

tImage chargePpm1(char* fichier);

tImage chargePpm(char* fichier);

void sauvePpm(char* nom, tImage im);
