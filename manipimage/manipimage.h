// ************************************************
// Definition des types
// ************************************************

// Un enum qui servira a identifier les couleur selon la ligne
enum
{
    ROUGE,
    VERT,
    BLEU
};

// Le type tPixel
typedef struct pixel {
    unsigned char r;
    unsigned char v;
    unsigned char b;
} tPixel;

// Le type tPixelS pour impl�menter des valeurs de couleurs plus grandes plus tard
typedef struct pixelS {
    unsigned short r;
    unsigned short v;
    unsigned short b;
} tPixelS;

// Le type representant une image en memoire
typedef struct image {
    int hauteur;        // Hauteur en pixels
    int largeur;        // Largeur en pixels
    char type[3];       // Type de l'image pnm P1 P2 P3 P4 ou P6
    int maxval;         // Valeur maximale de l'intensite d'une couleur
    tPixel** img;       // Le tableau des pixels
    //tPixelS** imgS;     // Le tableau des pixels ushort
} tImage;

static const struct image ImageVide = { 0 };

tImage initImage(int haut, int larg, char typ[3], int vmax);

tImage copieImage(tImage im);

tImage chargePpm1(char* fichier);

tImage chargePpm(char* fichier);

void sauvePpm(char* nom, tImage im);
