#include <stdio.h>
#include <stdlib.h>

#include "../inc/variables.h"
#include "../inc/klcul_moy.h"

void klcul_moyennes(Fiche *fiche)
{
    if(fiche == NULL)
        return;
    SDL_Rect ligne = {LIGNE_X, LIGNE_DEPART_Y, LIGNE_W, LIGNE_H};
    short i = 0;
    while(i < fiche->list_matiere->n)
    {
        SDL_SetRenderTarget(fiche->render, fiche->list_matiere->addrMatiere[i]->ligne);
        if(fiche->list_matiere->addrMatiere[i]->moy_int < 0)
            moy_int(fiche->list_matiere->addrMatiere[i], fiche->render);
        if(fiche->list_matiere->addrMatiere[i]->moy_dev < 0)
            moy_dev(fiche->list_matiere->addrMatiere[i], fiche->render);
        if(fiche->list_matiere->addrMatiere[i]->moy_totale < 0)
            moy_mat(fiche->list_matiere->addrMatiere[i], fiche->render);
        SDL_SetRenderTarget(fiche->render, fiche->texture);
        ligne.y = LIGNE_DEPART_Y+(i*LIGNE_H);
        SDL_RenderCopy(fiche->render, fiche->list_matiere->addrMatiere[i]->ligne, NULL, &ligne);
        i++;
    }
    SDL_SetRenderTarget(fiche->render, fiche->texture);
    bilan(fiche);
    SDL_SetRenderTarget(fiche->render, NULL);
    SDL_RenderCopy(fiche->render, fiche->texture, NULL, &fiche->surface_rect);
    SDL_RenderPresent(fiche->render);
}

void moy_int(Matiere *matiere, SDL_Renderer *render)
{
    /*short int i = 0;
    for(i = 0; i < matiere->interrogations->max_note; i++)
    {
        if(matiere->interrogations->notes[i] >= 0)
            matiere->interrogations->somme += matiere->interrogations->notes[i];
    }*/
    SDL_Surface *surMoyenne = NULL;
    SDL_Texture *texMoyenne = NULL;
    SDL_Rect rectMoyenne;
    char strMoyenne[6];
    matiere->moy_int = matiere->interrogations->somme/matiere->interrogations->n;
    sprintf(strMoyenne, "%.2lf", matiere->moy_int);
    surMoyenne = TTF_RenderText_Blended(myFont, strMoyenne, noire);
    texMoyenne = SDL_CreateTextureFromSurface(render, surMoyenne);
    rectMoyenne.x = MOY_INT_X + DECAL_CURSEUR_X;
    rectMoyenne.y = (CASE_H/2)-(surMoyenne->h/2);
    rectMoyenne.w = surMoyenne->w;
    rectMoyenne.h = surMoyenne->h;
    SDL_RenderCopy(render, texMoyenne, NULL, &rectMoyenne);
    SDL_FreeSurface(surMoyenne);
    SDL_DestroyTexture(texMoyenne);
}

void moy_dev(Matiere *matiere, SDL_Renderer *render)
{
    /*short int i = 0;
    for(i = 0; i < matiere->devoirs->max_note; i++)
    {
        if(matiere->devoirs->notes[i] >= 0)
            matiere->devoirs->somme += matiere->devoirs->notes[i];
    }*/
    SDL_Surface *surMoyenne = NULL;
    SDL_Texture *texMoyenne = NULL;
    SDL_Rect rectMoyenne;
    char strMoyenne[6];
    matiere->moy_dev = matiere->devoirs->somme/matiere->devoirs->n;
    sprintf(strMoyenne, "%.2lf", matiere->moy_dev);
    surMoyenne = TTF_RenderText_Blended(myFont, strMoyenne, noire);
    texMoyenne = SDL_CreateTextureFromSurface(render, surMoyenne);
    rectMoyenne.x = MOY_DEV_X + DECAL_CURSEUR_X;
    rectMoyenne.y = (CASE_H/2)-(surMoyenne->h/2);
    rectMoyenne.w = surMoyenne->w;
    rectMoyenne.h = surMoyenne->h;
    SDL_RenderCopy(render, texMoyenne, NULL, &rectMoyenne);
    SDL_FreeSurface(surMoyenne);
    SDL_DestroyTexture(texMoyenne);
}

void moy_mat(Matiere *matiere, SDL_Renderer *render)
{
    matiere->moy_totale = (matiere->moy_int + matiere->devoirs->somme)/(1+matiere->devoirs->n);
    matiere->moy_coeff = matiere->moy_totale * matiere->coefficient;
    SDL_Surface *surMoyenne = NULL;
    SDL_Texture *texMoyenne = NULL;
    SDL_Rect rectMoyenne;
    char strMoyenne[6];
    sprintf(strMoyenne, "%.2lf", matiere->moy_totale);
    surMoyenne = TTF_RenderText_Blended(myFont, strMoyenne, noire);
    texMoyenne = SDL_CreateTextureFromSurface(render, surMoyenne);
    rectMoyenne.x = MOY_SUR_20_X + DECAL_CURSEUR_X;
    rectMoyenne.y = (CASE_H/2)-(surMoyenne->h/2);
    rectMoyenne.w = surMoyenne->w;
    rectMoyenne.h = surMoyenne->h;
    SDL_RenderCopy(render, texMoyenne, NULL, &rectMoyenne);
    SDL_FreeSurface(surMoyenne);
    SDL_DestroyTexture(texMoyenne);
    sprintf(strMoyenne, "%.2lf", matiere->moy_coeff);
    surMoyenne = TTF_RenderText_Blended(myFont, strMoyenne, noire);
    texMoyenne = SDL_CreateTextureFromSurface(render, surMoyenne);
    rectMoyenne.x = MOY_COEF_X + DECAL_CURSEUR_X;
    rectMoyenne.y = (CASE_H/2)-(surMoyenne->h/2);
    rectMoyenne.w = surMoyenne->w;
    rectMoyenne.h = surMoyenne->h;
    SDL_RenderCopy(render, texMoyenne, NULL, &rectMoyenne);
    SDL_FreeSurface(surMoyenne);
    SDL_DestroyTexture(texMoyenne);
    char *mention = malloc(MAX_MENTION_CHAR*sizeof(char));
    if(matiere->moy_totale >= 9 && matiere->moy_totale < 12)
        mention = "Passable";
    else if(matiere->moy_totale >= 12 && matiere->moy_totale < 14)
        mention = "Assez-Bien";
    else if(matiere->moy_totale >= 14 && matiere->moy_totale < 16)
        mention = "Bien";
    else if(matiere->moy_totale >= 16 && matiere->moy_totale <= 18)
        mention = "Très Bien";
    else if(matiere->moy_totale > 18 && matiere->moy_totale <= 20)
        mention = "Exellent";
    else
        mention = "Insuffisant";
    matiere->appreciation = mention;
    surMoyenne = TTF_RenderText_Blended(myFont, mention, noire);
    texMoyenne = SDL_CreateTextureFromSurface(render, surMoyenne);
    rectMoyenne.x = APPRECIATION_X + (APPRECIATION_W/2) - (surMoyenne->w/2);
    rectMoyenne.y = (APPRECIATION_H/2)-(surMoyenne->h/2);
    rectMoyenne.w = surMoyenne->w;
    rectMoyenne.h = surMoyenne->h;
    SDL_RenderCopy(render, texMoyenne, NULL, &rectMoyenne);
    SDL_FreeSurface(surMoyenne);
    SDL_DestroyTexture(texMoyenne);
}

void bilan(Fiche *fiche)
{
    char *mention = malloc(MAX_MENTION_CHAR*sizeof(char));
    short i = 0;
    while(i < fiche->list_matiere->n)
    {
        fiche->list_matiere->bilan.moyCoef += fiche->list_matiere->addrMatiere[i]->moy_coeff;
        fiche->list_matiere->bilan.moyDev += fiche->list_matiere->addrMatiere[i]->moy_dev;
        fiche->list_matiere->bilan.moyInt += fiche->list_matiere->addrMatiere[i]->moy_int;
        fiche->list_matiere->bilan.totaux_coefficients += fiche->list_matiere->addrMatiere[i]->coefficient;
        i++;
    }
    fiche->list_matiere->bilan.moySur20 = fiche->list_matiere->bilan.moyCoef/fiche->list_matiere->bilan.totaux_coefficients;
    if(fiche->list_matiere->bilan.moySur20 >= 9 && fiche->list_matiere->bilan.moySur20 < 12)
        mention = "Passable";
    else if(fiche->list_matiere->bilan.moySur20 >= 12 && fiche->list_matiere->bilan.moySur20 < 14)
        mention = "Assez-Bien";
    else if(fiche->list_matiere->bilan.moySur20 >= 14 && fiche->list_matiere->bilan.moySur20 < 16)
        mention = "Bien";
    else if(fiche->list_matiere->bilan.moySur20 >= 16 && fiche->list_matiere->bilan.moySur20 < 18)
        mention = "Très Bien";
    else if(fiche->list_matiere->bilan.moySur20 >= 18 && fiche->list_matiere->bilan.moySur20 < 19)
        mention = "Exellent";
    else if(fiche->list_matiere->bilan.moySur20 >= 19 && fiche->list_matiere->bilan.moySur20 <= 20)
        mention = "Génial";
    else
        mention = "Insuffisant";
    fiche->list_matiere->bilan.mention = mention;
    afficherBilan(fiche);
}

void afficherBilan(Fiche *fiche)
{
    TTF_Font *bilanFont = NULL;
    SDL_Surface *surTitre = NULL;
    SDL_Surface *surMoyenne = NULL;
    SDL_Texture *texTitre = NULL;
    SDL_Texture *texMoyenne = NULL;
    SDL_Rect rectTitre;
    SDL_Rect rectMoyenne;
    char strMoyenne[6];
//********************************************************************************************************************************//
    bilanFont = TTF_OpenFont("appData/fonts/taile.ttf", 18);
    SET_INDIGO(fiche->render);
    SDL_RenderDrawLine(fiche->render, BILAN_X, BILAN_Y, GRAPHE_X, BILAN_Y);
    /*Bilan*/
    TTF_SetFontStyle(bilanFont, TTF_STYLE_UNDERLINE);
    TTF_SetFontStyle(bilanFont, TTF_STYLE_BOLD);
    surTitre = TTF_RenderText_Blended(bilanFont, "Bilan (Format Jean Piaget)", noire);
    texTitre = SDL_CreateTextureFromSurface(fiche->render, surTitre);
    rectTitre.x = ((BILAN_W-GRAPHE_W)/2)-(surTitre->w/2);
    rectTitre.y = BILAN_Y+BILAN_TITRE_DECALAGE_Y;
    rectTitre.w = surTitre->w;
    rectTitre.h = surTitre->h;
    SDL_RenderCopy(fiche->render, texTitre, NULL, &rectTitre);
    SDL_FreeSurface(surTitre);
    SDL_DestroyTexture(texTitre);

    TTF_SetFontStyle(bilanFont, TTF_STYLE_NORMAL);
    rectTitre.x = BILAN_X+BILAN_TITRE_DECALAGE_X;
    /*Moyenne interrogation*/
    TTF_SetFontStyle(bilanFont, TTF_STYLE_UNDERLINE);
    surTitre = TTF_RenderText_Blended(bilanFont, "Moyenne Interrogations :", noire);
    texTitre = SDL_CreateTextureFromSurface(fiche->render, surTitre);
    rectTitre.y = rectTitre.y + rectTitre.h + BILAN_TITRE_DECALAGE_Y;
    rectTitre.w = surTitre->w;
    rectTitre.h = surTitre->h;
    SDL_RenderCopy(fiche->render, texTitre, NULL, &rectTitre);
    SDL_FreeSurface(surTitre);
    SDL_DestroyTexture(texTitre);
    TTF_SetFontStyle(bilanFont, TTF_STYLE_NORMAL);
    sprintf(strMoyenne, "%.2f", fiche->list_matiere->bilan.moyInt);
    surMoyenne = TTF_RenderText_Blended(bilanFont, strMoyenne, noire);
    texMoyenne = SDL_CreateTextureFromSurface(fiche->render, surMoyenne);
    rectMoyenne.x = (rectTitre.x + rectTitre.w) + BILAN_MOYENNE_DECALAGE_X;
    rectMoyenne.y = rectTitre.y;
    rectMoyenne.w = surMoyenne->w;
    rectMoyenne.h = surMoyenne->h;
    SDL_RenderCopy(fiche->render, texMoyenne, NULL, &rectMoyenne);
    SDL_FreeSurface(surMoyenne);
    SDL_DestroyTexture(texMoyenne);
    /*Moyenne devoir*/
    TTF_SetFontStyle(bilanFont, TTF_STYLE_UNDERLINE);
    surTitre = TTF_RenderText_Blended(bilanFont, "Moyenne Devoirs :", noire);
    texTitre = SDL_CreateTextureFromSurface(fiche->render, surTitre);
    rectTitre.y = rectTitre.y + rectTitre.h + BILAN_TITRE_DECALAGE_Y;
    rectTitre.w = surTitre->w;
    rectTitre.h = surTitre->h;
    SDL_RenderCopy(fiche->render, texTitre, NULL, &rectTitre);
    SDL_FreeSurface(surTitre);
    SDL_DestroyTexture(texTitre);
    TTF_SetFontStyle(bilanFont, TTF_STYLE_NORMAL);
    sprintf(strMoyenne, "%.2f", fiche->list_matiere->bilan.moyDev);
    surMoyenne = TTF_RenderText_Blended(bilanFont, strMoyenne, noire);
    texMoyenne = SDL_CreateTextureFromSurface(fiche->render, surMoyenne);
    rectMoyenne.x = (rectTitre.x + rectTitre.w) + BILAN_MOYENNE_DECALAGE_X;
    rectMoyenne.y = rectTitre.y;
    rectMoyenne.w = surMoyenne->w;
    rectMoyenne.h = surMoyenne->h;
    SDL_RenderCopy(fiche->render, texMoyenne, NULL, &rectMoyenne);
    SDL_FreeSurface(surMoyenne);
    SDL_DestroyTexture(texMoyenne);
    /*Moyenne Trimestrielle*/
    TTF_SetFontStyle(bilanFont, TTF_STYLE_UNDERLINE);
    TTF_SetFontStyle(bilanFont, TTF_STYLE_BOLD);
    surTitre = TTF_RenderText_Blended(bilanFont, "Moyenne Trimestrielle :", noire);
    texTitre = SDL_CreateTextureFromSurface(fiche->render, surTitre);
    rectTitre.x = BILAN_X + (2*BILAN_TITRE_DECALAGE_X);
    rectTitre.y = rectTitre.y + rectTitre.h + BILAN_TITRE_DECALAGE_Y;
    rectTitre.w = surTitre->w;
    rectTitre.h = surTitre->h;
    SDL_RenderCopy(fiche->render, texTitre, NULL, &rectTitre);
    SDL_FreeSurface(surTitre);
    SDL_DestroyTexture(texTitre);

    TTF_SetFontStyle(bilanFont, TTF_STYLE_NORMAL);
    rectTitre.x = BILAN_X+BILAN_TITRE_DECALAGE_X;
    /*Moyenne /20*/
    TTF_SetFontStyle(bilanFont, TTF_STYLE_UNDERLINE);
    surTitre = TTF_RenderText_Blended(bilanFont, "Moyenne Coefficié :", noire);
    texTitre = SDL_CreateTextureFromSurface(fiche->render, surTitre);
    rectTitre.y = rectTitre.y + rectTitre.h + BILAN_TITRE_DECALAGE_Y;
    rectTitre.w = surTitre->w;
    rectTitre.h = surTitre->h;
    SDL_RenderCopy(fiche->render, texTitre, NULL, &rectTitre);
    SDL_FreeSurface(surTitre);
    SDL_DestroyTexture(texTitre);
    TTF_SetFontStyle(bilanFont, TTF_STYLE_NORMAL);
    sprintf(strMoyenne, "%.2f", fiche->list_matiere->bilan.moyCoef);
    surMoyenne = TTF_RenderText_Blended(bilanFont, strMoyenne, noire);
    texMoyenne = SDL_CreateTextureFromSurface(fiche->render, surMoyenne);
    rectMoyenne.x = (rectTitre.x + rectTitre.w) + BILAN_MOYENNE_DECALAGE_X;
    rectMoyenne.y = rectTitre.y;
    rectMoyenne.w = surMoyenne->w;
    rectMoyenne.h = surMoyenne->h;
    SDL_RenderCopy(fiche->render, texMoyenne, NULL, &rectMoyenne);
    SDL_FreeSurface(surMoyenne);
    SDL_DestroyTexture(texMoyenne);
    /*Moyenne Coefficie*/
    TTF_SetFontStyle(bilanFont, TTF_STYLE_UNDERLINE);
    surTitre = TTF_RenderText_Blended(bilanFont, "Moyenne /20 :", noire);
    texTitre = SDL_CreateTextureFromSurface(fiche->render, surTitre);
    rectTitre.y = rectTitre.y + rectTitre.h + BILAN_TITRE_DECALAGE_Y;
    rectTitre.w = surTitre->w;
    rectTitre.h = surTitre->h;
    SDL_RenderCopy(fiche->render, texTitre, NULL, &rectTitre);
    SDL_FreeSurface(surTitre);
    SDL_DestroyTexture(texTitre);
    TTF_SetFontStyle(bilanFont, TTF_STYLE_NORMAL);
    sprintf(strMoyenne, "%.2f", fiche->list_matiere->bilan.moySur20);
    surMoyenne = TTF_RenderText_Blended(bilanFont, strMoyenne, noire);
    texMoyenne = SDL_CreateTextureFromSurface(fiche->render, surMoyenne);
    rectMoyenne.x = (rectTitre.x + rectTitre.w) + BILAN_MOYENNE_DECALAGE_X;
    rectMoyenne.y = rectTitre.y;
    rectMoyenne.w = surMoyenne->w;
    rectMoyenne.h = surMoyenne->h;
    SDL_RenderCopy(fiche->render, texMoyenne, NULL, &rectMoyenne);
    SDL_FreeSurface(surMoyenne);
    SDL_DestroyTexture(texMoyenne);
    /*Mention*/
    TTF_SetFontStyle(bilanFont, TTF_STYLE_UNDERLINE);
    surTitre = TTF_RenderText_Blended(bilanFont, "Mention :", noire);
    texTitre = SDL_CreateTextureFromSurface(fiche->render, surTitre);
    rectTitre.y = rectTitre.y + rectTitre.h + BILAN_TITRE_DECALAGE_Y;
    rectTitre.w = surTitre->w;
    rectTitre.h = surTitre->h;
    SDL_RenderCopy(fiche->render, texTitre, NULL, &rectTitre);
    SDL_FreeSurface(surTitre);
    SDL_DestroyTexture(texTitre);
    TTF_SetFontStyle(bilanFont, TTF_STYLE_NORMAL);
    surMoyenne = TTF_RenderText_Blended(bilanFont, fiche->list_matiere->bilan.mention, noire);
    texMoyenne = SDL_CreateTextureFromSurface(fiche->render, surMoyenne);
    rectMoyenne.x = (rectTitre.x + rectTitre.w) + BILAN_MOYENNE_DECALAGE_X;
    rectMoyenne.y = rectTitre.y;
    rectMoyenne.w = surMoyenne->w;
    rectMoyenne.h = surMoyenne->h;
    SDL_RenderCopy(fiche->render, texMoyenne, NULL, &rectMoyenne);
    SDL_FreeSurface(surMoyenne);
    SDL_DestroyTexture(texMoyenne);
    TTF_CloseFont(bilanFont);
}
