//40007337  Drezen Corentin
//40003483  Pallard Benoît

#pragma
#include <stdio.h>
#include "manipimage.h"
#include "string.h"

int main()
{
    char reponse[256];

    printf("Choisissez un fichier.ppm à ouvrir :\n");

    if (!scanf("%s", &reponse)// Verifie si scanf n'a pas échoué.
        || strstr(reponse, ".ppm") - (int)reponse != strlen(reponse) - 4)// Verifie si l'extention est bien .ppm. (strstr retourne un pointeur vers '.ppm' dans la chaine path ou nullptr si non trouvé)
    {
        perror("Vous devez entrez un nom de fichier .ppm");
        return main();
    }
    
    printf("Chargement du fichier...\n");

    tImage im = chargePpm(reponse);

    if(im.largeur == 0 || im.hauteur == 0)// Verifie si chargePpm n'a pas échoué.
    {
        perror("Erreur lors du chargement du fichier.");
        return -1;
    }

    printf("Fichier importé.");

    char repAttendue = 1;
    //  Attend une réponse valide à "Voulez vous créer une copie du fichier ?"
    while (repAttendue && reponse[0] != 'y' && reponse[0] != 'n' && reponse[0] != 'o')
    {
        repAttendue = 1;
        printf("\nVoulez vous créer une copie du fichier ? (y/n):");
        if (!scanf("%s", &reponse)) { repAttendue = 1; }
        else { repAttendue = -1; }
    }

    if (reponse[0] == 'y' || reponse[0] == 'o')
    {
        printf("Choisissez un nom de fichier pour l'enregister :\n");

        if (!scanf("%s", &reponse))//   Verifie si scanf n'a pas échoué.// 
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
        printf("Fichier sauvegardé sous le nom %s\n", reponse);
    }

    repAttendue = 1;
    while (repAttendue && reponse[0] != 'y' && reponse[0] != 'n' && reponse[0] != 'o')
    {
        repAttendue = 1;
        printf("\nVoulez vous créer une copie en mémoire du fichier puis l'enregistrer ? (y/n):");
        if (!scanf(" %s", &reponse)) { repAttendue = 1; }
        else { repAttendue = -1; }
    }

    if (reponse[0] == 'y' || reponse[0] == 'o')
    {
        tImage im1 = copieImage(im);

        if (im1.largeur == 0 || im1.hauteur == 0)// Verifie si copieImage n'a pas échoué.
        {
            perror("Erreur lors de la copie de l'image.");
            return -1;
        }

        sauvePpm("im1.ppm", im1);
    }
    
    return 0;
}