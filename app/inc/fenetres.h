#ifndef FENETRES_H_INCLUDED
#define FENETRES_H_INCLUDED

#define OK_BUTTON_W 40 * UL_X
#define OK_BUTTON_H 20 * UL_Y
#define OK_BUTTON_X NEW_SUBJECT_WINDOW_W - OK_BUTTON_W
#define OK_BUTTON_Y NEW_SUBJECT_WINDOW_H - OK_BUTTON_H

Matiere* fenetre_nouvelleMatiere(Fiche *fiche);
Fiche* fenetre_nouvelleFiche();
Fiche* fenetre_chargerFiche(Liste_f *fiches, SDL_Renderer *fRender);
void fenetre_supprimerMatiere(Fiche *fiche);

#endif // FENETRES_H_INCLUDED
