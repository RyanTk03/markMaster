#include <stdio.h>
#include <stdlib.h>

#include "../inc/constantes.h"
#include "../inc/variables.h"

Fiche *CreerFiche()//Fini(sauf le volet)
{
    Fiche *fiche = malloc(sizeof(*fiche));

    if(fiche == NULL)
    {
        fprintf(stderr,"Impossible de creer une nouvelle fiche");
        return NULL;
    }
    fiche->nom = NULL;
    {
        //la surface où se trouve la fiche
        fiche->surface_rect.x = FICHE_X;
        fiche->surface_rect.y = FICHE_Y;
        fiche->surface_rect.w = FICHE_W;
        fiche->surface_rect.h = FICHE_H;
    }

    {
        //le curseur principale de la fiche
        fiche->curseur.w = CURSEUR_W;
        fiche->curseur.h = CURSEUR_H;
        fiche->curseur.x = DEBUT_NOTE_X + DECAL_CURSEUR_X;
        fiche->curseur.y = LIGNE_DEPART_Y + (LIGNE_H/2 - fiche->curseur.h/2);
    }

    {
        //la liste des  matières
        fiche->list_matiere = CreerListeMatiere();
    }

    {
        //le bilan
        fiche->list_matiere->bilan.mention = NULL;
        fiche->list_matiere->bilan.moyCoef = 0.0;
        fiche->list_matiere->bilan.moyDev = 0.0;
        fiche->list_matiere->bilan.moyInt = 0.0;
        fiche->list_matiere->bilan.moySur20 = 0.0;
        fiche->list_matiere->bilan.totaux_coefficients = 0.0;
    }

    {
        //La liste de point
        fiche->graphe = CreerGraphe();
    }

    {
        //bouton plus
        fiche->bouton_plus = BTN_CreateButtonWithTexture(BTN_DYNAMIC_BUTTON_FORM_B, BOUTON_PLUS_W, BOUTON_PLUS_H,
                             BOUTON_PLUS_X, BOUTON_PLUS_Y(fiche), "+", "appData/fonts/taile.ttf");
    }
    fiche->statut = actuelle;
    return fiche;
}

Fiche* DetruireFiche(Liste_f *fiches, Fiche *fiche)
{
    if(fiche == NULL)
        return NULL;
    int i = 0;
    while(fiches->addrFiche[i] != fiche)
        i++;
    fiches->addrFiche[i] = NULL;
    Fiche *ficheActuelle = NULL;
    if(i+1 < fiches->n)
        ficheActuelle = fiches->addrFiche[i+1];
    else if(i-1 >= 0)
        ficheActuelle = fiches->addrFiche[i-1];
    if(ficheActuelle != NULL)
        ficheActuelle->statut = actuelle;
    while(i+1 < fiches->n)
    {
        fiches->addrFiche[i] = fiches->addrFiche[i+1];
        fiches->addrFiche[i+1] = NULL;
        if(fiches->addrFiche[i]->statut == actuelle)
            SDL_RenderCopy(fiches->addrFiche[i]->render, fiches->addrFiche[i]->volet_titre->textureActu, NULL, fiches->addrFiche[i]->volet_titre->rectangle);
        else
            SDL_RenderCopy(fiches->addrFiche[i]->render, fiches->addrFiche[i]->volet_titre->texture, NULL, fiches->addrFiche[i]->volet_titre->rectangle);
        i++;
    }
    fiches->n--;
    BTN_FreeButton(fiche->bouton_plus);
    SupprimerVolet(fiches, fiche);
    LibererGraphe(fiche->graphe);
    SupprimerListeMatiere(fiche->list_matiere, fiche);
    free(fiche->nom);
    SDL_DestroyTexture(fiche->texture);
    SET_GREY(fiche->render);
    SDL_RenderFillRect(fiche->render, &fiche->surface_rect);
    free(fiche);
    if(ficheActuelle != NULL)
        SDL_RenderCopy(ficheActuelle->render, ficheActuelle->texture, NULL, &ficheActuelle->surface_rect);
    return ficheActuelle;
}

Liste_f* CreerListeFiche()//Fini
{
    Liste_f *listFiches = malloc(sizeof(*listFiches));
    int i = 0;

    if(listFiches == NULL)
    {
        fprintf(stderr, "Imposible de creer la liste de fiche.");
        return NULL;
    }
    while(i < MAX_FICHE)
    {
        listFiches->addrFiche[i] = NULL;
        i++;
    }
    listFiches->n = 0;
    return listFiches;
}

void SupprimerListeFiche(Liste_f *fiches)
{
    for(int i = 0; i < fiches->n; i++)
        DetruireFiche(fiches, fiches->addrFiche[i]);
    free(fiches);
}

Matiere* CreerMatiere(Fiche *fiche, char *nom, double coefficient)//Fini
{
    if(coefficient == 0 || nom == NULL)
    {
        if(coefficient == 0)
            fprintf(stderr, "le coefficient envoye a la fonction CreerMatiere() null\n");
        else
            fprintf(stderr, "le nom envoye a la fonction CreerMatiere() n'existe pas en memoire\n");
        return NULL;
    }
    Matiere *newMatiere = malloc(sizeof(*newMatiere));
    SDL_Texture *texture = SDL_CreateTexture(fiche->render, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, LIGNE_W, LIGNE_H);
    newMatiere->nom = nom;
    newMatiere->devoirs = InititialiserNotes(devoir);
    newMatiere->interrogations = InititialiserNotes(interrogation);
    newMatiere->coefficient = coefficient;
    newMatiere->moy_int = -1.0;
    newMatiere->moy_dev = -1.0;
    newMatiere->moy_totale = -1.0;
    newMatiere->moy_coeff = -1.0;
    newMatiere->indice = fiche->list_matiere->n-1;
    newMatiere->ligne = texture;
    newMatiere->appreciation = NULL;

    return newMatiere;
}

void SupprimerMatiere(Matiere *matiere, Fiche *fiche)
{
    int i = 0;
    while(fiche->list_matiere->addrMatiere[i] != matiere)
        i++;
    SDL_Rect r = {LIGNE_X, LIGNE_DEPART_Y+(i*LIGNE_H), LIGNE_W, LIGNE_H};
    SDL_SetRenderTarget(fiche->render, fiche->texture);
    SET_JPBLUE(fiche->render);
    SDL_RenderFillRect(fiche->render, &r);
    SDL_SetRenderTarget(fiche->render, NULL);
    SDL_RenderCopy(fiche->render, fiche->texture, NULL, &fiche->surface_rect);
    free(matiere->nom);
    free(matiere->interrogations);
    free(matiere->devoirs);
    SDL_DestroyTexture(matiere->ligne);
    free(matiere);
}

Liste_m* CreerListeMatiere()//Fini
{
    Liste_m *matieres = malloc(sizeof(*matieres));

    if(matieres == NULL)
    {
        fprintf(stderr, "Impossible de creer la liste de matiere");
        exit(EXIT_FAILURE);
    }
    int i = 0;
    while(i < MAX_MATIERE)
    {
        matieres->addrMatiere[i] = NULL;
        i++;
    }
    matieres->n = 0;

    return matieres;
}

void SupprimerListeMatiere(Liste_m *matieres, Fiche *fiche)
{
    for(int i = 0; i < matieres->n; i++)
    {
        SupprimerMatiere(matieres->addrMatiere[i], fiche);
        matieres->n--;
    }
    free(matieres);
}

Volet *Creer_AfficherVolet(Liste_f *fiches)//il reste à régler le nom
{
    TTF_Font *font = TTF_OpenFont("appData/fonts/taile.ttf", 15);
    Volet *volet = malloc(sizeof(*volet));

    if(volet == NULL)
    {
        fprintf(stderr, "Impossible de creer le volet de navigation");
        return NULL;
    }
    volet->texture = SDL_CreateTexture(fiches->addrFiche[fiches->n]->render, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET,
                                       VOLET_W-SUPP_FICHE_W, VOLET_H);
    volet->textureActu = SDL_CreateTexture(fiches->addrFiche[fiches->n]->render, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET,
                                           VOLET_W-SUPP_FICHE_W, VOLET_H);
    SDL_SetTextureBlendMode(volet->texture, SDL_BLENDMODE_BLEND);
    SDL_SetTextureBlendMode(volet->textureActu, SDL_BLENDMODE_BLEND);
    volet->rectangle = SDL_malloc(sizeof(*volet->rectangle));
    volet->rectangle->x = NAVIG_VOLET_X+(fiches->n*VOLET_W);
    volet->rectangle->y = NAVIG_VOLET_Y;
    volet->rectangle->w = VOLET_W-SUPP_FICHE_W;
    volet->rectangle->h = VOLET_H;
    //La texture du volet lorsque la fiche est en arrière-plan
    SDL_SetRenderTarget(fiches->addrFiche[fiches->n]->render, volet->texture);
    SET_GREY(fiches->addrFiche[fiches->n]->render);
    SDL_RenderFillRect(fiches->addrFiche[fiches->n]->render, NULL);
    SET_INDIGO(fiches->addrFiche[fiches->n]->render);
    SDL_RenderDrawRect(fiches->addrFiche[fiches->n]->render, NULL);
    SDL_Surface *s = TTF_RenderText_Blended(font, fiches->addrFiche[fiches->n]->nom, noire);
    SDL_Texture *t = SDL_CreateTextureFromSurface(fiches->addrFiche[fiches->n]->render, s);
    SDL_Rect r;
    r.w = s->w;
    r.h = s->h;
    r.x = (VOLET_W/2)-(r.w/2);
    r.y = (VOLET_H/2)-(r.h/2);
    SDL_RenderCopy(fiches->addrFiche[fiches->n]->render, t, NULL, &r);
    SDL_FreeSurface(s);
    SDL_DestroyTexture(t);
    //Fin
    SDL_SetRenderTarget(fiches->addrFiche[fiches->n]->render, NULL);
    //La texture du volet lorsque la fiche est en avant-plan
    SDL_SetRenderTarget(fiches->addrFiche[fiches->n]->render, volet->textureActu);
    SET_JPBLUE(fiches->addrFiche[fiches->n]->render);
    SDL_RenderFillRect(fiches->addrFiche[fiches->n]->render, NULL);
    SET_INDIGO(fiches->addrFiche[fiches->n]->render);
    SDL_RenderDrawRect(fiches->addrFiche[fiches->n]->render, NULL);
    s = TTF_RenderText_Blended(font, fiches->addrFiche[fiches->n]->nom, noire);
    t = SDL_CreateTextureFromSurface(fiches->addrFiche[fiches->n]->render, s);
    SDL_RenderCopy(fiches->addrFiche[fiches->n]->render, t, NULL, &r);
    SDL_FreeSurface(s);
    SDL_DestroyTexture(t);
    //Fin
    SDL_SetRenderTarget(fiches->addrFiche[fiches->n]->render, NULL);
    volet->supp = BTN_CreateButtonWithTexture(BTN_DYNAMIC_BUTTON_FORM_A, SUPP_FICHE_W, SUPP_FICHE_H,
                  NAVIG_VOLET_X+(VOLET_W+(fiches->n*VOLET_W))-SUPP_FICHE_W, NAVIG_VOLET_Y, "x", "appData/fonts/taile.ttf");
    BTN_ChangeEffectColor(volet->supp, 200, 0, 0, 80, fiches->addrFiche[fiches->n]->render);
    BTN_CopyButton(volet->supp, fiches->addrFiche[fiches->n]->render);
    SDL_RenderCopy(fiches->addrFiche[fiches->n]->render, volet->texture, NULL, volet->rectangle);
    for(int i = 0; i < 0; i++)
        fiches->addrFiche[i]->statut = arriere_plan;
    fiches->addrFiche[fiches->n]->statut = actuelle;
    TTF_CloseFont(font);
    return volet;
}

Return_Bool mouseMoveInVolet(Fiche *fiche, SDL_Event event)
{
    if(BTN_MOUSE_MOVE_IN_RECT(fiche->volet_titre->rectangle, event))
    {
        if(fiche->statut == actuelle)
            SDL_RenderCopy(fiche->render, fiche->volet_titre->textureActu, NULL, fiche->volet_titre->rectangle);
        else
            SDL_RenderCopy(fiche->render, fiche->volet_titre->texture, NULL, fiche->volet_titre->rectangle);
        SDL_Surface *s = SDL_CreateRGBSurface(SDL_SWSURFACE, fiche->volet_titre->rectangle->w, fiche->volet_titre->rectangle->h
                                              , 32, 0, 0, 0, 0);
        SDL_SetSurfaceBlendMode(s, SDL_BLENDMODE_BLEND);
        SDL_FillRect(s, NULL, SDL_MapRGB(s->format, 0, 0, 255));
        SDL_SetSurfaceAlphaMod(s, 80);
        SDL_Texture *t = SDL_CreateTextureFromSurface(fiche->render, s);
        SDL_RenderCopy(fiche->render, t, NULL, fiche->volet_titre->rectangle);
        // SDL_RenderPresent(fiche->render);
        SDL_FreeSurface(s);
        SDL_DestroyTexture(t);
        return ok;
    }
    else
    {
        if(fiche->statut == actuelle)
            SDL_RenderCopy(fiche->render, fiche->volet_titre->textureActu, NULL, fiche->volet_titre->rectangle);
        else
            SDL_RenderCopy(fiche->render, fiche->volet_titre->texture, NULL, fiche->volet_titre->rectangle);
        // SDL_RenderPresent(fiche->render);
        return error;
    }
}

void SupprimerVolet(Liste_f *fiches, Fiche *fiche)
{
    SET_GREY(fiche->render);
    SDL_RenderFillRect(fiche->render, fiche->volet_titre->rectangle);
    SDL_RenderFillRect(fiche->render, fiche->volet_titre->supp->rect);
    SDL_free(fiche->volet_titre->rectangle);
    BTN_FreeButton(fiche->volet_titre->supp);
    int i = 0;
    for(i = 0; i < fiches->n; i++)
    {
        SDL_RenderFillRect(fiches->addrFiche[i]->render, fiches->addrFiche[i]->volet_titre->rectangle);
        SDL_RenderFillRect(fiches->addrFiche[i]->render, fiches->addrFiche[i]->volet_titre->supp->rect);
        fiches->addrFiche[i]->volet_titre->rectangle->x = NAVIG_VOLET_X+(i*VOLET_W);
        BTN_ChangeButtonRect(fiches->addrFiche[i]->render, NULL, fiches->addrFiche[i]->volet_titre->supp, NAVIG_VOLET_X+(VOLET_W+(i*VOLET_W))-SUPP_FICHE_W, -1, -1, -1);
    }
}

Liste_n *InititialiserNotes(TypeNote typeNote)//Fini
{
    Liste_n *liste = malloc(sizeof(*liste));
    if(liste == NULL)
    {
        fprintf(stderr, "Une erreur s'est produite lors de l'initialisation des notes");
        return NULL;
    }
    liste->n = 0;
    liste->somme = 0;
    liste->type = typeNote;
    if(typeNote == devoir)
        liste->max_note = 2;
    else if(typeNote == interrogation)
        liste->max_note = 6;
    liste->notes = malloc(liste->max_note * sizeof(double));
    if(liste->notes == NULL)
        return NULL;
    for(int i = 0; i < liste->max_note; i++)
        liste->notes[i] = -1;
    return liste;
}

BoiteOutils *CreerOutils()
{
    BoiteOutils *outils = malloc(sizeof(*outils));
    if(outils == NULL)
    {
        fprintf(stderr, "Impossible de créer la barre des outils");
        return NULL;
    }
    outils->addFiche = BTN_CreateButtonWithTexture(BTN_STATIC_BUTTON, LITLE_BUTTON_W, LITLE_BUTTON_H, OUTILS_X, OUTILS_Y, "+Fiche", "appData/fonts/taile.ttf");
    outils->addMatiere = BTN_CreateButtonWithTexture(BTN_STATIC_BUTTON, LITLE_BUTTON_W, LITLE_BUTTON_H, OUTILS_X, OUTILS_Y+(OUTILS_H/2), "+Matiere", "appData/fonts/taile.ttf");
    outils->suppFiche = BTN_CreateButtonWithTexture(BTN_STATIC_BUTTON, LITLE_BUTTON_W, LITLE_BUTTON_H, OUTILS_X+LITLE_BUTTON_W, OUTILS_Y, "-Fiche", "appData/fonts/taile.ttf");
    outils->suppMatiere = BTN_CreateButtonWithTexture(BTN_STATIC_BUTTON, LITLE_BUTTON_W, LITLE_BUTTON_H, OUTILS_X+LITLE_BUTTON_W, OUTILS_Y+(OUTILS_H/2), "-Matiere", "appData/fonts/taile.ttf");
    outils->chargeFiche = BTN_CreateButtonWithTexture(BTN_STATIC_BUTTON, SMALL_BUTTON_W, SMALL_BUTTON_H, OUTILS_X+(2*LITLE_BUTTON_W), OUTILS_Y, "ouvrir Fiche", "appData/fonts/taile.ttf");
    outils->saveFiche = BTN_CreateButtonWithTexture(BTN_STATIC_BUTTON, SMALL_BUTTON_W, SMALL_BUTTON_H, OUTILS_X+(3*LITLE_BUTTON_W), OUTILS_Y, "sauver Fiche", "appData/fonts/taile.ttf");
    outils->klculBilan = BTN_CreateButtonWithTexture(BTN_STATIC_BUTTON, SMALL_BUTTON_W, SMALL_BUTTON_H, OUTILS_W-SMALL_BUTTON_W, OUTILS_Y, "Bilan", "appData/fonts/taile.ttf");
    outils->rectangle.x = OUTILS_X;
    outils->rectangle.y = OUTILS_Y;
    outils->rectangle.w = OUTILS_W;
    outils->rectangle.h = OUTILS_H;

    return outils;
}

void DetruireOutils(BoiteOutils *outils)
{
    BTN_FreeButton(outils->addFiche);
    BTN_FreeButton(outils->addMatiere);
    BTN_FreeButton(outils->suppFiche);
    BTN_FreeButton(outils->suppMatiere);
    BTN_FreeButton(outils->chargeFiche);
    BTN_FreeButton(outils->saveFiche);
    BTN_FreeButton(outils->klculBilan);
    free(outils);
}

void afficherOutils(SDL_Renderer *render, BoiteOutils *outils)
{
    SET_GREY(render);
    SDL_RenderFillRect(render, &outils->rectangle);
    SET_INDIGO(render);
    SDL_RenderDrawRect(render, &outils->rectangle);
    BTN_CopyButton(outils->addFiche, render);
    BTN_CopyButton(outils->addMatiere, render);
    BTN_CopyButton(outils->suppFiche, render);
    BTN_CopyButton(outils->suppMatiere, render);
    BTN_CopyButton(outils->chargeFiche, render);
    BTN_CopyButton(outils->saveFiche, render);
    BTN_CopyButton(outils->klculBilan, render);
    SDL_RenderPresent(render);
}

Graphe* CreerGraphe(void)
{
    Graphe *graphe = malloc(sizeof(*graphe));
    NotePoint *element = malloc(sizeof(*element));
    if(graphe == NULL || element == NULL)
    {
        fprintf(stderr, "Impossible de creer le graphe :: CreerGraphe()");
        return NULL;
    }
    element->indice = -1;
    element->note = -1.0;
    element->point.x = 0;
    element->point.y = 0;
    element->suivant = NULL;
    graphe->premiere = element;
    graphe->nbre_point = 0;
    for(int i = 0; i < MAX_NOTE; i++)
        graphe->indices[i] = -1;
    return graphe;
}

void LibererGraphe(Graphe *graphe)
{
    if(graphe == NULL)
        return;
    NotePoint *aSupprimer = graphe->premiere;
    for(short i = 0; i < graphe->nbre_point; i++)
    {
        graphe->premiere = aSupprimer->suivant;
        free(aSupprimer);
        aSupprimer = graphe->premiere;
    }
    free(graphe);
}

short CheckArray(short *tableau, int taille, short toCheck)
{
    for(int i = 0; i < taille; i++)
    {
        if(tableau[i] == toCheck)
            return i;
    }
    return -1;
}
