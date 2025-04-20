#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "../inc/constantes.h"
#include "../inc/variables.h"
#include "../inc/evenement.h"

int main(int argc, char **argv)
{
    /* Initialisation simple */
    if(SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO|SDL_INIT_EVENTS) != 0)
    {
        printf("Echec de l'initialisation de la SDL (%s)\n",SDL_GetError());
        return EXIT_FAILURE;
    }
    TTF_Init();

    SDL_GetDesktopDisplayMode(0, &resolution);
    if(resolution.w != 1366 || resolution.h != 768)
    {
        int choix = 0;
        SDL_MessageBoxColorScheme *color = malloc(sizeof(*color));
        color->colors->r = 100;
        color->colors->g = 0;
        color->colors->b = 125;
        SDL_MessageBoxData messageData;
        SDL_MessageBoxButtonData buttons[2];
        buttons[0].text = "non";
        buttons[0].buttonid = 0;
        buttons[0].flags = SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT;
        buttons[1].text = "oui";
        buttons[1].buttonid = 1;
        buttons[1].flags = SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT;
        messageData.window = NULL;
        messageData.title = "Display Error";
        messageData.numbuttons = 2;
        messageData.message = "La resolution actuel du bureau peut entrainer des bugs graphiques\n\
ou des erreurs lors du traitement des donnees.\n\
Voulez vous continuez quand meme ?\n\nNB: La resolution requise est 1366*768.\n\
Si vous disposez de cette resolution veillez la selectionner\ndans les parametres d'affichage avances.";
        messageData.flags = SDL_MESSAGEBOX_INFORMATION;
        messageData.buttons = buttons;
        messageData.colorScheme = color;
        SDL_ShowMessageBox(&messageData, &choix);
        if(choix == 0)
        {
            free(color);
            return EXIT_SUCCESS;
        }
        else
        {
            free(color);
        }
    }

    DECLARE_FONT();

    // Création de(s) fenêtre(s)
    SDL_Window *pWindow = NULL;
    SDL_Renderer *pRender;
    SDL_Surface *pWinSur = NULL;
    char *pWTitle = (argv[1] == NULL)? "Fiche De Note.exe" : "Fiche De Note";
    //d'autres variables
    Liste_f *plisteFiche = CreerListeFiche();
    BoiteOutils *pOutils = CreerOutils();

    // initialisation des fenêtres et surfaces
    // La fenêtre principale
    int windowPos_y = 0;
    SDL_CreateWindowAndRenderer(LARGEUR_UTILISATEUR, HAUTEUR_UTILISATEUR, SDL_WINDOW_SHOWN, &pWindow, &pRender);
    SDL_SetRenderDrawBlendMode(pRender, SDL_BLENDMODE_BLEND);
    SDL_SetWindowTitle(pWindow, pWTitle);
    SDL_GetWindowBordersSize(pWindow, &windowPos_y, NULL, NULL, NULL);
    SDL_SetWindowPosition(pWindow, SDL_WINDOWPOS_CENTERED, windowPos_y);
    //La(es) surface(s)
    pWinSur = SDL_GetWindowSurface(pWindow);
    if(pWindow != NULL && pRender != NULL) //Déroulement du programme
    {
        afficherOutils(pRender, pOutils);
        main_looping(plisteFiche, pOutils, pWindow, pRender, argv);

        SDL_FreeSurface(pWinSur);
        SDL_DestroyRenderer(pRender);
        SDL_DestroyWindow(pWindow);
    }
    else
    {
        fprintf(stderr, "Erreur de creation de la fenêtre ou du rendu ou encore de la texture: %s\n", SDL_GetError());
        return EXIT_FAILURE;
    }
    SupprimerListeFiche(plisteFiche);
    DetruireOutils(pOutils);

    SDL_Quit();
    return EXIT_SUCCESS;
}
