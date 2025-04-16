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

void main_looping(Liste_f *listFiche, BoiteOutils *outils, SDL_Window *window, SDL_Renderer *render, char **argv)
{
    SDL_Event winEvent;
    SDL_Event ficheEvent;
    int continuer = 1;
    int m = 0, f = 0;
    Fiche* ficheActel = NULL;
    FILE *lastRoot = fopen("appData/lastR.jpfdat", "rb");
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
            if(ficheActel != NULL)
                ficheActel->statut = arriere_plan;
            fscanf(lastRoot, "%s", lastFiche);
            if(lastFiche != NULL)
            {
                ficheActel = charger_fiche(listFiche, render, "test");
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
        case SDL_MOUSEMOTION:
            //D'abord les évènement de la fenetre principale
            if(BTN_RenderMouseMotion(outils->addMatiere, winEvent, render)) {}
            if(BTN_RenderMouseMotion(outils->addFiche, winEvent, render)) {}
            if(BTN_RenderMouseMotion(outils->suppFiche, winEvent, render)) {}
            if(BTN_RenderMouseMotion(outils->suppMatiere, winEvent, render)) {}
            if(BTN_RenderMouseMotion(outils->chargeFiche, winEvent, render)) {}
            if(BTN_RenderMouseMotion(outils->saveFiche, winEvent, render)) {}
            if(BTN_RenderMouseMotion(outils->klculBilan, winEvent, render)) {}
            if(listFiche->addrFiche[0] != NULL)
            {
                if(mouseMoveInVolet(listFiche->addrFiche[0], winEvent)) {}
                if(BTN_RenderMouseMotion(listFiche->addrFiche[0]->volet_titre->supp, winEvent, render)) {}
            }
            if(listFiche->addrFiche[1] != NULL)
            {
                if(mouseMoveInVolet(listFiche->addrFiche[1], winEvent)) {}
                if(BTN_RenderMouseMotion(listFiche->addrFiche[1]->volet_titre->supp, winEvent, render)) {}
            }
            if(listFiche->addrFiche[2] != NULL)
            {
                if(mouseMoveInVolet(listFiche->addrFiche[2], winEvent)) {}
                if(BTN_RenderMouseMotion(listFiche->addrFiche[2]->volet_titre->supp, winEvent, render)) {}
            }
            if(listFiche->addrFiche[3] != NULL)
            {
                if(mouseMoveInVolet(listFiche->addrFiche[3], winEvent)) {}
                if(BTN_RenderMouseMotion(listFiche->addrFiche[3]->volet_titre->supp, winEvent, render)) {}
            }
            if(listFiche->addrFiche[4] != NULL)
            {
                if(mouseMoveInVolet(listFiche->addrFiche[4], winEvent)) {}
                if(BTN_RenderMouseMotion(listFiche->addrFiche[4]->volet_titre->supp, winEvent, render)) {}
            }
            if(ficheActel != NULL)
            {
                if(winEvent.motion.x > FICHE_X && winEvent.motion.y > FICHE_Y) //Maintenant les éléments  de la fiche
                {
                    SDL_SetRenderTarget(render, ficheActel->texture);
                    GetFicheEvent(winEvent, &ficheEvent);
                    if(BTN_RenderMouseMotion(ficheActel->bouton_plus, ficheEvent, render)) {}
                    SDL_SetRenderTarget(render, NULL);
                    SDL_RenderCopy(ficheActel->render, ficheActel->texture, NULL, &ficheActel->surface_rect);
                }
            }
            break;
        case SDL_MOUSEBUTTONDOWN:
            if(winEvent.button.x > 0 && winEvent.button.y > 0 && winEvent.button.y < FICHE_Y) //D'abord les évènement de la fenetre principale
            {
                if(BTN_RenderMouseClicDown(outils->addFiche, winEvent, render))
                {
                    if(ficheActel != NULL)
                        ficheActel->statut = arriere_plan;
                    ficheActel = AjouterFiche(listFiche, render);
                    f++;
                }
                if(BTN_RenderMouseClicDown(outils->addMatiere, winEvent, render))
                {
                    if(AjouterMatiere(ficheActel) == ok)
                        m++;
                }
                if(BTN_RenderMouseClicDown(outils->suppFiche, winEvent, render))
                {
                    DetruireFiche(listFiche, ficheActel);
                    f--;
                }
                if(BTN_RenderMouseClicDown(outils->suppMatiere, winEvent, render))
                {
                    fenetre_supprimerMatiere(ficheActel);
                    m--;
                }
                if(BTN_RenderMouseClicDown(outils->chargeFiche, winEvent, render))
                {
                    if(ficheActel != NULL)
                        ficheActel->statut = arriere_plan;
                    ficheActel = fenetre_chargerFiche(listFiche, render);
                    f++;
                }
                if(BTN_RenderMouseClicDown(outils->saveFiche, winEvent, render))
                {
                    enregistrer_fiche(ficheActel, argv[0]);
                }
                if(BTN_RenderMouseClicDown(outils->klculBilan, winEvent, render))
                {
                    klcul_moyennes(ficheActel);
                }
                if(listFiche->addrFiche[0] != NULL)
                {
                    if(BTN_MOUSE_CLIC_ON_RECT(listFiche->addrFiche[0]->volet_titre->rectangle, winEvent))
                    {
                        ficheActel->statut = arriere_plan;
                        ficheActel = listFiche->addrFiche[0];
                        ficheActel->statut = actuelle;
                        SDL_RenderCopy(ficheActel->render, ficheActel->texture, NULL, &ficheActel->surface_rect);
                    }
                    if(BTN_RenderMouseClicDown(listFiche->addrFiche[0]->volet_titre->supp, winEvent, render))
                    {
                        ficheActel = DetruireFiche(listFiche, listFiche->addrFiche[0]);
                        f--;
                    }
                }
                if(listFiche->addrFiche[1] != NULL)
                {
                    if(BTN_MOUSE_CLIC_ON_RECT(listFiche->addrFiche[1]->volet_titre->rectangle, winEvent))
                    {
                        ficheActel->statut = arriere_plan;
                        ficheActel = listFiche->addrFiche[1];
                        ficheActel->statut = actuelle;
                        SDL_RenderCopy(ficheActel->render, ficheActel->texture, NULL, &ficheActel->surface_rect);
                    }
                    if(BTN_RenderMouseClicDown(listFiche->addrFiche[1]->volet_titre->supp, winEvent, render))
                    {
                        ficheActel = DetruireFiche(listFiche, listFiche->addrFiche[1]);
                        f--;
                    }
                }
                if(listFiche->addrFiche[2] != NULL)
                {
                    if(BTN_MOUSE_CLIC_ON_RECT(listFiche->addrFiche[2]->volet_titre->rectangle, winEvent))
                    {
                        ficheActel->statut = arriere_plan;
                        ficheActel = listFiche->addrFiche[2];
                        ficheActel->statut = actuelle;
                        SDL_RenderCopy(ficheActel->render, ficheActel->texture, NULL, &ficheActel->surface_rect);
                    }
                    if(BTN_RenderMouseClicDown(listFiche->addrFiche[2]->volet_titre->supp, winEvent, render))
                    {
                        ficheActel = DetruireFiche(listFiche, listFiche->addrFiche[2]);
                        f--;
                    }
                }
                if(listFiche->addrFiche[3] != NULL)
                {
                    if(BTN_MOUSE_CLIC_ON_RECT(listFiche->addrFiche[3]->volet_titre->rectangle, winEvent))
                    {
                        ficheActel->statut = arriere_plan;
                        ficheActel = listFiche->addrFiche[3];
                        ficheActel->statut = actuelle;
                        SDL_RenderCopy(ficheActel->render, ficheActel->texture, NULL, &ficheActel->surface_rect);
                    }
                    if(BTN_RenderMouseClicDown(ficheActel->volet_titre->supp, winEvent, render))
                    {
                        ficheActel = DetruireFiche(listFiche, listFiche->addrFiche[3]);
                        f--;
                    }
                }
                if(listFiche->addrFiche[4] != NULL)
                {
                    if(BTN_MOUSE_CLIC_ON_RECT(listFiche->addrFiche[4]->volet_titre->rectangle, winEvent))
                    {
                        ficheActel->statut = arriere_plan;
                        ficheActel = listFiche->addrFiche[4];
                        ficheActel->statut = actuelle;
                        SDL_RenderCopy(ficheActel->render, ficheActel->texture, NULL, &ficheActel->surface_rect);
                    }
                    if(BTN_RenderMouseClicDown(listFiche->addrFiche[4]->volet_titre->supp, winEvent, render))
                    {
                        ficheActel = DetruireFiche(listFiche, listFiche->addrFiche[0]);
                        f--;
                    }
                }
            }
            else  //Maintenant les éléments  de la fiche
            {
                if(ficheActel != NULL)
                {
                    SDL_SetRenderTarget(render, ficheActel->texture);
                    GetFicheEvent(winEvent, &ficheEvent);
                    if(BTN_RenderMouseClicDown(ficheActel->bouton_plus, ficheEvent, render))
                    {
                        if(AjouterMatiere(ficheActel) == ok)
                            m++;
                    }
                    else
                    {
                        switch(SaisirNote(ficheActel, &ficheEvent))
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
        SDL_RenderPresent(render);
    }
}

void GetFicheEvent(SDL_Event winEvent, SDL_Event *ficheEvent)
{
    *ficheEvent = winEvent;
    ficheEvent->button.x = winEvent.button.x - FICHE_X;
    ficheEvent->button.y = winEvent.button.y - FICHE_Y;
    ficheEvent->motion.x = winEvent.motion.x - FICHE_X;
    ficheEvent->motion.y = winEvent.motion.y - FICHE_Y;
}
