#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include "../inc/constantes.h"
#include "../inc/variables.h"
#include "../inc/KeyDown.h"
#include "../inc/lireNote.h"
#include "../inc/ajouter.h"

Return_Bool SaisirNote(Fiche *fiche, SDL_Event *ficheEvent)
{
    if(ficheEvent->button.x > LIGNE_X && ficheEvent->button.x < LIGNE_X+TABLEAU_TETE_W
            && ficheEvent->button.y > LIGNE_DEPART_Y && ficheEvent->button.y < LIGNE_DEPART_Y+(LIGNE_H*fiche->list_matiere->n))
    {
        SDL_SetRenderTarget(fiche->render, fiche->texture);
        short note_indice = 0, matiere = 0;
        if(!TrouverPositionCurseur(fiche, &ficheEvent->button, &matiere, &note_indice))
            return error;
        SDL_Rect note_rect = {DEBUT_NOTE_X + (note_indice*CASE_W), LIGNE_DEPART_Y + (matiere*LIGNE_H), CASE_W+UL_X, CASE_H};
        if(note_indice == 8)
            note_rect.w += UL_X;
        if(note_indice == 9)
            note_rect.x = TABLEAU_TETE_X + MOY_DEV_X;
        if(note_indice == 10)
            note_rect.x = TABLEAU_TETE_X + MOY_SUR_20_X;
        if(note_indice == 11)
            note_rect.x = TABLEAU_TETE_X + MOY_COEF_X;
        SET_JPBLUE(fiche->render);
        SDL_RenderFillRect(fiche->render, &note_rect);
        SET_BLACK(fiche->render);
        SDL_RenderDrawRect(fiche->render, &note_rect);
        SDL_Surface *caractere = NULL;
        SDL_Texture *texture = NULL;
        char note[6] = {'-', '1', '.', '0', '0', '\0'};
        short n = 0;
        SDL_Event event;
        SDL_bool enregistrer = SDL_FALSE;
        int tn = 0, tb = 0;
        fiche->curseur.x += note_rect.w/2;
        SDL_Rect caracter_rect = {fiche->curseur.x, fiche->curseur.y, 0, fiche->curseur.h};
        while(!enregistrer)
        {
            SDL_WaitEventTimeout(&event, 850);
            switch(event.type)
            {
            case SDL_QUIT:
                return quit_window;
                break;
            case SDL_KEYDOWN:
                if(event.key.keysym.sym == SDLK_RETURN || event.key.keysym.sym == SDLK_KP_ENTER) //Si on fini d'écrire
                {
                    SET_JPBLUE(fiche->render);
                    SDL_RenderFillRect(fiche->render, &fiche->curseur);
                    enregistrer = SDL_TRUE;
                }
                else if(event.key.keysym.sym == SDLK_BACKSPACE && n > 0) //Si on efface un caractère
                {
                    n--;
                    note[n] = '\0';
                    SET_JPBLUE(fiche->render);
                    SDL_RenderFillRect(fiche->render, &note_rect);
                    SET_BLACK(fiche->render);
                    SDL_RenderDrawRect(fiche->render, &note_rect);
                    fiche->curseur.x = note_rect.x+(note_rect.w/2);
                    if(n > 0)
                    {
                        caractere = TTF_RenderText_Blended(myFont, note, noire);
                        texture = SDL_CreateTextureFromSurface(fiche->render, caractere);
                        caracter_rect.x = (note_rect.x+(note_rect.w/2))-caractere->w/2;
                        caracter_rect.w = caractere->w*UL_X;
                        SDL_RenderCopy(fiche->render, texture, NULL, &caracter_rect);
                        fiche->curseur.x = caracter_rect.x + caracter_rect.w;

                        SDL_FreeSurface(caractere);
                        SDL_DestroyTexture(texture);
                    }
                    SDL_RenderFillRect(fiche->render, &fiche->curseur);
                }
                else if(n < 5) //Sinon, on écrit un caractère normalement mais on vérifie d'abord si le nombre de caractère n'atteint pas déjà 5
                {
                    note[n] = GetCharPress(event, NULL);
                    if(note[n] != '\0' && (isalnum(note[n]) || note[n] == ',' || note[n] == '.'))
                    {
                        if(note[n] == ',')
                            note[n] = '.';
                        note[n+1] = '\0';
                        caractere = TTF_RenderText_Blended(myFont, note, noire);
                        texture = SDL_CreateTextureFromSurface(fiche->render, caractere);
                        caracter_rect.w = caractere->w*UL_X;
                        caracter_rect.h = caractere->h*UL_Y;
                        if(caracter_rect.w < (note_rect.w-DECAL_CURSEUR_X))
                        {
                            SET_JPBLUE(fiche->render);
                            SDL_RenderFillRect(fiche->render, &note_rect);
                            SET_BLACK(fiche->render);
                            SDL_RenderDrawRect(fiche->render, &note_rect);
                            caracter_rect.x = (note_rect.x+(note_rect.w/2))-caracter_rect.w/2;
                            SDL_RenderCopy(fiche->render, texture, NULL, &caracter_rect);
                            fiche->curseur.x = caracter_rect.x + caracter_rect.w;
                            n++;
                            SDL_FreeSurface(caractere);
                            SDL_DestroyTexture(texture);
                        }
                        else
                        {
                            note[n] = '\0';
                            SDL_FreeSurface(caractere);
                            SDL_DestroyTexture(texture);
                        }
                    }
                }
                break;
            default:
                tn = SDL_GetTicks();
                if(tn - tb >= 1000)
                {
                    SET_JPBLUE(fiche->render);
                    SDL_RenderFillRect(fiche->render, &fiche->curseur);
                    if(tn % 3000 >= 1500)
                        tb = tn;
                }
                else
                {
                    SET_BLACK(fiche->render);
                    SDL_RenderFillRect(fiche->render, &fiche->curseur);
                }
                break;
            }
            SDL_SetRenderTarget(fiche->render, NULL);
            SDL_RenderCopy(fiche->render, fiche->texture, NULL, &fiche->surface_rect);
            SDL_RenderPresent(fiche->render);
            SDL_SetRenderTarget(fiche->render, fiche->texture);
        }
        if(texture != NULL)
            SET_JPBLUE(fiche->render);
        SDL_RenderFillRect(fiche->render, &fiche->curseur);
        double numNote = -1.0;
        numNote = strtod(note, NULL);
        return EnregistrerNote(fiche, numNote, matiere, note_indice);
    }
    else
        return error;
}

void ChangerPositionCurseur(Fiche *fiche, int x, int y)//fini
{
    SET_JPBLUE(fiche->render);
    SDL_RenderFillRect(fiche->render, &fiche->curseur);
    SDL_SetRenderTarget(fiche->render, NULL);
    SDL_RenderCopy(fiche->render, fiche->texture, NULL, &fiche->surface_rect);
    SDL_RenderPresent(fiche->render);
    SDL_SetRenderTarget(fiche->render, fiche->texture);

    fiche->curseur.x = x - DECAL_CURSEUR_X;
    fiche->curseur.y = y;
}

void AjusterPositionCurseur(Fiche *fiche, int i, int j)//fini
{
    int y = (LIGNE_DEPART_Y + (i*LIGNE_H)) + (LIGNE_H/2 - fiche->curseur.h/2);
    int x =  DEBUT_NOTE_X + ((j*CASE_W) + DECAL_CURSEUR_X);
    if(j == 9)
        x = TABLEAU_TETE_X + MOY_DEV_X + DECAL_CURSEUR_X;
    if(j == 10)
        x = TABLEAU_TETE_X + MOY_SUR_20_X + DECAL_CURSEUR_X;
    if(j == 11)
        x = TABLEAU_TETE_X + MOY_COEF_X + DECAL_CURSEUR_X;
    ChangerPositionCurseur(fiche, x, y);
}

Return_Bool TrouverPositionCurseur(Fiche *fiche, SDL_MouseButtonEvent *mouse, short *indMat, short *indNt)//fini
{
    *indMat = (mouse->y - LIGNE_DEPART_Y) / CASE_H;
    if(*indMat >= fiche->list_matiere->n)
        return error;
    *indNt = (mouse->x - DEBUT_NOTE_X) / CASE_W;
    if(*indNt >= 12)
        return error;
    AjusterPositionCurseur(fiche, *indMat, *indNt);
    return ok;
}

Return_Bool EnregistrerNote(Fiche *fiche, double note, short indiceMatiere, short indiceNote)//fini
{
    if(indiceNote < 6 || indiceNote == 7 || indiceNote == 8)
    {
        if(AjouterNote(fiche->list_matiere->addrMatiere[indiceMatiere], note, indiceNote, fiche->render))
            return TracerGraphe(fiche, note, (indiceMatiere*8)+indiceNote);
        else
            return error;
    }
    else if(indiceNote == 6 || indiceNote == 9 || indiceNote == 10 || indiceNote == 11)
        return AjouterMoy(fiche->list_matiere->addrMatiere[indiceMatiere], note, indiceNote);
    else
        return error;
}
