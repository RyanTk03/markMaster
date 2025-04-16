#ifndef CHARGE_H_INCLUDED
#define CHARGE_H_INCLUDED

#include "variables.h"

Fiche* charger_fiche(Liste_f *fiches, SDL_Renderer *render, char *fichier);
Return_Bool charger_donnes(char *fichier, Fiche *fiche);
Return_Bool charger_matiere(Fiche *fiche, char *nomMatiere, double coefficient);
void charger_note(Fiche *fiche, double note, short numero);

#endif // CHARGE_H_INCLUDED
