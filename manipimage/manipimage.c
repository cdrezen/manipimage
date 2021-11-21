#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include "manipimage.h"

/* Une fonction tImage initImage(int haut, int larg) qui retourne une structure tImage dans laquelle les champs hauteur
et largeur sont initialis ́es avec les valeurs donn ́ees en param`etre et qui r ́ealise l’allocation du tableau img pour haut lignes de larg pixels.
Les champs type et maxval seront initialis ́es `a 0. */
tImage initImage(int haut, int larg, char typ[3], int vmax)
{
    tImage image;
    image.hauteur = haut;
    image.largeur = larg;
    strcpy(image.type, typ);
    image.maxval = vmax;

    int size = larg * haut * sizeof(tPixel);

    if (size == 0) 
    { 
        perror("L'image ne peut pas avoir 0 de hauteur ou de largeur.");
        return image; 
    }

    //  Allocation de la mémoire pour le tPixel**
    image.img = (tPixel**) malloc(size);

    //  Allocation de la mémoire pour les sous tableaux dans le tPixel**
    for (int i = 0; i < larg; i++)
    {
        image.img[i] = (tPixel*) malloc(size / larg);//taille = haut * sizeof(tPixel)
    }

    return image;
}
/* Ecrivez une fonction tImage copieImage(tImage im) qui r ́ealise la copie de l’image donn ́ee en param`etre
dans une nouvelle image initialis ́ee et allou ́ee avec les mˆemes caract ́eristiques
que l’image donn ́ee en param`etre et dans laquelle tous les pixels sont copi ́es. La fonction retourne la copie de l’image. */
tImage copieImage(tImage im)
{
    tImage image = initImage(im.hauteur, im.largeur, im.type, im.maxval);

    for (int i = 0; i < im.largeur; i++)
        for (int j = 0; j < im.hauteur; j++)
            image.img[i][j] = im.img[i][j];

    return image;
}

/* Ecrivez une fonction tImage chargePnm(char* fichier) qui charge dans une structure tImage
l’image contenue dans le fichier donn ́ee en param`etre. Une partie du code est donn ́ee, vous devez le compl ́eter.
Apr`es avoir pass ́e les lignes de commentaire, chaque donn ́ee  ́etant s ́epar ́ee de la suivante par un espace ou
un passage `a la ligne vous pouvez utiliser la fonction fscanf pour les r ́ecup ́erer. */

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

tImage chargePpm(char* fichier)
{
    int largeur = 0, hauteur = 0, vmax = 0;
    char str[256];

    FILE* pFile = fopen(fichier, "r");// Ouvre le ficher avec le mode de lecture.

    //  Copie le type P1/P2/P3.. + terminateur null et verifie si ça fini bien par NULL
    if (!fscanf(pFile, "%3s\n", str) || str[2] != 0)
    {
        perror("Type invalide ou à plus de 2 caractères.");
        return ImageVide;
    }

    //  Enregistre la ligne de commentaire sans écraser le tImage.type en l'écrivant à l'adresse + 3 de  la chaine str (3 = longueur de tImage.type)
    if (!fscanf(pFile, "%[^\n]\n", str + 3))
    {
        perror("Erreur lors de la lecture de la ligne de commentaire.");
        return ImageVide;
    }

    //  Copie la largeur, hauteur et la valeur maximum des pixels
    if (!fscanf(pFile, "%d %d\n%d\n", &largeur, &hauteur, &vmax))
    {
        perror("Erreur lors de la lecture des entiers de parametre.");
        return ImageVide;
    }

    if (vmax > 255)// Le type de tPixel.r .v et .b est unsigned char. Il faudrait faire un autre struct dans le .h pour prendre en charge des plus grande valeurs
    {
        perror("Valeurs supérieur à 255 non pris en charge.");
    }

    tImage image = initImage(hauteur, largeur, str, vmax);

    //  Lecture pixel par pixel des valeurs RVB:
    for (int i = 0; i < largeur * hauteur; i++)
    {
        unsigned char r = 0, v = 0, b = 0;
        if (fscanf(pFile, "%hhu\n", &r)//       lecture du rouge (%hhu -> unsigned char (0 à 255) enregisté sur 3 caractères)
            && fscanf(pFile, "%hhu\n", &v)//    lecture du vert
            && fscanf(pFile, "%hhu\n", &b))//   lecture du bleu
        {
            //  Calcul de la position (x, y) du pixel dans l'image par rapport à l'avancement dans la boucle.
            int x = i % largeur;
            int y = i / largeur;
            //  Affectation des valeurs avec les indexes correspondant à la position.
            image.img[x][y].r = r;
            image.img[x][y].v = v;
            image.img[x][y].b = b;
        }
        else
        {   //  Si il y a une erreur sur un des pixel lance une erreur et arrete la lecture du fichier.
            perror("Lecture des pixels: Fichier invalide ou corrompu ou mauvais nombre de valeurs.");
            break;
        }
    }

    fclose(pFile);

    return image;
}

void sauvePpm(char* nom, tImage im)
{
    //  On ouvre le fichier en mode écriture
    FILE* fichier = fopen(nom, "w");

    if (im.maxval > 255)// Le type de tPixel.r .v et .b est unsigned char. Il faudrait faire un autre struct dans le .h pour prendre en charge des plus grande valeurs
    {
        perror("Valeurs supérieur à 255 non pris en charge.");
    }

    fprintf(fichier, "%s\n", im.type);

    fprintf(fichier, "# Creator: George Lucas\n");

    fprintf(fichier, "%d %d\n", im.largeur, im.hauteur);

    fprintf(fichier, "%d\n", im.maxval);

    //  Ecriture des 3 couleurs pixel par pixel dans le fichier
    for (int i = 0; i < im.largeur * im.hauteur; i++)
    {
        //  Calcul de la position (x, y) du pixel dans l'image par rapport à l'avancement dans la boucle.
        int x = i % im.largeur;
        int y = i / im.largeur;
        //  Affectation des valeurs avec les indexes correspondant à la position.
        fprintf(fichier, "%hhu\n", im.img[x][y].r);
        fprintf(fichier, "%hhu\n", im.img[x][y].v);
        fprintf(fichier, "%hhu\n", im.img[x][y].b);
    }

    fclose(fichier);
}