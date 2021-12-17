//40007337  Drezen Corentin
//40003483  Pallard Benoît

//Fonction min et max
#define MIN(a,b) (((a)<(b))?(a):(b))//si a<b retourne a sinon retourne b
#define MAX(a,b) (((a)>(b))?(a):(b))

// ************************************************
// Definition des types
// ************************************************

// Le type tPixel
typedef struct pixel {
    unsigned char r;
    unsigned char v;
    unsigned char b;
} tPixel;

// Le type representant une image en memoire
typedef struct image {
    int hauteur;        // Hauteur en pixels
    int largeur;        // Largeur en pixels
    char type[3];       // Type de l'image pnm P1 P2 P3 P4 ou P6
    int maxval;         // Valeur maximale de l'intensite d'une couleur
    tPixel** img;       // Le tableau des pixels
} tImage;

static const struct image ImageVide = { 0, 0 };

tImage initImage(int haut, int larg, char typ[3], int vmax);

tImage copieImage(tImage im);

tImage chargePnm(char* fichier);

void sauvePnm(char* nom, tImage im);

/// 1.2

float luminance(tPixel p);

tImage niveauGris(tImage im);

tPixel floumoy(tImage im, int i, int j, int r);

tImage flou(tImage im, int r);

tImage contours(tImage im);

/// 1.3

unsigned char fusionOctets(unsigned char octet1, unsigned char octet2);

tImage cacheImage(tImage originale, tImage adissimuler);

tImage reveleImage(tImage im);

tPixel cacheCarac(tPixel pix, char c);

char extraitCaract(tPixel pix);

tImage cacheTexte(tImage im, char* lefichier);

void reveleTexte(tImage im, char* fichExtrait);