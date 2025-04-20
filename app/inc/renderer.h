#ifndef RENDERER_H_INCLUDED
#define RENDERER_H_INCLUDED

#include "variables.h"
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_ttf.h>

void renderOutils(BoiteOutils* outils, SDL_Renderer* render, SDL_Event* event);
void renderFiche(Fiche* fiche, SDL_Event* event, SDL_Renderer* render);
void renderFicheVolet(Liste_f* listFiche, SDL_Event* event, SDL_Renderer* render);
void renderCredits(SDL_Renderer* render, TTF_Font* font);

#endif // RENDERER_H_INCLUDED

