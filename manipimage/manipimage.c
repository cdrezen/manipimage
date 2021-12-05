//40007337  Drezen Corentin
//40003483  Pallard Benoît

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "manipimage.h"

/* Une fonction tImage initImage(int haut, int larg) qui retourne une structure tImage dans laquelle les champs hauteur
et largeur sont initialis ́es avec les valeurs donn ́ees en param`etre et qui r ́ealise l’allocation du tableau img pour haut lignes de larg pixels.
Les champs type et maxval seront initialis ́es `a 0. */
tImage initImage(int haut, int larg, char typ[3], int vmax)
{
    tImage image;
    image.hauteur = haut;
    image.largeur = larg;
    strcpy(image.type, typ);//copie de la chaine de char 'typ' dans 'image.type'
    image.maxval = vmax;

    if (larg * haut  == 0) 
    { 
        perror("L'image ne peut pas avoir 0 de hauteur ou de largeur.");
        return image; 
    }

    //Allocation de la mémoire pour le tPixel**
    image.img = malloc(haut * sizeof(tPixel*));

    // Allocation de la mémoire pour les sous tableaux dans le tPixel**
    for (int i = 0; i < haut; i++)
    {
        image.img[i] = malloc(larg * sizeof(tPixel));
    }

    return image;
}
/* Ecrivez une fonction tImage copieImage(tImage im) qui r ́ealise la copie de l’image donn ́ee en param`etre
dans une nouvelle image initialis ́ee et allou ́ee avec les mˆemes caract ́eristiques
que l’image donn ́ee en param`etre et dans laquelle tous les pixels sont copi ́es. La fonction retourne la copie de l’image. */
tImage copieImage(tImage im)
{
    //  Initialisation d'une image avec les mêmes paramettres
    tImage image = initImage(im.hauteur, im.largeur, im.type, im.maxval);

    //  Copie des valeurs des pixels dans la nouvelle image
    for (int i = 0; i < im.hauteur; i++)
        for (int j = 0; j < im.largeur; j++)
            image.img[i][j] = im.img[i][j];

    return image;
}

/* Ecrivez une fonction tImage chargePnm(char* fichier) qui charge dans une structure tImage
l’image contenue dans le fichier donn ́ee en param`etre. Une partie du code est donn ́ee, vous devez le compl ́eter.
Apr`es avoir pass ́e les lignes de commentaire, chaque donn ́ee  ́etant s ́epar ́ee de la suivante par un espace ou
un passage `a la ligne vous pouvez utiliser la fonction fscanf pour les r ́ecup ́erer. */

tImage chargePnm(char* fichier)
{
    int largeur = 0, hauteur = 0, vmax = 0;
    char str[3];

    FILE* pFile;// Ouvre le ficher avec le mode de lecture.

    if (!(pFile = fopen(fichier, "r")))
    {
        fprintf(stderr, "\nErreur dans sauvePnm : %s innaccessible", fichier);
        return ImageVide;
    }

    //  Copie le type P1/P2/P3.. + terminateur null et verifie si ça fini bien par NULL
    if (!fscanf(pFile, "%3s\n", str) || str[2] != 0)
    {
        perror("Type invalide ou à plus de 2 caractères.");
        return ImageVide;
    }

    //  Saute le commentaire, %* : pas stocké dans une variable, [^\n] : pattern 'tout sauf /n'
    if (fscanf(pFile, "%*[^\n]\n") != 0)
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
    for (int y = 0; y < hauteur; y++)
    {
        for (int x = 0; x < largeur; x++)
        {
            unsigned char r = 0, v = 0, b = 0;
            if (fscanf(pFile, "%hhu", &r)     //       lecture du rouge (%hhu -> unsigned char (0 à 255) enregisté sur 3 caractères)
                && fscanf(pFile, "%hhu", &v)  //    lecture du vert
                && fscanf(pFile, "%hhu", &b)) //   lecture du bleu
            {
                //  Affectation des valeurs avec les indexes correspondant à la position.
                image.img[y][x].r = r;

                if (image.type[1] <= '2')
                    continue;

                image.img[y][x].v = v;
                image.img[y][x].b = b;
            }
            else
            { //  Si il y a une erreur sur un des pixel lance une erreur et arrete la lecture du fichier.
                perror("Lecture des pixels: Fichier invalide ou corrompu ou mauvais nombre de valeurs.");
                break;
            }
        }
    }

    fclose(pFile);

    return image;
}

/*Ecrivez une fonction void sauvePpm(char* nom, tImage im) qui cr ́eer un fichier .ppm dont le nom est donn ́e en
param`etre et dans lequel vous sauvez le contenu de la structure tImage donn ́ee en param`etre en respectant le format d’un
fichier .ppm.
Vous ajouterez dans ce fichier une ligne de commentaire contenant votre nom et pr ́enom pour renseigner le cr ́eateur du
fichier.*/
void sauvePnm(char* nom, tImage im)
{
    //  On ouvre le fichier en mode écriture
    FILE* fichier;

    if (!(fichier = fopen(nom, "w")))
    {
        fprintf(stderr, "\nErreur dans sauvePnm : %s innaccessible", nom);
        return;
    }

    if (im.maxval > 255)// Le type de tPixel.r .v et .b est unsigned char. Il faudrait faire un autre struct dans le .h pour prendre en charge des plus grande valeurs
    {
        perror("Valeurs supérieur à 255 non pris en charge.");
    }

    fprintf(fichier, "%s\n", im.type);

    fprintf(fichier, "# Creator: George Lucas\n");

    fprintf(fichier, "%d %d\n", im.largeur, im.hauteur);

    fprintf(fichier, "%d\n", im.maxval);

    //  Ecriture des 3 couleurs pixel par pixel dans le fichier
    for (int y = 0; y < im.hauteur; y++)
    {
        for (int x = 0; x < im.largeur; x++)
        {
            //  Affectation des valeurs avec les indexes correspondant à la position.
            fprintf(fichier, "%hhu\n", im.img[y][x].r);

            if (im.type[1] <= '2')
                continue; // Seulement une valeur par pixel pour P2 et P1

            fprintf(fichier, "%hhu\n", im.img[y][x].v);
            fprintf(fichier, "%hhu\n", im.img[y][x].b);
        }
    }

    fclose(fichier);
}

/// 1.2 Manipulation d’une image en memoire

// calcule la valeur de niveau de gris
float luminance(tPixel p)
{
    return (0.2125f * p.r) + (0.7154f * p.v) + (0.0721f * p.b);// Gris = 0.2125 Rouge + 0.7154 Vert + 0.0721 Bleu       xf = float
}

tImage niveauGris(tImage im)
{
    // Faire une copie et remplacer le type par P2 (niveau de gris) qui ne prend qu'une valeur par pixel cf https://fr.wikipedia.org/wiki/Portable_pixmap#PGM
    // sauvepPnm et chargePnm modifiés pour gérer les P2
    tImage image = initImage(im.hauteur, im.largeur, "P2", im.maxval);

    //  Copie des valeurs des pixels luminant dans la nouvelle image
    for (int i = 0; i < im.hauteur; i++)
    {
        for (int j = 0; j < im.largeur; j++)
        {
            // Change la valeur en niveau de gris

            unsigned char lum = (unsigned char)luminance(im.img[i][j]);// calcule la valeur de niveau de gris
            image.img[i][j].r = lum;
            // inutile pour les autres valeurs mais permeterai aussi de l'enregistrer en P3 facilement :
            image.img[i][j].v = lum;//
            image.img[i][j].b = lum;//
        }
    }

    return image;
}

tPixel floumoy(tImage im, int i, int j, int r)
{
    int R = 0, V = 0, B = 0;
    int n = 0;

    // itère dans toute le carré de côté 2r+1(ou moins si i ou j < 0 ou > largeur ou hauteur) et fait les sommes pour chaque couleurs 
    for(int y = MAX(i - r, 0); y < MIN(i + r, im.hauteur); y++) // la définition de MIN et MAX est dans le .h, équivalente à celle de python ou de c++      
    {                                                           // MIN et MAX sont là pour empecher d'utiliser des indexes hors des bords de l'image
        for(int x = MAX(j - r, 0); x < MIN(j + r, im.largeur); x++)
        {
            // incrémente les sommes des couleurs
            R += im.img[y][x].r;
            V += im.img[y][x].v;
            B += im.img[y][x].b;
            n++; // incrémente de diviseur
        }
    }

    if(n > 0) // divise chaque somme de couleurs par le nombre de pixels -> moyenne
    { 
        R /= n; 
        V /= n; 
        B /= n; 
    }

    return (tPixel)//renvoie un pixel avec comme couleurs les moyennes des couleurs du carré 
    { 
        (unsigned char)R, 
        (unsigned char)V, 
        (unsigned char)B 
    };
}

tImage flou(tImage im, int r)
{
    // on fait une copie de l'image car on va se servir de l'autre comme référence et celle-ci va être modifiée
    tImage image = copieImage(im); 

    for (int i = 0; i < im.hauteur; i++)
    {
        for (int j = 0; j < im.largeur; j++)
        {
            // chaque pixel est remplacé par la moyenne des pixels dans un carré de rayon r
            image.img[i][j] = floumoy(im, i, j, r);
        }
    }

    return image;
}

tImage contours(tImage im)
{
    // on fait une copie de l'image car on va s'en servir et on ne veut pas modifié celle qui est donné en argument
    tImage image = copieImage(im);

    for (int i = 0; i < im.hauteur; i++)
    {
        for (int j = 0; j < im.largeur; j++)
        {   
            // On remplace chaque couleur du pixel par sa valeur de flou gaussien

            tPixel p = floumoy(im, i, j, 2); // Obtient le pixel flouté Fij de rayon 2  
            image.img[i][j].r = 255 - abs(image.img[i][j].r - p.r);// Pij.r = blanc - | Pij.r - Fij.r |     blanc = 255
            image.img[i][j].v = 255 - abs(image.img[i][j].v - p.v);// Pij.v = blanc - | Pij.v - Fij.v |
            image.img[i][j].b = 255 - abs(image.img[i][j].b - p.b);// Pij.b = blanc - | Pij.b - Fij.b |
        }
    }

    return image;
}

/// 1.3 Steganographie

unsigned char fusionOctets(unsigned char octet1, unsigned char octet2)
{
    return (octet1 & 0xf0) | (octet2 >> 4);// 0xf0 = 11110000
    // met à 0 les bits de poids faible de l'octet et garde le poid fort avec l'opérateur &: 11111010 & 11110000 = 11110000
    // octet2 >> 4 shift :  'pousse' les 4 bits de poids fort vers la droite (il deviennent poid faible) ex: 11110000 devient 00001111
    // octet1 | x : fusion des octet par la l'opération OR (1|Ø=1, 1|1=1, seul 0|Ø=0)ex:
    // 11110100
    //|00000111     //https://en.wikipedia.org/wiki/Bitwise_operations_in_C
    // 11110111
}

tImage cacheImage(tImage originale, tImage adissimuler)
{
    tImage image = copieImage(originale);

    if(adissimuler.largeur > image.hauteur || adissimuler.hauteur > image.hauteur)
    {
        printf("L'image à dissimuler doit avoir des dimmensions égales ou plus petites que la cible.");
        return image;
    }

    for (int i = 0; i < adissimuler.hauteur; i++)
    {
        for (int j = 0; j < adissimuler.largeur; j++)
        {
            image.img[i][j].r = fusionOctets(image.img[i][j].r, adissimuler.img[i][j].r);
            image.img[i][j].v = fusionOctets(image.img[i][j].v, adissimuler.img[i][j].v);
            image.img[i][j].b = fusionOctets(image.img[i][j].b, adissimuler.img[i][j].b);
        }
    }

    return image;
}

tImage reveleImage(tImage im)
{
    tImage image = initImage(im.hauteur, im.largeur, im.type, im.maxval);
    for (int i = 0; i < im.hauteur; i++)
    {
        for (int j = 0; j < im.largeur; j++)
        {
            image.img[i][j].r = im.img[i][j].r << 4;
            image.img[i][j].v = im.img[i][j].v << 4;
            image.img[i][j].b = im.img[i][j].b << 4;
        }
    }

    return image;
}

tPixel cacheCarac(tPixel pix, char c)
{
    tPixel p = pix;
    //0xf0 = 11110000. XXXXXXX & 0xf0= XXXX0000
    p.r = (p.r & 0xf0) | (c >> 4); //  XXXXABCD >> 4 = 0000XXXX
    p.v = (p.r & 0xf0) | (c & 0xf);
    // 0xf = 00001111. XXXXXXX & 0xf = 0000XXXX
    return p;
}

char extraitCaract(tPixel pix)
{
    return (pix.r << 4) | (pix.v & 0xf);
    //      pixr0000   '+'   0000pixv
}

tImage cacheTexte(tImage im, char* lefichier)
{
    FILE* f;
    tImage image = copieImage(im);
    int tailleDisponible = image.largeur * image.hauteur - 2;
    char* str;
    
    // Ouverture du fichier texte en lecture
    // En cas d'erreur de lecture du fichier on affiche un message d'erreur
    // et on retourne une copie de l'image
    if ((f = fopen(lefichier, "r")) == NULL){
        fprintf(stderr, "\nErreur dans cacheTexte : %s innaccessible", lefichier);
        return image;
    };

    fseek(f, 0, SEEK_END);
    int fTaille = ftell(f) + 1;

    if(fTaille > tailleDisponible)
    {
        fprintf(stderr, "\nErreur dans cacheTexte : %s est trop volumineux pour cette image", lefichier);
        fclose(f);
        return image;
    }

    fseek(f, 0, SEEK_SET);//=rewind

    str = malloc(fTaille);

    if (!str) 
    { 
        fprintf(stderr, "\nErreur dans cacheTexte : erreur de malloc");
        return image; 
    }

    if(!fread(str, 1, fTaille, f))
    {
        fprintf(stderr, "\nErreur dans cacheTexte : erreur lors de la lecture du fichier");
        fclose(f);
        free(str);
        return image;
    }

    int pos = 0;
    for (int y = 0; y < fTaille / image.largeur && pos <= fTaille; y++)
    {
        for (int x = 0; x < image.largeur; x++)
        {
            if(pos == fTaille)
            { 
                image.img[y][x] = cacheCarac(im.img[y][x], (char)0);
                break; 
            }

            image.img[y][x] = cacheCarac(im.img[y][x], str[pos]);
            pos++;
        }
    }

    fclose(f);
    free(str);
    return image;
}

void reveleTexte(tImage im, char* fichExtrait)
{
    FILE* f;

    if (!(f = fopen(fichExtrait, "w"))){
        fprintf(stderr, "\nErreur dans cacheTexte : %s innaccessible", fichExtrait);
        return;
    };

    int pos = 0;
    for (int y = 0; y < im.hauteur; y++)
    {
        for (int x = 0; x < im.largeur; x++)
        {
            char c = extraitCaract(im.img[y][x]);
            fputc(c, f);

            if(c == 0) 
            {
                fclose(f);
                return;
            }
        }
    }

}