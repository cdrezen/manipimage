//40007337  Drezen Corentin
//40003483  Pallard Benoît

#pragma
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "manipimage.h"

#ifdef _WIN32
#include <conio.h>
#include <Windows.h>
#define FLECHE_HAUT 72
#define FLECHE_GAUCHE 75
#define FLECHE_DROITE 77
#define FLECHE_BAS 80
#define TOUCHE_ENTRER '\r'
#else
#include <termios.h>
#include <unistd.h>
#include <sys/ioctl.h>
#define FLECHE_HAUT 65
#define FLECHE_BAS 66
#define FLECHE_DROITE 67
#define FLECHE_GAUCHE 68
#define TOUCHE_ENTRER '\n'
#endif

#define ASCII_0 48

#ifndef _WIN32 // pour consoles de systemes Unix avec prise en charge des codes VT100 ( '\x1b...' https://www.csie.ntu.edu.tw/~r92094/c++/VT100.html) : xterm, Terminal, etc. \
               // cmd.exe de Windows ne les prend en charges seulement à partir d'une version récente de Windows 10

int menu(const char **choix, int nbChoix, char* titre)
{
    struct termios attr;
    tcgetattr(STDIN_FILENO, &attr); //récupère la structure terminos de stdin, l'entré utlisateur de la console

    attr.c_lflag &= ~(ICANON | ECHO); //enlève le mode canonique (mode entrées ligne par ligne) et l'affichage des entrées
    tcsetattr(STDIN_FILENO, TCSANOW, &attr);

    printf("\x1b[6n"); //requete coordonnées curseur actuelles

    int origineConsole = 0;
    char str[10];

    for (int i = 0; i < 10; i++) //lecture de la réponse de la forme 'x1b[y;xR'
    {
        str[i] = getchar();
        if (str[i] == 'R')
        {
            str[i] = 0;
            break;
        }
    }

    sscanf(str, " \x1b[%d;", &origineConsole); // récupérer la valeur y

    printf("%s\n", titre);
    origineConsole++;

    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

    if(origineConsole + nbChoix > w.ws_row)//corige l'origine si ça dépasse la hauteur de la console
    {
        origineConsole = w.ws_row - nbChoix;
    }

    for (int i = 0; i < nbChoix - 1; i++)
    {
        printf("%d. %s\n", i + 1, choix[i]);
    }

    //Le dernier élément est chosi comme selection d'origine
    printf("0. \x1B[7m%s\n", choix[nbChoix - 1]);       //7m = inversion couleurs arière plan / premier plan
    printf("\x1B[0mChoix ==> %s", choix[nbChoix - 1]);  //0m = rétablissement couleurs par défault
    printf("\x1b[%d;0f", nbChoix + origineConsole - 1); // déplacement curseur au dernier élément du menu

    char selectionPred = nbChoix - 1;
    char selection = nbChoix - 1;
    char c = 0;

    while (c != TOUCHE_ENTRER)
    {
        c = getchar();

        if (c >= ASCII_0 && c <= ASCII_0 + 9) // Le controle du tableau par valeur numérique ne fonctionnera pas pour des valeurs de choix superieur à 9
        {
            selection = c - ASCII_0;
            if (selection > nbChoix)
            {
                selection = selectionPred;
            }
            if (selection == 0)
            {
                selection = nbChoix - 1;
            }
        }

        switch (c)
        {
        case FLECHE_HAUT:
        case FLECHE_GAUCHE:
            if (selection != 0)
            {
                selection--;
            }
            break;
        case FLECHE_BAS:
        case FLECHE_DROITE:
            if (selection < nbChoix - 1)
            {
                selection++;
            }
            break;
        default:
            break;
        }

        if (selection != selectionPred)
        {
            printf("\x1b[%d;4f\x1B[7m%s", selection + origineConsole, choix[selection]);
            printf("\x1b[%d;4f\x1B[0m%s", selectionPred + origineConsole, choix[selectionPred]);
            printf("\x1b[%d;0f\x1b[KChoix ==> %s", origineConsole + nbChoix, choix[selection]); //K = effacer la ligne
            selectionPred = selection;
        }
    }

    attr.c_lflag |= ICANON | ECHO; //rétabli le mode canonique et l'affichage des entrées
    tcsetattr(STDIN_FILENO, TCSANOW, &attr);

    printf("\n");

    return selection;
}

#else //Windows

int menu(const char **choix, int nbChoix, char* titre)
{
    HANDLE hConsoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);
    int origineConsole = 0;
    char str[8];

    CONSOLE_SCREEN_BUFFER_INFO csbi;
    BOOL ok = GetConsoleScreenBufferInfo(hConsoleOutput, &csbi);

    origineConsole = csbi.dwCursorPosition.Y;

    if (origineConsole > 16)
    {

    }

    printf("%s\n", titre);

    for (int i = 0; i < nbChoix - 1; i++)
    {
        printf("%d. %s\n", i + 1, choix[i]);
    }

    printf("0. ");
    SetConsoleTextAttribute(hConsoleOutput, csbi.wAttributes | COMMON_LVB_REVERSE_VIDEO); //inversion couleurs arière plan / premier plan
    printf("%s", choix[nbChoix - 1]);
    SetConsoleTextAttribute(hConsoleOutput, csbi.wAttributes);
    printf("\nChoix ==> %s", choix[nbChoix - 1]);

    SetConsoleCursorPosition(hConsoleOutput, (COORD){0, nbChoix + origineConsole});

    char selectionPred = nbChoix - 1;
    char selection = nbChoix - 1;
    char c = 0;

    while (c != TOUCHE_ENTRER)
    {
        while (!_kbhit())
            ; //attend une touche

        c = _getch(); // équivalent à passer la console en non canonique et lire un char sans l'afficher sous linux

        if (c >= ASCII_0 && c <= ASCII_0 + 9) // Le controle du tableau par valeur numérique ne fonctionnera pas pour des valeurs de choix superieur à 9
        {
            selection = c - ASCII_0;
            if (selection > nbChoix)
            {
                selection = selectionPred;
            }
            if (selection == 0)
            {
                selection = nbChoix;
            }
        }

        switch (c)
        {
        case FLECHE_HAUT:
        case FLECHE_GAUCHE:
            if (selection != 0)
            {
                selection--;
            }
            break;
        case FLECHE_BAS:
        case FLECHE_DROITE:
            if (selection < nbChoix - 1)
            {
                selection++;
            }
            break;
        default:
            break;
        }

        if (selection != selectionPred)
        {
            SetConsoleCursorPosition(hConsoleOutput, (COORD){3, selection + origineConsole + 1});
            SetConsoleTextAttribute(hConsoleOutput, csbi.wAttributes | COMMON_LVB_REVERSE_VIDEO); //inversion couleurs arière plan / premier plan
            printf("%s", choix[selection]);

            SetConsoleCursorPosition(hConsoleOutput, (COORD){3, selectionPred + origineConsole + 1});
            SetConsoleTextAttribute(hConsoleOutput, csbi.wAttributes);
            printf("%s", choix[selectionPred]);

            SetConsoleCursorPosition(hConsoleOutput, (COORD){0, origineConsole + nbChoix + 1});
            printf("Choix ==> %s", choix[selection]);

            int predDiff = strlen(choix[selectionPred]) - strlen(choix[selection]);

            if (predDiff)
            { // si le texte de l'ancienne selection est plus grande il faut effacer les caractères 'qui dépassent'
                int n = 0;
                FillConsoleOutputCharacterA(hConsoleOutput, ' ', predDiff, (COORD){10 + strlen(choix[selection]), origineConsole + nbChoix + 1}, &n);
                FillConsoleOutputAttribute(
                    hConsoleOutput, csbi.wAttributes,
                    predDiff, (COORD) { 10 + strlen(choix[selection]), origineConsole + nbChoix + 1 }, &n
                );
            }

            selectionPred = selection;
        }
    }

    printf("\n");

    return selection;
}

#endif

int demandeChemin(char *question, char *chemin)
{
    printf("%s", question);

    if(!fgets(chemin, 256, stdin))
    {
        return -1;
    }

    chemin[strcspn(chemin, "\n")] = 0;

    return 0;
}

int demandeImportationImage(char *question, tImage *image)
{
    char reponse[256];

    demandeChemin(question, reponse);

    printf("Chargement du fichier...\n");

    *image = chargePnm(reponse);

    if (image->largeur == 0 || image->hauteur == 0) // Verifie si chargePpm n'a pas �chou�.
    {
        perror("\nVous devez choisir un fichier pnm de type P2 ou P3.");
        return demandeImportationImage(question, image);
    }

    printf(u8"Fichier importé.\n");
    return 0;
}

int main()
{
    const char* MENU_YN[2] = {"Oui.", "Non."};

    const char *MENU[10] =
        {
            "Transformer une image en niveau de gris",
            "Flouter une image",
            u8"Détourer une image",
            "Dissimuler une image dans une autre",
            u8"Révéler une image cachée dans une autre",
            "Dissimuler du texte dans une image",
            u8"Révéler un texte caché dans une image",
            "Changer d'image source",
            "Sauvegarder l'image",
            "Quitter"
        };

    enum
    {
        GRIS,
        FLOUTE,
        DETOUR,
        DISSIMULE_IMG,
        REVELE_IMG,
        DISSIMULE_TXT,
        REVELE_TXT,
        IMPORT,
        SAUV,
        QUITTER
    };

    int selection = -1;

    
    tImage source; source.largeur = 0;
    tImage source1; source1.largeur = 0;
    tImage document; document.largeur = 0;
    char str[256];
    int rFlou = 0;

    #ifdef _WIN32
        
    #endif

    while (selection != QUITTER)
    {
        selection = menu(MENU, 10, u8"Quelle opération voulez-vous effectuer ?");

        if(selection < IMPORT && source.largeur == 0)
        { 
            demandeImportationImage(u8"Choisissez un fichier à ouvrir (.pnm | .ppm | .pgm) :\n", &source); 
        }

        switch (selection)
        {

        case GRIS:
            printf("Convertion en niveau de gris...\n");
            document = niveauGris(source);
            break;

        case FLOUTE:
            rFlou = 0;
            while (rFlou < 1 || rFlou > 20)
            {
                printf("Choisissez un rayon (entre 1 et 20 px):\n");                
                if (!fgets(str, 256, stdin) || !sscanf(str, "%d", &rFlou)) 
                { rFlou = 0; }
            }
            printf("Floutage de l'image (r=%dpx)...\n", rFlou);
            document = flou(source, rFlou);
            break;

        case DETOUR:
            printf(u8"Détourage de l'image par flou gaussien...\n");
            document = contours(source);
            break;

        case DISSIMULE_IMG:
            demandeImportationImage(u8"Entrez le nom du fichier image à dissimuler à l'interieur:\n", &source1);
            printf("Dissimulation de l'image dans l'image source...\n");
            document = cacheImage(source, source1);
            free(source1.img);
            break;

        case REVELE_IMG:
            printf(u8"Tentative de récupération d'une image caché dans l'image source...\n");
            document = reveleImage(source);
            break;

        case DISSIMULE_TXT:
            demandeChemin("Entrez un nom de fichier texte:\n", str);
            document = cacheTexte(source, str);
            break;

        case REVELE_TXT:
            demandeChemin("Entrez un nom de fichier texte de sortie:\n", str);
            reveleTexte(source, str);
            printf("Opération effectuée.\n");
            break;
        
        case IMPORT:
            if(source.largeur != 0){
                free(source.img);
             }
            demandeImportationImage(u8"Choisissez un fichier à ouvrir (.pnm | .ppm | .pgm) :\n", &source); 
            break;

        case SAUV:

            if(document.largeur == 0) 
            { 
                printf(u8"Pas d'image modifiée à sauvegarder.\n");
                break;
            }

            if(demandeChemin("Choissisez un nom de fichier pour sauvegarder votre image:\n", str) == 0 && str[0] != 0 && strlen(str) > 3)
            {
                sauvePnm(str, document);
                printf(u8"Fichier %s enregistré.\n", str);
            }
            else { printf("nom invalide.\n"); }

            break;

        default:
            break;
        }

        if(selection < REVELE_TXT && document.largeur > 0)
        { 
            printf(u8"Opération effectuée.\n"); 
        }

        if(selection != QUITTER)
        {
            printf(u8"Appuyez sur (Entrée) pour faire d'autres opérations\n");

            if(selection < IMPORT){
                printf("ou entrez un nom de fichier pour sauvegarder votre image et quitter:\n");
            }

            demandeChemin("", str);

            if(str[0] == 0 && document.largeur != 0)
            {
                free(source.img);
                source = copieImage(document);
            }
            else if(selection < IMPORT && strlen(str) > 3 && !menu(MENU_YN, 2, "Souhaiter-vous vraiment sauvegarder et quitter ?"))
            {
                sauvePnm(str, document);
                printf(u8"Fichier %s enregistré.\n", str);
                selection = QUITTER;
            }
        }
    }

    if(source.largeur != 0){
        free(source.img);
    }

    if(document.largeur != 0){
        free(document.img);
    }

    return 0;
}