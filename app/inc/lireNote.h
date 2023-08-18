#ifndef LIRECARACTERE_H_INCLUDED
#define LIRECARACTERE_H_INCLUDED
#include "variables.h"

Return_Bool SaisirNote(Fiche *fiche, SDL_Event *ficheEvent);
void ChangerPositionCurseur(Fiche *fiche, int x, int y);
void AjusterPositionCurseur(Fiche *fiche, int i, int j);
Return_Bool TrouverPositionCurseur(Fiche *fiche, SDL_MouseButtonEvent *mouse, short *indMat, short *indNt);
Return_Bool EnregistrerNote(Fiche *fiche, double note, short indiceMatiere, short indiceNote);

#endif // LIRECARACTERE_H_INCLUDED
