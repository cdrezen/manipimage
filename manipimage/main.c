//40007337  Drezen Corentin
//40003483  Pallard Benoît

#pragma
#include <stdio.h>
#include "manipimage.h"
#include "string.h"

int main()
{
    /*
    char reponse[256];

    printf("Choisissez un fichier.ppm � ouvrir :\n");

    if (!scanf("%s", &reponse)// Verifie si scanf n'a pas �chou�.
        || strstr(reponse, ".ppm") - (int)reponse != strlen(reponse) - 4)// Verifie si l'extention est bien .ppm. (strstr retourne un pointeur vers '.ppm' dans la chaine path ou nullptr si non trouv�)
    {
        perror("Vous devez entrez un nom de fichier .ppm");
        return main();
    }
    
    printf("Chargement du fichier...\n");

    tImage im = chargePpm(reponse);

    if(im.largeur == 0 || im.hauteur == 0)// Verifie si chargePpm n'a pas �chou�.
    {
        perror("Erreur lors du chargement du fichier.");
        return -1;
    }

    printf("Fichier import�.");

    char repAttendue = 1;
    //  Attend une r�ponse valide � "Voulez vous cr�er une copie du fichier ?"
    while (repAttendue && reponse[0] != 'y' && reponse[0] != 'n' && reponse[0] != 'o')
    {
        repAttendue = 1;
        printf("\nVoulez vous cr�er une copie du fichier ? (y/n):");
        if (!scanf("%s", &reponse)) { repAttendue = 1; }
        else { repAttendue = -1; }
    }

    if (reponse[0] == 'y' || reponse[0] == 'o')
    {
        printf("Choisissez un nom de fichier pour l'enregister :\n");

        if (!scanf("%s", &reponse))//   Verifie si scanf n'a pas �chou�.// 
        {
            perror("Vous devez entrez un nom de fichier\n"); 
            strcpy(reponse, "sauvegarde.ppm");
        }

        // Corrige le nom si il manque l'extension '.ppm'
        if (strstr(reponse, ".ppm") - (int)reponse != strlen(reponse) - 4)
        {
            strcpy(reponse + strlen(reponse), ".ppm");
        }

        printf("Sauvegarde du fichier...\n");
        sauvePpm(reponse, im);
        printf("Fichier sauvegard� sous le nom %s\n", reponse);
    }

    repAttendue = 1;
    while (repAttendue && reponse[0] != 'y' && reponse[0] != 'n' && reponse[0] != 'o')
    {
        repAttendue = 1;
        printf("\nVoulez vous cr�er une copie en m�moire du fichier puis l'enregistrer ? (y/n):");
        if (!scanf(" %s", &reponse)) { repAttendue = 1; }
        else { repAttendue = -1; }
    }

    if (reponse[0] == 'y' || reponse[0] == 'o')
    {
        tImage im1 = copieImage(im);

        if (im1.largeur == 0 || im1.hauteur == 0)// Verifie si copieImage n'a pas �chou�.
        {
            perror("Erreur lors de la copie de l'image.");
            return -1;
        }

        sauvePpm("im1.ppm", im1);
    }
    */

    tImage im = chargePnm("image1.ppm");
    tImage imPGM = niveauGris(im);
    tImage imFlou = flou(im, 3);
    tImage imGauss = contours(im);

    sauvePnm("image1Gris.pgm", imPGM);
    sauvePnm("image1Flou.ppm", imFlou);
    sauvePnm("image1Contours.ppm", imGauss);

    return 0;
}