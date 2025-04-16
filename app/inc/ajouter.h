#ifndef AJOUTER_H_INCLUDED
#define AJOUTER_H_INCLUDED

#include "variables.h"

Fiche* AjouterFiche(Liste_f *fiches, SDL_Renderer *render);
Return_Bool AjouterMatiere(Fiche *fiche);
Return_Bool AjouterNote(Matiere *matiere, double note, int position, SDL_Renderer *render);
Return_Bool AjouterMoy(Matiere *matiere, double moyenne, int position);
Return_Bool TracerGraphe(Fiche *fiche, double note, short indice);
Return_Bool RetracerGraphe(Fiche *fiche, short indiceMatiere);

#endif // AJOUTER_H_INCLUDED
