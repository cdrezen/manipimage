
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

    image.img = (tPixel**)malloc(larg * haut * sizeof(tPixel));
    for (int i = 0; i < larg; i++)
    {
        image.img[i] = (tPixel*)malloc(haut * sizeof(tPixel));
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

    FILE* pFile = fopen(fichier, "r");

    fscanf(pFile, "%3s\n", str);//copie le type P1/P2/P3.. + terminateur null

    if (str[2] != 0) 
    { 
        perror("Type invalide (plus de 2 caractères)"); 
    }

    fscanf(pFile, "%[^\n]\n", str + 3);//enregistre la ligne de commentaire sans écraser le type en l'écrivant à l'adresse + 3 de str (3 = longueur de type)
    //printf("%s\n", str);//

    fscanf(pFile, "%d %d\n", &largeur, &hauteur);
    fscanf(pFile, "%d\n", &maxval);

    tImage image = initImage(hauteur, largeur, str, maxval);


    //printf("%s %i %i\n", image.type, image.largeur, image.hauteur);//test

    int i = 0, val = 0, x = 0, y = 0;
    while (fscanf(pFile, "%d\n", &val) != EOF)
    {
        switch (i % 3)
        {
        case 0:
            image.img[x][y].r = val;
            break;
        case 1:
            image.img[x][y].v = val;
            break;
        case 2:
            image.img[x][y].b = val;
            x++;
            break;
        }

        i++;

        if (x == largeur)
        {
            x = 0;
            y++;
        }
    }

    //printf("%i\n", image.img[0][0].r);
    //printf("%i\n", image.img[image.largeur - 1][image.hauteur-1].b);
    //printf("%d\n", (i - 1) / 3);


    fclose(pFile);

    return image;
}

tImage chargePpm(char* fichier)
{
    int largeur = 0, hauteur = 0, vmax = 0;
    char str[256];

    FILE* pFile = fopen(fichier, "r");

    fscanf(pFile, "%3s\n", str);//copie le type P1/P2/P3.. + terminateur null
    fscanf(pFile, "%[^\n]\n", str + 3);//skip la ligne de commentaire
    //printf("%s\n", str);//

    fscanf(pFile, "%d %d\n", &largeur, &hauteur);

    fscanf(pFile, "%s\n", &vmax);//skip la ligne "255"

    tImage image = initImage(hauteur, largeur, str, vmax);

    //printf("%s %i %i\n", image.type, image.largeur, image.hauteur);//test

    for (int i = 0; i < largeur * hauteur; i++)
    {
        unsigned char r = 0, v = 0, b = 0;
        if (fscanf(pFile, "%hhu\n", &r)
            && fscanf(pFile, "%hhu\n", &v)
            && fscanf(pFile, "%hhu\n", &b))
        {
            int x = i % largeur;
            int y = i / largeur;
            image.img[x][y].r = r;
            image.img[x][y].v = v;
            image.img[x][y].b = b;
        }
        else
        {
            perror("Fichier invalide ou corrompu.");// (%d)", ferror(pFile));
            break;
        }
    }

    //printf("%i\n", image.img[0][0].r);
    //printf("%i\n", image.img[image.largeur - 1][image.hauteur-1].b);
    //printf("%d\n", (i - 1) / 3);

    fclose(pFile);

    return image;
}

void sauvePpm(char* nom, tImage im)
{
    FILE* fichier = fopen(nom, "w");

    fprintf(fichier, "%s\n", im.type);

    fprintf(fichier, "# Creator: George Lucas\n");

    fprintf(fichier, "%d %d\n", im.largeur, im.hauteur);

    fprintf(fichier, "255\n");

    for (int i = 0; i < im.largeur * im.hauteur; i++)
    {
        int x = i % im.largeur;
        int y = i / im.largeur;
        fprintf(fichier, "%hhu\n", im.img[x][y].r);
        fprintf(fichier, "%hhu\n", im.img[x][y].v);
        fprintf(fichier, "%hhu\n", im.img[x][y].b);
    }

    fclose(fichier);
}