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
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);//récupère la hauteur / capacité de nombre de ligne à l'écran de la console

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
            selection = c - ASCII_0 - 1;
            if (selection > nbChoix)
            {
                selection = selectionPred;
            }
            if (selection == -1)
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
    HANDLE hConsoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);//récupère le handle(un pointeur utilisable dans le winapi) qui fait référence au stdout de la console
    int origineConsole = 0;
    char str[8];

    CONSOLE_SCREEN_BUFFER_INFO csbi;
    BOOL ok = GetConsoleScreenBufferInfo(hConsoleOutput, &csbi);// récupère la structure CONSOLE_SCREEN_BUFFER_INFO qui donne entre autre des infos sur la position du curseur de la console

    origineConsole = csbi.dwCursorPosition.Y;

    printf("%s\n", titre);

    for (int i = 0; i < nbChoix - 1; i++) //print le menu
    {
        printf("%d. %s\n", i + 1, choix[i]);
    }

   //print le dernier élément du menu + le "Choix ==> ...":
    printf("0. ");
    SetConsoleTextAttribute(hConsoleOutput, csbi.wAttributes | COMMON_LVB_REVERSE_VIDEO); //inversion couleurs arière plan / premier plan
    printf("%s", choix[nbChoix - 1]);
    SetConsoleTextAttribute(hConsoleOutput, csbi.wAttributes); //rétablissment des couleur par défault
    printf("\nChoix ==> %s", choix[nbChoix - 1]);
   //

    SetConsoleCursorPosition(hConsoleOutput, (COORD){0, nbChoix + origineConsole});//met le curseur sur le dernier choix

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
            selection = c - ASCII_0 - 1;
            if (selection > nbChoix)
            {
                selection = selectionPred;
            }
            if (selection == -1)
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
            int n = 0;

            SetConsoleCursorPosition(hConsoleOutput, (COORD){3, selection + origineConsole + 1}); // met le curseur avant le texte que l'on veut réécrire avec d'autres couleurs
            SetConsoleTextAttribute(hConsoleOutput, csbi.wAttributes | COMMON_LVB_REVERSE_VIDEO); //inversion couleurs arière plan / premier plan
            printf("%s", choix[selection]);

            SetConsoleCursorPosition(hConsoleOutput, (COORD){3, selectionPred + origineConsole + 1});//met le curseur avant là ou on veut rétablir la couleur par défault
            SetConsoleTextAttribute(hConsoleOutput, csbi.wAttributes);
            printf("%s", choix[selectionPred]);                         //réécrit le texte avec les couleurs par défault pour enlever 'l'aspect séléction'

            SetConsoleCursorPosition(hConsoleOutput, (COORD){0, origineConsole + nbChoix + 1});
            printf("Choix ==> %s", choix[selection]);//réécrit le choix actuelle par dessus le précédant

            int predDiff = strlen(choix[selectionPred]) - strlen(choix[selection]);// différence de nombre de charactère par rapport à la séléction précédante

            if (predDiff)
            { // si le texte de l'ancienne selection est plus long il faut effacer les caractères 'qui dépassent'               
                FillConsoleOutputCharacterA(hConsoleOutput, ' ', predDiff, (COORD) { 10 + strlen(choix[selection]), origineConsole + nbChoix + 1 }, & n); 
            }

            selectionPred = selection;
        }
    }

    printf("\n");

    return selection;
}

#endif

int demandeChemin(char* question, char* chemin)//demande un fichier avec une question
{
    printf("%s", question);

    if(!fgets(chemin, 256, stdin))
    {
        return -1;
    }

    chemin[strcspn(chemin, "\n")] = 0;//enlève la fin de ligne

    return 0;
}

int demandeImportationImage(char* question, tImage* image)//demande et charge un fichier image dans un tImage
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

    printf("Fichier importé.\n");
    return 0;
}

int main()
{
    const char* MENU_YN[2] = {"Oui.", "Non."};

    const char *MENU[10] =
        {
            "Transformer une image en niveau de gris",
            "Flouter une image",
            "Détourer une image",
            "Dissimuler une image dans une autre",
            "Révéler une image cachée dans une autre",
            "Dissimuler du texte dans une image",
            "Révéler un texte caché dans une image",
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
        SetConsoleOutputCP(CP_UTF8);//pour passer la console en utf8 sur Windows pour afficher les accents
    #endif

    while (selection != QUITTER)
    {
        selection = menu(MENU, 10, "Quelle opération voulez-vous effectuer ?");//lance le menu pricipale

        if(selection < IMPORT && source.largeur == 0)//toutes les premières options requièrent un fichier image et 'import' est géré séparement
        { 
            demandeImportationImage("Choisissez un fichier à ouvrir (.pnm | .ppm | .pgm) :\n", &source); 
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
            printf("Détourage de l'image par flou gaussien...\n");
            document = contours(source);
            break;

        case DISSIMULE_IMG:
            demandeImportationImage("Entrez le nom du fichier image à dissimuler à l'interieur:\n", &source1);
            printf("Dissimulation de l'image dans l'image source...\n");
            document = cacheImage(source, source1);
            free(source1.img);
            break;

        case REVELE_IMG:
            printf("Tentative de récupération d'une image caché dans l'image source...\n");
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
            demandeImportationImage("Choisissez un fichier à importerà la place du précédant (.pnm | .ppm | .pgm) :\n", &source); 
            break;

        case SAUV:

            if(document.largeur == 0) 
            { 
                printf("Pas d'image modifiée à sauvegarder.\n");
                break;
            }

            if(demandeChemin("Choissisez un nom de fichier pour sauvegarder votre image:\n", str) == 0 && str[0] != 0 && strlen(str) > 3)
            {
                sauvePnm(str, document);
                printf("Fichier %s enregistré.\n", str);
            }
            else { printf("nom invalide.\n"); }

            break;

        default:
            break;
        }

        if(selection < REVELE_TXT && document.largeur > 0)
        { 
            printf("Opération effectuée.\n"); 
        }

        if(selection != QUITTER)
        {
            printf("Appuyez sur (Entrée) pour faire d'autres opérations\n");

            if(selection < IMPORT){
                printf("ou entrez un nom de fichier pour sauvegarder votre image et quitter:\n");
            }

            demandeChemin("", str);

            if(str[0] == 0 && document.largeur != 0)// a appuyé sur entrée
            {
                free(source.img);
                source = copieImage(document);
            }
            else if(selection < IMPORT && strlen(str) > 3 && !menu(MENU_YN, 2, "Souhaiter-vous vraiment sauvegarder et quitter ?"))// a entré un nom de fichier
            {
                sauvePnm(str, document);
                printf("Fichier %s enregistré.\n", str);
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