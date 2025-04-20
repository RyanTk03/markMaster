#ifndef EVENEMENT_H_INCLUDED
#define EVENEMENT_H_INCLUDED

#include "constantes.h"
#include "variables.h"

void main_looping(Liste_f *listFiche, BoiteOutils *outils, SDL_Window *window, SDL_Renderer *render, char **argv);
void GetFicheEvent(SDL_Event winEvent, SDL_Event *ficheEvent);

#endif // EVENEMENT_H_INCLUDED
