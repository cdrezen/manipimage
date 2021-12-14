//40007337  Drezen Corentin
//40003483  Pallard Benoît

#pragma
#include <stdio.h>
#include <string.h>
#include "manipimage.h"

#ifdef _WIN32
    #include <conio.h>
    #define FLECHE_HAUT 72
    #define FLECHE_GAUCHE 75
    #define FLECHE_DROITE 77
    #define FLECHE_BAS 80
#else
    #include "consoleunix.h"
    #define FLECHE_HAUT 65
    #define FLECHE_BAS 66
    #define FLECHE_DROITE 67
    #define FLECHE_GAUCHE 68
#endif

#define TOUCHE_ENTRER '\n'  
#define ASCII_0 48

int menu(char** choix, int nbChoix) 
{
    printf("\x1b[6n"); //requete coordonnées

    int origineConsole = 0;
    char str[8];

    for(int i = 0; i < 8; i++)
    {
        str[i] = _getch();
        if(str[i] == 'R') 
        { 
            str[i] = 0;
            break;
        }
    }

    sscanf(str, "\x1b[%d;*", &origineConsole);
    origineConsole--;


    for (int i = 0; i < nbChoix - 1; i++)
    {
        printf("%d. %s\n", i + 1, choix[i]);
    }

    //////////////////////https://www.csie.ntu.edu.tw/~r92094/c++/VT100.html

    printf("0. \x1B[7m%s\n", choix[nbChoix - 1]);

    printf("\x1B[0mChoix ==> %s", choix[nbChoix - 1]);


    printf("\x1b[%d;0f", nbChoix + origineConsole);

    char selectionPred = 8;
    char selection = 8;
    char c = 0;

    while (c != TOUCHE_ENTRER)
    {

        /* if (c != 0)
        {
            printf("%d\n", c);
            continue;
        } */

        if (c >= ASCII_0 && c <= ASCII_0 + 9) // Le controle du tableau par valeur numérique ne fonctionnera pas pour des valeurs de choix superieur à 9
        {
            selection = c - ASCII_0;
            if (selection > nbChoix) { selection = selectionPred; }
            if (selection == 0) { selection = nbChoix; }
        }

        switch (c)
        {
        case FLECHE_HAUT:
        case FLECHE_GAUCHE:
            if (selection != 1)
            {
                selection--;
            }
            break;
        case FLECHE_BAS:
        case FLECHE_DROITE:
            if (selection < nbChoix)
            {
                selection++;
            }
            break;
        default:
            break;
        }

        if (selection != selectionPred)
        {
            printf("\x1b[%d;4f\x1B[0m%s", selectionPred + origineConsole, choix[selectionPred - 1]);
            printf("\x1b[%d;4f\x1B[7m%s", selection + origineConsole, choix[selection - 1]);
            printf("\x1b[%d;0f\x1B[0m\x1b[KChoix ==> %s", origineConsole + nbChoix + 1, choix[selection - 1]);
            selectionPred = selection;
        }

        c = _getch();
    }

    return selection;
}

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

    /*
    tImage im = chargePnm("image1.ppm");

    tImage imPGM = niveauGris(im);
    tImage imFlou = flou(im, 3);
    tImage imGauss = contours(im);
    free(im.img);

    sauvePnm("image1Gris.pgm", imPGM);
    free(imPGM.img);

    sauvePnm("image1Flou.ppm", imFlou);
    free(imFlou.img);

    sauvePnm("image1Contours.ppm", imGauss);
    free(imGauss.img);
    */

    printf("Quelle opération voulez-vous effectuer ?\n");

    const char * MENU[8] =
    {
        "Transformer une image en niveau de gris",
        "Flouter une image",
        "Détourer une image",
        "Dissimuler une image dans une autre",
        "Révéler une image cachée dans une autre",
        "Dissimuler du texte dans une image",
        "Révéler un texte caché dans une image",
        "Quitter"
    };

    int selection = menu(MENU, 8);

    printf("%d", selection);
    
    return 0;
}