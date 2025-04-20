#include <SDL2/SDL_events.h>
#include <SDL2/SDL_render.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "../inc/constantes.h"
#include "../inc/variables.h"
#include "../inc/evenement.h"
#include "../inc/BTN_.h"
#include "../inc/klcul_moy.h"
#include "../inc/sauvegarde.h"
#include "../inc/ajouter.h"
#include "../inc/fenetres.h"
#include "../inc/lireNote.h"
#include "../inc/charge.h"
#include "../inc/renderer.h"

void main_looping(Liste_f *listFiche, BoiteOutils *outils, SDL_Window *window, SDL_Renderer *render, char **argv)
{
    SDL_Event winEvent;
    SDL_Event ficheEvent;
    int continuer = 1;
    int m = 0, f = 0;
    Fiche* ficheActuelle = NULL;
    FILE *lastRoot = fopen("appData/lastR.jpfdat", "rb");
    TTF_Font *font = TTF_OpenFont("appData/fonts/taile.ttf", 14);

    if(lastRoot == NULL)
    {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_WARNING, NULL, "Repertoire des donnees introuvable.", NULL);
    }
    else
    {
        char *lastFiche = malloc((MAX_NAME_CHAR+1)*sizeof(char));
        int nf = 0;
        fscanf(lastRoot, "%d", &nf);
        for(int i = 0; i < nf; i++)
        {
            if(ficheActuelle != NULL)
                ficheActuelle->statut = arriere_plan;
            fscanf(lastRoot, "%s", lastFiche);
            if(lastFiche != NULL)
            {
                ficheActuelle = charger_fiche(listFiche, render, "test");
                f++;
            }
        }
        free(lastFiche);
        fclose(lastRoot);
    }

    while(continuer) //Looping
    {
        SDL_WaitEvent(&winEvent);
        switch(winEvent.type)
        {
        case SDL_QUIT:
            lastRoot = fopen("appData/lastR.jpfdat", "w+");
            if(lastRoot != NULL)
            {
                char nom[MAX_NAME_CHAR+1];
                fprintf(lastRoot, "%d", f);
                for(int i = 0; i < f; i++)
                {
                    for(int j = 0; j < strlen(listFiche->addrFiche[i]->nom)-4; j++)
                    {
                        nom[j] = listFiche->addrFiche[i]->nom[j];
                        nom[j+1] = '\0';
                    }
                    fprintf(lastRoot, "\n%s", nom);
                }
            }
            continuer = 0;
            break;
        case SDL_MOUSEBUTTONDOWN:
            if(winEvent.button.x > 0 && winEvent.button.y > 0 && winEvent.button.y < FICHE_Y) //D'abord les évènement de la fenetre principale
            {
                if(BTN_RenderMouseClicDown(outils->addFiche, winEvent, render))
                {
                    if(ficheActuelle != NULL)
                        ficheActuelle->statut = arriere_plan;
                    ficheActuelle = AjouterFiche(listFiche, render);
                    f++;
                }
                if(BTN_RenderMouseClicDown(outils->addMatiere, winEvent, render))
                {
                    if(AjouterMatiere(ficheActuelle) == ok)
                        m++;
                }
                if(BTN_RenderMouseClicDown(outils->suppFiche, winEvent, render))
                {
                    DetruireFiche(listFiche, ficheActuelle);
                    f--;
                }
                if(BTN_RenderMouseClicDown(outils->suppMatiere, winEvent, render))
                {
                    fenetre_supprimerMatiere(ficheActuelle);
                    m--;
                }
                if(BTN_RenderMouseClicDown(outils->chargeFiche, winEvent, render))
                {
                    if(ficheActuelle != NULL)
                        ficheActuelle->statut = arriere_plan;
                    ficheActuelle = fenetre_chargerFiche(listFiche, render);
                    f++;
                }
                if(BTN_RenderMouseClicDown(outils->saveFiche, winEvent, render))
                {
                    enregistrer_fiche(ficheActuelle, argv[0]);
                }
                if(BTN_RenderMouseClicDown(outils->klculBilan, winEvent, render))
                {
                    klcul_moyennes(ficheActuelle);
                }
                if(listFiche->addrFiche[0] != NULL)
                {
                    if(BTN_MOUSE_CLIC_ON_RECT(listFiche->addrFiche[0]->volet_titre->rectangle, winEvent))
                    {
                        ficheActuelle->statut = arriere_plan;
                        ficheActuelle = listFiche->addrFiche[0];
                        ficheActuelle->statut = actuelle;
                        SDL_RenderCopy(ficheActuelle->render, ficheActuelle->texture, NULL, &ficheActuelle->surface_rect);
                    }
                    if(BTN_RenderMouseClicDown(listFiche->addrFiche[0]->volet_titre->supp, winEvent, render))
                    {
                        ficheActuelle = DetruireFiche(listFiche, listFiche->addrFiche[0]);
                        f--;
                    }
                }
                if(listFiche->addrFiche[1] != NULL)
                {
                    if(BTN_MOUSE_CLIC_ON_RECT(listFiche->addrFiche[1]->volet_titre->rectangle, winEvent))
                    {
                        ficheActuelle->statut = arriere_plan;
                        ficheActuelle = listFiche->addrFiche[1];
                        ficheActuelle->statut = actuelle;
                        SDL_RenderCopy(ficheActuelle->render, ficheActuelle->texture, NULL, &ficheActuelle->surface_rect);
                    }
                    if(BTN_RenderMouseClicDown(listFiche->addrFiche[1]->volet_titre->supp, winEvent, render))
                    {
                        ficheActuelle = DetruireFiche(listFiche, listFiche->addrFiche[1]);
                        f--;
                    }
                }
                if(listFiche->addrFiche[2] != NULL)
                {
                    if(BTN_MOUSE_CLIC_ON_RECT(listFiche->addrFiche[2]->volet_titre->rectangle, winEvent))
                    {
                        ficheActuelle->statut = arriere_plan;
                        ficheActuelle = listFiche->addrFiche[2];
                        ficheActuelle->statut = actuelle;
                        SDL_RenderCopy(ficheActuelle->render, ficheActuelle->texture, NULL, &ficheActuelle->surface_rect);
                    }
                    if(BTN_RenderMouseClicDown(listFiche->addrFiche[2]->volet_titre->supp, winEvent, render))
                    {
                        ficheActuelle = DetruireFiche(listFiche, listFiche->addrFiche[2]);
                        f--;
                    }
                }
                if(listFiche->addrFiche[3] != NULL)
                {
                    if(BTN_MOUSE_CLIC_ON_RECT(listFiche->addrFiche[3]->volet_titre->rectangle, winEvent))
                    {
                        ficheActuelle->statut = arriere_plan;
                        ficheActuelle = listFiche->addrFiche[3];
                        ficheActuelle->statut = actuelle;
                        SDL_RenderCopy(ficheActuelle->render, ficheActuelle->texture, NULL, &ficheActuelle->surface_rect);
                    }
                    if(BTN_RenderMouseClicDown(ficheActuelle->volet_titre->supp, winEvent, render))
                    {
                        ficheActuelle = DetruireFiche(listFiche, listFiche->addrFiche[3]);
                        f--;
                    }
                }
                if(listFiche->addrFiche[4] != NULL)
                {
                    if(BTN_MOUSE_CLIC_ON_RECT(listFiche->addrFiche[4]->volet_titre->rectangle, winEvent))
                    {
                        ficheActuelle->statut = arriere_plan;
                        ficheActuelle = listFiche->addrFiche[4];
                        ficheActuelle->statut = actuelle;
                        SDL_RenderCopy(ficheActuelle->render, ficheActuelle->texture, NULL, &ficheActuelle->surface_rect);
                    }
                    if(BTN_RenderMouseClicDown(listFiche->addrFiche[4]->volet_titre->supp, winEvent, render))
                    {
                        ficheActuelle = DetruireFiche(listFiche, listFiche->addrFiche[0]);
                        f--;
                    }
                }
            }
            else  //Maintenant les éléments  de la fiche
            {
                if(ficheActuelle != NULL)
                {
                    SDL_SetRenderTarget(render, ficheActuelle->texture);
                    GetFicheEvent(winEvent, &ficheEvent);
                    if(BTN_RenderMouseClicDown(ficheActuelle->bouton_plus, ficheEvent, render))
                    {
                        if(AjouterMatiere(ficheActuelle) == ok)
                            m++;
                    }
                    else
                    {
                        switch(SaisirNote(ficheActuelle, &ficheEvent))
                        {
                        case ok:
                            break;
                        case error:
                            break;
                        case quit_window:
                            continuer = 0;
                            break;
                        }
                    }
                    SDL_SetRenderTarget(render, NULL);
                }
            }
            break;
        }
        SET_GREY(render);
        SDL_RenderClear(render);
        renderCredits(render, font);
        renderOutils(outils, render, &winEvent);
        renderFicheVolet(listFiche, &winEvent, render);
        renderFiche(ficheActuelle, &winEvent, render);
        SDL_RenderPresent(render);
    }
    TTF_CloseFont(font);
}

void GetFicheEvent(SDL_Event winEvent, SDL_Event *ficheEvent)
{
    *ficheEvent = winEvent;
    ficheEvent->button.x = winEvent.button.x - FICHE_X;
    ficheEvent->button.y = winEvent.button.y - FICHE_Y;
    ficheEvent->motion.x = winEvent.motion.x - FICHE_X;
    ficheEvent->motion.y = winEvent.motion.y - FICHE_Y;
}

