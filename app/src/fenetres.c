#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include "../inc/constantes.h"
#include "../inc/variables.h"
#include "../inc/fenetres.h"
#include "../inc/KeyDown.h"
#include "../inc/charge.h"
#include "../inc/BTN_.h"
#include "../inc/ajouter.h"

Matiere* fenetre_nouvelleMatiere(Fiche *fiche) //Fini
{
    if(fiche == NULL)
    {
        fprintf(stdout, "le message qui sera renvoye signifie que la fiche sur laquelle on ajoute une nouvelle matiere n'existe pas :: fenetre_nouvelleMatiere().\n");
        return NULL;
    }
    Matiere *matiere = NULL;
    int tn = 0, tb = 0;
    int n = 0, c = 0;
    int quitter = 0;
    char *nom = malloc((MAX_NAME_CHAR+1) * sizeof(char));
    char coeff[5];
    SDL_Texture *texCaractere = NULL;
    BTN_Button *button_ok = BTN_CreateButtonWithTexture(BTN_STATIC_BUTTON, OK_BUTTON_W, OK_BUTTON_H, OK_BUTTON_X, OK_BUTTON_Y, "OK", "appData/fonts/taile.ttf");
    SDL_Event event;
    SDL_Surface *caractere = NULL;
    SDL_Window *fenetre = NULL;
    SDL_Renderer *render = NULL;
    SDL_Rect editSpace[2] = {{NEW_SUBJECT_NAME_X, NEW_SUBJECT_NAME_Y, NEW_SUBJECT_NAME_W, NEW_SUBJECT_NAME_H},
        {NEW_SUBJECT_COEFF_X, NEW_SUBJECT_COEFF_Y, NEW_SUBJECT_COEFF_W, NEW_SUBJECT_COEFF_H}
    };
    SDL_Rect curseur = {NEW_SUBJECT_NAME_X + DECAL_CURSEUR_X, NEW_SUBJECT_NAME_Y+(NEW_SUBJECT_NAME_H/2-CURSEUR_H/2),
                        CURSEUR_W, CURSEUR_H
                       };
    SDL_Rect char_rect = {0, 0, CHAR_RECT_W, CURSEUR_H};
    SDL_Point derPosition = {NEW_SUBJECT_NAME_X + DECAL_CURSEUR_X, NEW_SUBJECT_COEFF_Y+(NEW_SUBJECT_COEFF_H/2-CURSEUR_H/2)};
    SDL_Rect texteRect;
    SDL_CreateWindowAndRenderer(NEW_SUBJECT_WINDOW_W, NEW_SUBJECT_WINDOW_H, SDL_WINDOW_BORDERLESS, &fenetre, &render);
    int w, h;
    //______________________________________________________________________________________//fin des créations  et initialisations
    SDL_SetRenderDrawColor(render, 255, 255, 50, 255);
    SDL_RenderClear(render);
    SDL_SetRenderDrawColor(render, 255, 255, 255, 255);
    SDL_RenderFillRects(render, editSpace, 2);
    SET_BLACK(render);
    SDL_RenderDrawRects(render, editSpace, 2);
    TTF_SetFontStyle(myFont, TTF_STYLE_UNDERLINE);
    TTF_SizeText(myFont, "Matiere : ", &w, &h);
    caractere = TTF_RenderText_Blended(myFont, "Matiere :", noire);
    texteRect.w = caractere->w;
    texteRect.h = caractere->h;
    texteRect.x = NEW_SUBJECT_NAME_X-w;
    texteRect.y = NEW_SUBJECT_NAME_Y+(NEW_SUBJECT_NAME_H/2-h/2);
    texCaractere = SDL_CreateTextureFromSurface(render, caractere);
    SDL_RenderCopy(render, texCaractere, NULL, &texteRect);
    SDL_FreeSurface(caractere);
    SDL_DestroyTexture(texCaractere);
    TTF_SizeText(myFont, "Coefficient : ", &w, &h);
    caractere = TTF_RenderText_Blended(myFont, "Coefficient :", noire);
    texCaractere = SDL_CreateTextureFromSurface(render, caractere);
    texteRect.w = caractere->w;
    texteRect.h = caractere->h;
    texteRect.x = NEW_SUBJECT_COEFF_X-w;
    texteRect.y = NEW_SUBJECT_COEFF_Y+(NEW_SUBJECT_NAME_H/2-h/2);
    SDL_RenderCopy(render, texCaractere, NULL, &texteRect);
    SDL_FreeSurface(caractere);
    SDL_DestroyTexture(texCaractere);
    TTF_SetFontStyle(myFont, TTF_STYLE_NORMAL);
    BTN_ChangeEffectColor(button_ok, 200, 200, 0, 80, render);
    if(BTN_CopyButton(button_ok, render) == NULL || button_ok == NULL || fenetre == NULL || render == NULL)
    {
        fprintf(stdout, "impossible d'ouvrir la nouvelle fenetre des nouvelles matieres.");
        quitter = 1;
    }
    else
        SDL_RenderPresent(render);
    SET_INDIGO(render);
    SDL_RenderDrawRect(render, &editSpace[0]);
    while(!quitter)
    {
        SDL_WaitEventTimeout(&event, 850);
        if(event.type == SDL_QUIT)
            exit(1);
        if(event.window.windowID == SDL_GetWindowID(fenetre))
        {
            tn = SDL_GetTicks();
            if(tn - tb >= 1000)
            {
                SDL_SetRenderDrawColor(render, 255, 255, 255, 255);
                SDL_RenderFillRect(render, &curseur);
                if(tn % 2000 >= 1000)
                    tb = tn;
            }
            else
            {
                SDL_SetRenderDrawColor(render, 0, 0, 0, 255);
                SDL_RenderFillRect(render, &curseur);
            }
            char_rect.x = curseur.x;
            char_rect.y = curseur.y;
            char_rect.w = 0;
            switch(event.type)
            {
            case SDL_KEYDOWN:
                SDL_SetRenderDrawColor(render, 255, 255, 255, 255);
                SDL_RenderFillRect(render, &curseur);
                if(event.key.keysym.sym == SDLK_RETURN || event.key.keysym.sym == SDLK_KP_ENTER)  //Si on fini d'écrire dans un champs
                {
                    SET_INDIGO(render);
                    if(curseur.y == NEW_SUBJECT_NAME_Y+(NEW_SUBJECT_NAME_H/2-CURSEUR_H/2))
                    {
                        SDL_RenderDrawRect(render, &editSpace[1]);
                        SET_BLACK(render);
                        SDL_RenderDrawRect(render, &editSpace[0]);
                    }
                    else if(curseur.y == NEW_SUBJECT_COEFF_Y+(NEW_SUBJECT_COEFF_H/2-CURSEUR_H/2))
                    {
                        SDL_RenderDrawRect(render, &editSpace[0]);
                        SET_BLACK(render);
                        SDL_RenderDrawRect(render, &editSpace[1]);
                    }
                    w = curseur.x; //réutilisation des variables w et h: w = x et
                    h = curseur.y; //h = y pour sauvegarder la derniere position du curseur
                    curseur.x = derPosition.x;
                    curseur.y = derPosition.y;
                    derPosition.x = w;
                    derPosition.y = h;
                }
                else if(event.key.keysym.sym == SDLK_BACKSPACE)   //Si on efface un caractère
                {
                    if(curseur.y == NEW_SUBJECT_COEFF_Y+(NEW_SUBJECT_COEFF_H/2-CURSEUR_H/2) && c > 0)
                    {
                        TTF_SizeText(myFont, &coeff[c-1], &char_rect.w, &char_rect.h);
                        c--;
                        coeff[c] = '\0';
                    }
                    else if(curseur.y == NEW_SUBJECT_NAME_Y+(NEW_SUBJECT_NAME_H/2-CURSEUR_H/2) && n > 0)
                    {
                        TTF_SizeText(myFont, &nom[n-1], &char_rect.w, &char_rect.h);
                        n--;
                        nom[n] = '\0';
                    }
                    char_rect.x -= char_rect.w;
                    SDL_RenderFillRect(render, &char_rect);
                    SDL_RenderFillRect(render, &curseur);
                    curseur.x = char_rect.x;
                }
                else   //Sinon, on écrit un caractère normalement
                {
                    if(curseur.y == NEW_SUBJECT_NAME_Y+(NEW_SUBJECT_NAME_H/2-CURSEUR_H/2) && n < MAX_NAME_CHAR)
                    {
                        nom[n] = GetCharPress(event, NULL);
                        if(nom[n] != '\0')
                        {
                            nom[n+1] = '\0';
                            caractere = TTF_RenderText_Blended(myFont, &nom[n], noire);
                            texCaractere = SDL_CreateTextureFromSurface(render, caractere);
                            char_rect.w = caractere->w;
                            char_rect.h = caractere->h;
                            if((curseur.x + char_rect.w) < (NEW_SUBJECT_NAME_X+NEW_SUBJECT_NAME_W))
                            {
                                SDL_RenderCopy(render, texCaractere, NULL, &char_rect);
                                curseur.x += char_rect.w;
                                n++;
                            }
                            else
                                nom[n] = '\0';
                            SDL_FreeSurface(caractere);
                            SDL_DestroyTexture(texCaractere);
                        }
                    }
                    else if(curseur.y == NEW_SUBJECT_COEFF_Y+(NEW_SUBJECT_COEFF_H/2-CURSEUR_H/2) && c < 2)
                    {
                        coeff[c] = GetCharPress(event, NULL);
                        if(coeff[c] != '\0' && isdigit(coeff[c]))
                        {
                            coeff[c+1] = '\0';
                            caractere = TTF_RenderText_Blended(myFont, &coeff[c], noire);
                            texCaractere = SDL_CreateTextureFromSurface(render, caractere);
                            char_rect.w = caractere->w;
                            char_rect.h = caractere->h;
                            if((curseur.x + char_rect.w) < (NEW_SUBJECT_COEFF_X+NEW_SUBJECT_COEFF_W))
                            {
                                SDL_RenderCopy(render, texCaractere, NULL, &char_rect);
                                curseur.x += char_rect.w;
                                c++;
                            }
                            else
                                coeff[c] = '\0';
                            SDL_FreeSurface(caractere);
                            SDL_DestroyTexture(texCaractere);
                        }
                    }
                }
                break;
            case SDL_MOUSEMOTION:
                if(BTN_RenderMouseMotion(button_ok, event, render)) {}
                break;
            case SDL_MOUSEBUTTONDOWN:
                SDL_SetRenderDrawColor(render, 255, 255, 255, 255);
                SDL_RenderFillRect(render, &curseur);
                if(BTN_RenderMouseClicDown(button_ok, event, render))
                {
                    double coefficient = strtod(coeff, NULL);
                    matiere = CreerMatiere(fiche, nom, coefficient);
                    quitter = 1;
                }
                if((event.button.x > editSpace[0].x && event.button.y > editSpace[0].y && event.button.x < editSpace[0].x + editSpace[0].w && event.button.y < editSpace[0].y + editSpace[0].h)
                    || (event.button.x > editSpace[1].x && event.button.y > editSpace[1].y && event.button.x < editSpace[1].x + editSpace[1].w && event.button.y < editSpace[1].y + editSpace[1].h))
                {
                    SET_INDIGO(render);
                    if(curseur.y == NEW_SUBJECT_NAME_Y+(NEW_SUBJECT_NAME_H/2-CURSEUR_H/2))
                    {
                        SDL_RenderDrawRect(render, &editSpace[1]);
                        SET_BLACK(render);
                        SDL_RenderDrawRect(render, &editSpace[0]);
                    }
                    else if(curseur.y == NEW_SUBJECT_COEFF_Y+(NEW_SUBJECT_COEFF_H/2-CURSEUR_H/2))
                    {
                        SDL_RenderDrawRect(render, &editSpace[0]);
                        SET_BLACK(render);
                        SDL_RenderDrawRect(render, &editSpace[1]);
                    }
                    w = curseur.x; //réutilisation des variables w et h: w = x et
                    h = curseur.y; //h = y pour sauvegarder la derniere position du curseur
                    curseur.x = derPosition.x;
                    curseur.y = derPosition.y;
                    derPosition.x = w;
                    derPosition.y = h;
                }
                break;
            }
            SDL_RenderPresent(render);
        }
    }
    SDL_DestroyWindow(fenetre);
    SDL_DestroyRenderer(render);
    printf("sortie de la fonction fenetre_nouvelleMatiere()\n");
    return matiere;
}

Fiche* fenetre_nouvelleFiche()
{
    Fiche *fiche = CreerFiche();
    if(fiche == NULL)
    {
        fprintf(stdout, "La fiche n'a pas pu etre creer :: fenetre_nouvelleFiche().\n");
        return NULL;
    }
    int tn = 0, tb = 0;
    int n = 0;
    int quitter = 0;
    char *nom = malloc((MAX_NAME_CHAR+1) * sizeof(char));
    BTN_Button *button_ok = BTN_CreateButtonWithSurface(BTN_DYNAMIC_BUTTON_FORM_A, OK_BUTTON_W, OK_BUTTON_H, 32, (258*UL_X), (78*UL_Y), "OK", "appData/fonts/taile.ttf");
    SDL_Event event;
    SDL_Surface *caractere = NULL;
    SDL_Window *fenetre = NULL;
    SDL_Rect editSpace = {NEW_FICHENAME_AREA_X, NEW_FICHENAME_AREA_Y, NEW_FICHENAME_AREA_W, NEW_FICHENAME_AREA_H};
    SDL_Rect curseur = {NEW_FICHENAME_AREA_X+DECAL_CURSEUR_X, NEW_FICHENAME_AREA_Y+(NEW_FICHENAME_AREA_H/2-CURSEUR_H/2),
                        CURSEUR_W, CURSEUR_H
                       };
    SDL_Rect texteRect;
    fenetre = SDL_CreateWindow(NULL, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, NEW_FICHE_WINDOW_W, NEW_FICHE_WINDOW_H, SDL_WINDOW_BORDERLESS);
    int w, h;
    //______________________________________________________________________________________//fin des créations  et initialisations
    SDL_FillRect(SDL_GetWindowSurface(fenetre), NULL, SDL_MapRGB(SDL_GetWindowSurface(fenetre)->format, 64, 72, 204));
    SDL_FillRect(SDL_GetWindowSurface(fenetre), &editSpace, SDL_MapRGB(SDL_GetWindowSurface(fenetre)->format, 255, 255, 255));
    TTF_SetFontStyle(myFont, TTF_STYLE_UNDERLINE);
    TTF_SizeText(myFont, "Nom : ", &w, &h);
    caractere = TTF_RenderText_Blended(myFont, "Nom :", noire);
    texteRect.w = w;
    texteRect.h = h;
    texteRect.x = NEW_FICHENAME_AREA_X-w;
    texteRect.y = NEW_FICHENAME_AREA_Y+(NEW_FICHENAME_AREA_H/2-h/2);
    SDL_BlitSurface(caractere, NULL, SDL_GetWindowSurface(fenetre), &texteRect);
    SDL_FreeSurface(caractere);
    TTF_SetFontStyle(myFont, TTF_STYLE_NORMAL);
    if(button_ok == NULL || BTN_BlitButton(button_ok, SDL_GetWindowSurface(fenetre)) < 0 || fenetre == NULL)
    {
        fprintf(stdout, "impossible d'ouvrir la nouvelle fenetre de nouvelle fiche :: fenetre_nouvelleFiche().");
        quitter = 1;
    }
    else
        SDL_UpdateWindowSurface(fenetre);
    while(!quitter)
    {
        SDL_WaitEventTimeout(&event, 850);
        if(event.type == SDL_QUIT)
            exit(1);
        else if(event.window.windowID == SDL_GetWindowID(fenetre))
        {
            tn = SDL_GetTicks();
            if(tn - tb >= 1000)
            {
                SDL_FillRect(SDL_GetWindowSurface(fenetre), &curseur, SDL_MapRGB(SDL_GetWindowSurface(fenetre)->format, 255, 255, 255));
                if(tn % 2000 >= 1000)
                    tb = tn;
            }
            else
            {
                SDL_FillRect(SDL_GetWindowSurface(fenetre), &curseur, SDL_MapRGB(SDL_GetWindowSurface(fenetre)->format, 0, 0, 0));
            }
            texteRect.x = curseur.x;
            texteRect.y = curseur.y;
            texteRect.w = 0;
            switch(event.type)
            {
            case SDL_KEYDOWN:
                SDL_FillRect(SDL_GetWindowSurface(fenetre), &curseur, SDL_MapRGB(SDL_GetWindowSurface(fenetre)->format, 255, 255, 255));
                if(event.key.keysym.sym == SDLK_RETURN || event.key.keysym.sym == SDLK_KP_ENTER)  //Si on fini d'écrire
                {
                    fiche->nom = nom;
                    quitter = 1;
                }
                else if(event.key.keysym.sym == SDLK_BACKSPACE && n > 0)   //Si on efface un caractère
                {
                    TTF_SizeText(myFont, &nom[n-1], &texteRect.w, &texteRect.h);
                    n--;
                    nom[n] = '\0';
                    texteRect.x -= texteRect.w;
                    SDL_FillRect(SDL_GetWindowSurface(fenetre), &texteRect, SDL_MapRGB(SDL_GetWindowSurface(fenetre)->format, 255, 255, 255));
                    SDL_FillRect(SDL_GetWindowSurface(fenetre), &curseur, SDL_MapRGB(SDL_GetWindowSurface(fenetre)->format, 255, 255, 255));
                    curseur.x = texteRect.x;
                }
                else if(n < MAX_NAME_CHAR)   //Sinon, on écrit un caractère normalement)
                {
                    nom[n] = GetCharPress(event, NULL);
                    if(nom[n] != '\0')
                    {
                        nom[n+1] = '\0';
                        caractere = TTF_RenderText_Blended(myFont, &nom[n], noire);
                        texteRect.w = caractere->w;
                        texteRect.h = caractere->h;
                        if((curseur.x + texteRect.w) < (NEW_FICHENAME_AREA_X+NEW_FICHENAME_AREA_W))
                        {
                            SDL_BlitSurface(caractere, NULL, SDL_GetWindowSurface(fenetre), &texteRect);
                            curseur.x += texteRect.w;
                            n++;
                        }
                        else
                            nom[n] = '\0';
                        SDL_FreeSurface(caractere);
                    }
                }
                break;
            case SDL_MOUSEMOTION:
                if(BTN_SurfaceMouseMotion(button_ok, event, SDL_GetWindowSurface(fenetre))) {}
                break;
            case SDL_MOUSEBUTTONDOWN:
                if(BTN_SurfaceMouseClicDown(button_ok, event, SDL_GetWindowSurface(fenetre))) {}
                break;
            case SDL_MOUSEBUTTONUP:
                if(BTN_SurfaceMouseClicUp(button_ok, event, SDL_GetWindowSurface(fenetre)))
                {
                    fiche->nom = nom;
                    quitter = 1;
                }
                break;
            }
            SDL_UpdateWindowSurface(fenetre);
        }
    }
    BTN_FreeButton(button_ok);
    SDL_DestroyWindow(fenetre);
    printf("sortie de la fonction fenetre_nouvelleFiche()\n");
    return fiche;
}

Fiche* fenetre_chargerFiche(Liste_f *fiches, SDL_Renderer *fRender)
{
    Fiche *fiche = CreerFiche();
    if(fiche == NULL)
    {
        fprintf(stdout, "La fiche n'a pas pu etre creer :: fenetre_nouvelleFiche().\n");
        return NULL;
    }
    int tn = 0, tb = 0;
    int n = 0;
    int quitter = 0;
    char *nom = malloc(100 * sizeof(char));
    BTN_Button *button_ok = BTN_CreateButtonWithSurface(BTN_DYNAMIC_BUTTON_FORM_A, OK_BUTTON_W, OK_BUTTON_H, 32, (258*UL_X), (78*UL_Y), "OK", "appData/fonts/taile.ttf");
    SDL_Event event;
    SDL_Surface *caractere = NULL;
    SDL_Window *fenetre = NULL;
    SDL_Rect editSpace = {NEW_FICHENAME_AREA_X, NEW_FICHENAME_AREA_Y, NEW_FICHENAME_AREA_W, NEW_FICHENAME_AREA_H};
    SDL_Rect curseur = {NEW_FICHENAME_AREA_X+DECAL_CURSEUR_X, NEW_FICHENAME_AREA_Y+(NEW_FICHENAME_AREA_H/2-CURSEUR_H/2),
                        CURSEUR_W, CURSEUR_H
                       };
    SDL_Rect texteRect;
    fenetre = SDL_CreateWindow(NULL, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, NEW_FICHE_WINDOW_W, NEW_FICHE_WINDOW_H, SDL_WINDOW_BORDERLESS);
    int w, h;
    //______________________________________________________________________________________//fin des créations  et initialisations
    SDL_FillRect(SDL_GetWindowSurface(fenetre), NULL, SDL_MapRGB(SDL_GetWindowSurface(fenetre)->format, 64, 72, 204));
    SDL_FillRect(SDL_GetWindowSurface(fenetre), &editSpace, SDL_MapRGB(SDL_GetWindowSurface(fenetre)->format, 255, 255, 255));
    TTF_SetFontStyle(myFont, TTF_STYLE_UNDERLINE);
    TTF_SizeText(myFont, "Nom : ", &w, &h);
    caractere = TTF_RenderText_Blended(myFont, "Nom :", noire);
    texteRect.w = w;
    texteRect.h = h;
    texteRect.x = NEW_FICHENAME_AREA_X-w;
    texteRect.y = NEW_FICHENAME_AREA_Y+(NEW_FICHENAME_AREA_H/2-h/2);
    SDL_BlitSurface(caractere, NULL, SDL_GetWindowSurface(fenetre), &texteRect);
    SDL_FreeSurface(caractere);
    TTF_SetFontStyle(myFont, TTF_STYLE_NORMAL);
    if(button_ok == NULL || BTN_BlitButton(button_ok, SDL_GetWindowSurface(fenetre)) < 0 || fenetre == NULL)
    {
        fprintf(stdout, "impossible d'ouvrir la nouvelle fenetre de nouvelle fiche :: fenetre_nouvelleFiche().");
        quitter = 1;
    }
    else
        SDL_UpdateWindowSurface(fenetre);
    while(!quitter)
    {
        SDL_WaitEventTimeout(&event, 850);
        if(event.type == SDL_QUIT)
            exit(1);
        else if(event.window.windowID == SDL_GetWindowID(fenetre))
        {
            tn = SDL_GetTicks();
            if(tn - tb >= 1000)
            {
                SDL_FillRect(SDL_GetWindowSurface(fenetre), &curseur, SDL_MapRGB(SDL_GetWindowSurface(fenetre)->format, 255, 255, 255));
                if(tn % 2000 >= 1000)
                    tb = tn;
            }
            else
            {
                SDL_FillRect(SDL_GetWindowSurface(fenetre), &curseur, SDL_MapRGB(SDL_GetWindowSurface(fenetre)->format, 0, 0, 0));
            }
            texteRect.x = curseur.x;
            texteRect.y = curseur.y;
            texteRect.w = 0;
            switch(event.type)
            {
            case SDL_KEYDOWN:
                SDL_FillRect(SDL_GetWindowSurface(fenetre), &curseur, SDL_MapRGB(SDL_GetWindowSurface(fenetre)->format, 255, 255, 255));
                if(event.key.keysym.sym == SDLK_RETURN || event.key.keysym.sym == SDLK_KP_ENTER)  //Si on fini d'écrire
                {
                    fiche = charger_fiche(fiches, fRender, nom);
                    quitter = 1;
                }
                else if(event.key.keysym.sym == SDLK_BACKSPACE && n > 0)   //Si on efface un caractère
                {
                    TTF_SizeText(myFont, &nom[n-1], &texteRect.w, &texteRect.h);
                    n--;
                    nom[n] = '\0';
                    texteRect.x -= texteRect.w;
                    SDL_FillRect(SDL_GetWindowSurface(fenetre), &texteRect, SDL_MapRGB(SDL_GetWindowSurface(fenetre)->format, 255, 255, 255));
                    SDL_FillRect(SDL_GetWindowSurface(fenetre), &curseur, SDL_MapRGB(SDL_GetWindowSurface(fenetre)->format, 255, 255, 255));
                    curseur.x = texteRect.x;
                }
                else if(n < MAX_NAME_CHAR)   //Sinon, on écrit un caractère normalement)
                {
                    nom[n] = GetCharPress(event, NULL);
                    if(nom[n] != '\0')
                    {
                        nom[n+1] = '\0';
                        caractere = TTF_RenderText_Blended(myFont, &nom[n], noire);
                        texteRect.w = caractere->w;
                        texteRect.h = caractere->h;
                        if((curseur.x + texteRect.w) < (NEW_FICHENAME_AREA_X+NEW_FICHENAME_AREA_W))
                        {
                            SDL_BlitSurface(caractere, NULL, SDL_GetWindowSurface(fenetre), &texteRect);
                            curseur.x += texteRect.w;
                            n++;
                        }
                        else
                            nom[n] = '\0';
                        SDL_FreeSurface(caractere);
                    }
                }
                break;
            case SDL_MOUSEMOTION:
                if(BTN_SurfaceMouseMotion(button_ok, event, SDL_GetWindowSurface(fenetre))) {}
                break;
            case SDL_MOUSEBUTTONDOWN:
                if(BTN_SurfaceMouseClicDown(button_ok, event, SDL_GetWindowSurface(fenetre))) {}
                break;
            case SDL_MOUSEBUTTONUP:
                if(BTN_SurfaceMouseClicUp(button_ok, event, SDL_GetWindowSurface(fenetre)))
                {
                    fiche = charger_fiche(fiches, fRender, nom);
                    quitter = 1;
                }
                break;
            }
            SDL_UpdateWindowSurface(fenetre);
        }
    }
    BTN_FreeButton(button_ok);
    SDL_DestroyWindow(fenetre);
    printf("sortie de la fonction fenetre_nouvelleFiche()\n");
    return fiche;
}

void fenetre_supprimerMatiere(Fiche *fiche)
{
    if(fiche == NULL || fiche->list_matiere->n == 0)
        return;
    int choix = 0;
    int i = 0;
    SDL_MessageBoxColorScheme *color = malloc(sizeof(*color));
    color->colors->r = 100;
    color->colors->g = 0;
    color->colors->b = 125;
    SDL_MessageBoxData messageData;
    SDL_MessageBoxButtonData buttons[fiche->list_matiere->n];
    for(i = 0; i < fiche->list_matiere->n; i++)
    {
        buttons[i].text = fiche->list_matiere->addrMatiere[i]->nom;
        buttons[i].buttonid = i;
        buttons[i].flags = SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT;
    }
    messageData.window = NULL;
    messageData.title = NULL;
    messageData.numbuttons = fiche->list_matiere->n;
    messageData.message = "Veuillez selectionner la matiere a supprimer.\n";
    messageData.flags = SDL_MESSAGEBOX_BUTTONS_RIGHT_TO_LEFT;
    messageData.buttons = buttons;
    messageData.colorScheme = color;
    SDL_ShowMessageBox(&messageData, &choix);
    SupprimerMatiere(fiche->list_matiere->addrMatiere[choix], fiche);
    SDL_SetRenderTarget(fiche->render, fiche->texture);
    RetracerGraphe(fiche, choix);
    SDL_Rect r = {LIGNE_X, LIGNE_DEPART_Y, LIGNE_W, fiche->list_matiere->n*LIGNE_H};
    SET_JPBLUE(fiche->render);
    SDL_RenderFillRect(fiche->render, &r);
    free(color);
    while(choix+1 < fiche->list_matiere->n)
    {
        fiche->list_matiere->addrMatiere[choix] = fiche->list_matiere->addrMatiere[choix+1];
        choix++;
    }
    fiche->list_matiere->addrMatiere[choix] = NULL;
    SDL_RenderFillRect(fiche->render, fiche->bouton_plus->rect);
    fiche->list_matiere->n--;
    fiche->bouton_plus->rect->y = BOUTON_PLUS_Y(fiche);
    BTN_CopyButton(fiche->bouton_plus, fiche->render);
    SET_BLACK(fiche->render);
    SDL_RenderDrawLine(fiche->render, TABLEAU_TETE_X, LIGNE_DEPART_Y, TABLEAU_TETE_X+TABLEAU_TETE_W, LIGNE_DEPART_Y);
    r.h = LIGNE_H;
    for(i = 0; i < fiche->list_matiere->n; i++)
    {
        r.y = LIGNE_DEPART_Y+(i*LIGNE_H);
        SDL_RenderCopy(fiche->render, fiche->list_matiere->addrMatiere[i]->ligne, NULL, &r);
    }
    SDL_SetRenderTarget(fiche->render, NULL);
    SDL_RenderCopy(fiche->render, fiche->texture, NULL, &fiche->surface_rect);
}
