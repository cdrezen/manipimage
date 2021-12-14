#include "stdio.h"
#include "manipimage.h"

/* Ecrivez une fonction tImage chargePnm(char* fichier) qui charge dans une structure tImage
l’image contenue dans le fichier donn ́ee en param`etre. Une partie du code est donn ́ee, vous devez le compl ́eter.
Apr`es avoir pass ́e les lignes de commentaire, chaque donn ́ee  ́etant s ́epar ́ee de la suivante par un espace ou
un passage `a la ligne vous pouvez utiliser la fonction fscanf pour les r ́ecup ́erer. */

enum
{
    ROUGE,
    VERT,
    BLEU
};

tImage chargePpm1(char* fichier)
{
    int largeur = 0, hauteur = 0, maxval = 0;
    char str[256];

    FILE* pFile = fopen(fichier, "r");// Ouvre le ficher avec le mode de lecture.

    //  Copie le type P1/P2/P3.. + terminateur null
    fscanf(pFile, "%3s\n", str);
    //  Enregistre la ligne de commentaire sans écraser le type en l'écrivant à l'adresse + 3 de str(3 = longueur de tImage.type)
    //  Si on fait un strcpy avec cette chaine comme source, ça s'arettera au terminateur NULL. Cela evite de déclarer une 2eme chaine seulement pour une ligne ignorée
    fscanf(pFile, "%[^\n]\n", str + 3);

    //  Copie la largeur, hauteur et la valeur maximum des pixels
    fscanf(pFile, "%d %d\n%d\n", &largeur, &hauteur, &maxval);

    tImage image = initImage(hauteur, largeur, str, maxval);

    int i = 0, val = 0, x = 0, y = 0;
    //  Lire les entiers tant que l'on est pas arrivé à la fin du fichier
    while (fscanf(pFile, "%d\n", &val) != EOF)
    {
        switch (i % 3)// Les valeurs rouge vert et bleu sont à la suite donc on peut faire 'position % 3' pour retrouver à quelle couleur les valeurs correspondent
        {
        case ROUGE:
            image.img[x][y].r = val;
            break;
        case VERT:
            image.img[x][y].v = val;
            break;
        case BLEU:
            image.img[x][y].b = val;
            x++;// Après le bleu on passe à un nouveau pixel
            break;
        }

        i++;

        //  Si on a fini une ligne de pixel on passe à celle d'en dessous
        if (x == largeur)
        {
            x = 0;
            y++;
        }
    }

    fclose(pFile);

    return image;
}
