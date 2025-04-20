#include "../inc/renderer.h"
#include "../inc/evenement.h"

void renderOutils(BoiteOutils* outils, SDL_Renderer* render, SDL_Event* event)
{
    SET_GREY(render);
    SDL_RenderFillRect(render, &outils->rectangle);
    SET_INDIGO(render);
    SDL_RenderDrawRect(render, &outils->rectangle);
    if(BTN_RenderMouseMotion(outils->addMatiere, *event, render)) {}
    if(BTN_RenderMouseMotion(outils->addFiche, *event, render)) {}
    if(BTN_RenderMouseMotion(outils->suppFiche, *event, render)) {}
    if(BTN_RenderMouseMotion(outils->suppMatiere, *event, render)) {}
    if(BTN_RenderMouseMotion(outils->chargeFiche, *event, render)) {}
    if(BTN_RenderMouseMotion(outils->saveFiche, *event, render)) {}
    if(BTN_RenderMouseMotion(outils->klculBilan, *event, render)) {}
}

void renderFiche(Fiche* fiche, SDL_Event* event, SDL_Renderer* render)
{
    SDL_Event ficheEvent;
    GetFicheEvent(*event, &ficheEvent);
    if(fiche != NULL)
    {
        if(event->motion.x > FICHE_X && event->motion.y > FICHE_Y) //Maintenant les éléments  de la fiche
        {
            SDL_SetRenderTarget(render, fiche->texture);
            GetFicheEvent(*event, &ficheEvent);
            if(BTN_RenderMouseMotion(fiche->bouton_plus, ficheEvent, render)) {}
            SDL_SetRenderTarget(render, NULL);
            SDL_RenderCopy(fiche->render, fiche->texture, NULL, &fiche->surface_rect);
        }
    }
}

void renderFicheVolet(Liste_f* listFiche, SDL_Event* event, SDL_Renderer* render)
{
    if(listFiche->addrFiche[0] != NULL)
    {
        if(mouseMoveInVolet(listFiche->addrFiche[0], *event)) {}
        if(BTN_RenderMouseMotion(listFiche->addrFiche[0]->volet_titre->supp, *event, render)) {}
    }
    if(listFiche->addrFiche[1] != NULL)
    {
        if(mouseMoveInVolet(listFiche->addrFiche[1], *event)) {}
        if(BTN_RenderMouseMotion(listFiche->addrFiche[1]->volet_titre->supp, *event, render)) {}
    }
    if(listFiche->addrFiche[2] != NULL)
    {
        if(mouseMoveInVolet(listFiche->addrFiche[2], *event)) {}
        if(BTN_RenderMouseMotion(listFiche->addrFiche[2]->volet_titre->supp, *event, render)) {}
    }
    if(listFiche->addrFiche[3] != NULL)
    {
        if(mouseMoveInVolet(listFiche->addrFiche[3], *event)) {}
        if(BTN_RenderMouseMotion(listFiche->addrFiche[3]->volet_titre->supp, *event, render)) {}
    }
    if(listFiche->addrFiche[4] != NULL)
    {
        if(mouseMoveInVolet(listFiche->addrFiche[4], *event)) {}
        if(BTN_RenderMouseMotion(listFiche->addrFiche[4]->volet_titre->supp, *event, render)) {}
    }
}

void renderCredits(SDL_Renderer* render, TTF_Font* font)
{
    SDL_Rect userRect = {0, 0, LARGEUR_UTILISATEUR, OUTILS_Y-UL_Y};
    SET_INDIGO(render);
    SDL_RenderFillRect(render, &userRect);
    char *userName = "Les Collèges Jean Piaget";
    SDL_Surface *userSurface = TTF_RenderText_Blended(font, userName, noire);
    userRect.x = (LARGEUR_UTILISATEUR/2)-(userSurface->w);
    userRect.y = (userRect.h/2)-(userSurface->h/2);
    userRect.w = userSurface->w;
    SDL_Texture *userTexture = SDL_CreateTextureFromSurface(render, userSurface);
    SDL_RenderCopy(render, userTexture, NULL, &userRect);
    SDL_FreeSurface(userSurface);
    SDL_DestroyTexture(userTexture);
    SDL_Rect editorRect = {0, FICHE_Y+FICHE_H, LARGEUR_UTILISATEUR, HAUTEUR_UTILISATEUR-(FICHE_Y+FICHE_H)};
    SDL_SetRenderDrawColor(render, 255, 255, 0, 255);
    SDL_RenderFillRect(render, &editorRect);
    SDL_Surface *editorSurface = TTF_RenderText_Blended(font, "Edité par TOKO Rayane", noire);
    editorRect.w = editorSurface->w;
    SDL_Texture *editorTexture = SDL_CreateTextureFromSurface(render, editorSurface);
    SDL_RenderCopy(render, editorTexture, NULL, &editorRect);
    SDL_FreeSurface(editorSurface);
    editorSurface = TTF_RenderText_Blended(font, "Contact : tkryan03@gmail.com", noire);
    editorTexture = SDL_CreateTextureFromSurface(render, editorSurface);
    editorRect.w = editorSurface->w;
    editorRect.x = LARGEUR_UTILISATEUR - editorSurface->w-UL_X;
    SDL_RenderCopy(render, editorTexture, NULL, &editorRect);
    SDL_FreeSurface(editorSurface);
    SDL_DestroyTexture(editorTexture);
}

