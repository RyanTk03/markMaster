#include <stdio.h>
#include <stdlib.h>

#include "../inc/constantes.h"
#include "../inc/variables.h"
#include "../inc/ajouter.h"
#include "../inc/fenetres.h"

Fiche* AjouterFiche(Liste_f *fiches, SDL_Renderer *render)
{
    Fiche *fiche = fenetre_nouvelleFiche();
    SDL_Surface *tableau = IMG_Load("appData/IMGs/entete.jpg");
    SDL_Surface *fond = IMG_Load("appData/IMGs/fondIMG.jpg");

    SDL_Texture *texTableau = SDL_CreateTextureFromSurface(render, tableau);
    SDL_FreeSurface(tableau);
    SDL_Texture *texFond = SDL_CreateTextureFromSurface(render, fond);
    SDL_FreeSurface(fond);
    SDL_SetTextureBlendMode(texFond, SDL_BLENDMODE_BLEND);
    SDL_SetTextureAlphaMod(texFond, 20);

    SDL_Rect position;

    if(fiches->n < MAX_FICHE && fiche != NULL)
    {
        char *nom = malloc((strlen(fiche->nom)+5)*sizeof(char));
        strcpy(nom, fiche->nom);
        strcat(nom, ".jpf");
        free(fiche->nom);
        fiche->nom = nom;
        fiche->render = render;
        fiche->texture = SDL_CreateTexture(fiche->render, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, FICHE_W, FICHE_H);
        SDL_SetRenderTarget(fiche->render, fiche->texture);
        {
            //Remplir le rectangle de la fiche
            SET_JPBLUE(fiche->render);
            SDL_RenderFillRect(fiche->render, NULL);
            SET_INDIGO(fiche->render);
            SDL_RenderDrawRect(fiche->render, NULL);
        }
        {
            //l'image de fond:
            position.x = FICHE_W/2-LARGEUR_FOND/2;
            position.y = FICHE_H/2-HAUTEUR_FOND/2;
            position.w = LARGEUR_FOND;
            position.h = HAUTEUR_FOND;
            SDL_RenderCopy(fiche->render, texFond, NULL, &position);
        }
        {
            //la tête du tableau:
            position.x = TABLEAU_TETE_X;
            position.y = TABLEAU_TETE_Y;
            position.w = TABLEAU_TETE_W;
            position.h = TABLEAU_TETE_H;
            SDL_RenderCopy(fiche->render, texTableau, NULL, &position);
        }
        {
            //le bouton plus
            BTN_CopyButton(fiche->bouton_plus, fiche->render);
        }
        SDL_Rect graphe = {GRAPHE_X, GRAPHE_Y, GRAPHE_W, GRAPHE_H};
        SDL_RenderDrawRect(fiche->render, &graphe);
        SDL_SetRenderTarget(fiche->render, NULL);
        fiches->addrFiche[fiches->n] = fiche;
        fiche->volet_titre = Creer_AfficherVolet(fiches);
        printf("apres bouton plus\n");
        fiches->n++;
        SDL_DestroyTexture(texFond);
        SDL_DestroyTexture(texTableau);
        SDL_RenderCopy(fiche->render, fiche->texture, NULL, &fiche->surface_rect);
        SDL_RenderPresent(fiche->render);
        fprintf(stdout, "La fiche a ete creer :: AjouterFiche()\n");
        return fiche;
    }
    else
    {
        SDL_DestroyTexture(texFond);
        SDL_DestroyTexture(texTableau);
        if(fiche != NULL)
            fprintf(stdout, "La fiche creer est NULL :: AjouterFiche()\n");
        else
            fprintf(stdout, "Le nombre limite de fiche a ete atteint :: AjouterFiche()\n");
        return NULL;
    }
}

Return_Bool AjouterMatiere(Fiche *fiche)//Achevé
{
    if(fiche->list_matiere->n >= MAX_MATIERE)
    {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, NULL, "Le nombre limite de matiere a ete atteint.", NULL);
        return error;
    }
    Matiere *matiere = fenetre_nouvelleMatiere(fiche);
    if(fiche == NULL || matiere == NULL)
    {
        if(fiche == NULL)
            SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, NULL, "Vous devez d'abord ajouter une fiche.", NULL);
        else
            fprintf(stdout, "Impossible d'ajouter la matiere n°%d", fiche->list_matiere->n);
        return error;
    }
    else
    {
        SDL_Surface *ligne = IMG_Load("appData/IMGs/ligne.jpg");
        SDL_Texture *texLigneImg = SDL_CreateTextureFromSurface(fiche->render, ligne);
        SDL_FreeSurface(ligne);
        SDL_Surface *surName = TTF_RenderText_Blended(myFont, matiere->nom, noire);
        SDL_Texture *texName = SDL_CreateTextureFromSurface(fiche->render, surName);
        SDL_Rect rectName = {(MATIERE_W/2)-(surName->w/2),(MATIERE_H/2)-(surName->h/2), surName->w, surName->h};
        SDL_FreeSurface(surName);
        if(rectName.w > MATIERE_W)
            rectName.w = MATIERE_W;
        if(rectName.h > MATIERE_H)
            rectName.h = MATIERE_H;
        SDL_SetRenderTarget(fiche->render, matiere->ligne);
        SDL_RenderCopy(fiche->render, texLigneImg, NULL, NULL);
        printf("Entre dans la copie du nom de la matiere (fonction AjouterMatiere())\n");
        //il reste a copier le nom dans la case correspondant dans la ligne.
        SDL_RenderCopy(fiche->render, texName, NULL, &rectName);
        SDL_DestroyTexture(texName);
        char *coefficient = malloc(5*sizeof(char));
        sprintf(coefficient, "%.1f", matiere->coefficient);
        surName = TTF_RenderText_Blended(myFont, coefficient, noire);
        texName = SDL_CreateTextureFromSurface(fiche->render, surName);
        rectName.x = COEFFICIENT_X + (COEFFICIENT_W/2)-(surName->w/2);
        rectName.w = surName->w;
        rectName.y = (NEW_SUBJECT_COEFF_H/2)-(surName->h/2);
        rectName.h = surName->h;
        if(rectName.w > COEFFICIENT_W)
            rectName.w = COEFFICIENT_W;
        if(rectName.h > COEFFICIENT_H)
            rectName.h = COEFFICIENT_H;
        SDL_RenderCopy(fiche->render, texName, NULL, &rectName);
        SDL_SetRenderTarget(fiche->render, NULL);
        SDL_SetRenderTarget(fiche->render, fiche->texture);//Maintenant, on dessine sur la fiche
        //D'abord, il faut supprimer l'ancien bouton plus et effacer ses traces
        SET_JPBLUE(fiche->render);
        SDL_RenderFillRect(fiche->render, fiche->bouton_plus->rect);
        ACTUALIZE_RECT_LINE(fiche);
        SDL_RenderCopy(fiche->render, matiere->ligne, NULL, &ligneActuelle);
        //maintenant que la ligne à été copiée, on peut mettre à jour la liste de matière de la fiche
        fiche->list_matiere->addrMatiere[fiche->list_matiere->n] = matiere;
        fiche->list_matiere->n++;
        fiche->bouton_plus->rect->y = BOUTON_PLUS_Y(fiche);
        BTN_CopyButton(fiche->bouton_plus, fiche->render);
        SDL_SetRenderTarget(fiche->render, NULL);
        SDL_RenderCopy(fiche->render, fiche->texture, NULL, &fiche->surface_rect);
        SDL_RenderPresent(fiche->render);
        SDL_DestroyTexture(texName);
        SDL_FreeSurface(surName);
        SDL_DestroyTexture(texLigneImg);
        matiere->indice = fiche->list_matiere->n;
        return ok;
    }
}

Return_Bool AjouterNote(Matiere *matiere, double note, int position, SDL_Renderer *render)//Achevé
{
    if(position < 6)
    {
        if(matiere->interrogations->notes[position] >= 0 && note < 0)
        {
            matiere->interrogations->n--;
            matiere->interrogations->somme -= matiere->interrogations->notes[position];
            matiere->interrogations->notes[position] = -1.0;
        }
        else if(matiere->interrogations->notes[position] < 0 && note < 0){}
        else
        {
            if(matiere->interrogations->notes[position] < 0.0)
                matiere->interrogations->n++;
            else if(matiere->interrogations->somme > 0.0)
                matiere->interrogations->somme -= matiere->interrogations->notes[position];
            matiere->interrogations->notes[position] = note;
            matiere->interrogations->somme += note;
        }
    }
    else if(position == 7 || position == 8)
    {
        if(position == 7)
        {
            if(matiere->devoirs->notes[0] >= 0 && note < 0)
            {
                matiere->devoirs->n--;
                matiere->devoirs->somme -= matiere->devoirs->notes[0];
                matiere->devoirs->notes[position] = -1.0;
            }
            else if(matiere->devoirs->notes[0] < 0 && note < 0){}
            else
            {
                if(matiere->devoirs->notes[0] < 0.0)
                    matiere->devoirs++;
                else if(matiere->devoirs->somme > 0)
                    matiere->devoirs->somme -= matiere->devoirs->notes[0];
                matiere->devoirs->notes[0] = note;
                matiere->devoirs->somme += note;
            }
        }
        else if(position == 8)
        {
            if(matiere->devoirs->notes[1] >= 0 && note < 0)
            {
                matiere->devoirs->n--;
                matiere->devoirs->somme -= matiere->devoirs->notes[1];
                matiere->devoirs->notes[1] = -1.0;
            }
            else if(matiere->devoirs->notes[1] < 0 && note < 0){}
            else
            {
                if(matiere->devoirs->notes[1] < 0.0)
                    matiere->devoirs++;
                else if(matiere->devoirs->somme > 0.0)
                    matiere->devoirs->somme -= matiere->devoirs->notes[1];
                matiere->devoirs->notes[1] = note;
                matiere->devoirs->somme += note;
            }
        }
    }
    else
    {
        fprintf(stdout, "Ajout d'une note d'interrogation a une position non-reconnue.");
        return error;
    }
    return ok;
}

Return_Bool AjouterMoy(Matiere *matiere, double moyenne, int position)// Achevé
{
    if(position == 6)
        matiere->moy_int = moyenne;
    else if(position == 9)
        matiere->moy_dev = moyenne;
    else if(position == 10)
        matiere->moy_totale = moyenne;
    else if(position == 11)
        matiere->moy_coeff = moyenne;
    else
    {
        fprintf(stdout, "Ajout d'une moyenne a une position non-reconnue.");
        return error;
    }
    return ok;
}

Return_Bool TracerGraphe(Fiche *fiche, double note, short indice)
{
    if(fiche == NULL || fiche->graphe == NULL || fiche->graphe->premiere == NULL)
    {
        fprintf(stdout, "Impossible d'ajouter un point a un graphe NULL :: TracerGraphe()\n");
        return error;
    }
    else
    {
        NotePoint *actuel = fiche->graphe->premiere;
        short i = 0;
        double x;
        i = CheckArray(fiche->graphe->indices, fiche->graphe->nbre_point, indice);
        if(i >= 0)
        {
            NotePoint *precedent = fiche->graphe->premiere;
            int j = 0;
            for(j = 0; j < i; j++)
            {
                precedent = actuel;
                actuel = actuel->suivant;
            }
            if(note < 0.0)
            {
                if(j == 0)
                    fiche->graphe->premiere = actuel->suivant;
                else
                    precedent->suivant = actuel->suivant;
                for(i = i; i+1 < fiche->graphe->nbre_point; i++)
                    fiche->graphe->indices[i] = fiche->graphe->indices[i+1];
                fiche->graphe->indices[i] = -1;
                free(actuel);
                fiche->graphe->nbre_point--;
                x = (double)GRAPHE_W/(fiche->graphe->nbre_point-1);
                actuel = fiche->graphe->premiere;
                for(j = 0; j < fiche->graphe->nbre_point; j++)
                {
                    actuel->point.x = GRAPHE_X + (j*x);
                    actuel = actuel->suivant;
                }
            }
            else
            {
                actuel->note = note;
                actuel->point.y = (GRAPHE_Y+GRAPHE_H)-((double)GRAPHE_POINT_Y*note);
            }
        }
        else
        {
            if(note < 0)
                return error;
            x = (double)GRAPHE_W/fiche->graphe->nbre_point;
            for(i = 0; i < fiche->graphe->nbre_point; i++)
            {
                actuel->point.x = GRAPHE_X + (i*x);
                if(actuel->suivant == NULL)
                    actuel->suivant = malloc(sizeof(*actuel->suivant));
                actuel = actuel->suivant;
            }
            actuel->indice = indice;
            actuel->note = note;
            actuel->point.x = GRAPHE_X + (fiche->graphe->nbre_point*x);
            actuel->point.y = (GRAPHE_Y+GRAPHE_H)-((double)GRAPHE_POINT_Y*note);
            actuel->suivant = NULL;
            fiche->graphe->indices[fiche->graphe->nbre_point] = indice;
            fiche->graphe->nbre_point++;
        }
        SDL_Rect graphe = {GRAPHE_X, GRAPHE_Y, GRAPHE_W, GRAPHE_H};
        SET_JPBLUE(fiche->render);
        SDL_RenderFillRect(fiche->render, &graphe);
        SET_INDIGO(fiche->render);
        SDL_RenderDrawRect(fiche->render, &graphe);
        SET_BLACK(fiche->render);
        SDL_Point pb = {fiche->graphe->premiere->point.x, fiche->graphe->premiere->point.y};
        SDL_RenderDrawPoint(fiche->render, pb.x, pb.y);
        fprintf(stdout, "Debut tracer :: TracerGraphe()\n");
        if(fiche->graphe->nbre_point-1 > 0)
        {
            actuel = fiche->graphe->premiere->suivant;
            fprintf(stdout, "Entrer :: TracerGraphe()\n");
            SDL_Point pa = {actuel->point.x, actuel->point.y};
            for(i = 0; i < fiche->graphe->nbre_point-1; i++)
            {
                fprintf(stdout, "Tracer N°%d :: TracerGraphe()\n", i);
                SDL_RenderDrawLine(fiche->render, pb.x, pb.y, pa.x, pa.y);
                if(actuel->suivant != NULL)
                {
                    actuel = actuel->suivant;
                    pb.x = pa.x;
                    pb.y = pa.y;
                    pa.x = actuel->point.x;
                    pa.y = actuel->point.y;
                }
            }
        }
        SDL_RenderPresent(fiche->render);
        fprintf(stdout, "Fin :: TracerGraphe()\n");
        return ok;
    }
}

Return_Bool RetracerGraphe(Fiche *fiche, short indiceMatiere)
{
    if(fiche == NULL || fiche->graphe == NULL || fiche->graphe->premiere == NULL)
    {
        fprintf(stdout, "Impossible d'ajouter un point a un graphe NULL :: TracerGraphe()\n");
        return error;
    }
    else
    {
        /*D'abord, on supprime les traces de l'ancienne matiere*/
        NotePoint *actuel = NULL;
        short indice = indiceMatiere * 8;
        int i = 0;
        int j = 0;
        for(i = 0; i < 8; i++)
        {
            actuel = fiche->graphe->premiere;
            j = CheckArray(fiche->graphe->indices, fiche->graphe->nbre_point, indice);
            if(j >= 0)
            {
                if(j == 0)
                {
                    fiche->graphe->premiere = actuel->suivant;
                    while(j+1 < fiche->graphe->nbre_point)
                    {
                        fiche->graphe->indices[j] = fiche->graphe->indices[j+1];
                        j++;
                    }
                    fiche->graphe->indices[j] = -1;
                }
                else if(j > 0)
                {
                    NotePoint *precedent = NULL;
                    for(int k = 0; k < j; k++)
                    {
                        precedent = actuel;
                        actuel = actuel->suivant;
                    }
                    precedent->suivant = actuel->suivant;
                    while(j+1 < fiche->graphe->nbre_point)
                    {
                        fiche->graphe->indices[j] = fiche->graphe->indices[j+1];
                        j++;
                    }
                    fiche->graphe->indices[j] = -1;
                }
                free(actuel);
                fiche->graphe->indices[j] = -1;
                fiche->graphe->nbre_point--;
            }
            indice++;
        }
        /*Maintenant, on peut retracer le graphe*/
        SDL_Rect graphe = {GRAPHE_X, GRAPHE_Y, GRAPHE_W, GRAPHE_H};
        SET_JPBLUE(fiche->render);
        SDL_RenderFillRect(fiche->render, &graphe);
        SET_INDIGO(fiche->render);
        SDL_RenderDrawRect(fiche->render, &graphe);
        if(fiche->graphe->nbre_point > 0)
        {
            SET_BLACK(fiche->render);
            SDL_Point pb = {fiche->graphe->premiere->point.x, fiche->graphe->premiere->point.y};
            SDL_RenderDrawPoint(fiche->render, pb.x, pb.y);
            if(fiche->graphe->nbre_point-1 > 0)
            {
                actuel = fiche->graphe->premiere->suivant;
                fprintf(stdout, "Entrer :: TracerGraphe()\n");
                SDL_Point pa = {actuel->point.x, actuel->point.y};
                for(i = 0; i < fiche->graphe->nbre_point-1; i++)
                {
                    fprintf(stdout, "Tracer N°%d :: TracerGraphe()\n", i);
                    SDL_RenderDrawLine(fiche->render, pb.x, pb.y, pa.x, pa.y);
                    if(actuel->suivant != NULL)
                    {
                        actuel = actuel->suivant;
                        pb.x = pa.x;
                        pb.y = pa.y;
                        pa.x = actuel->point.x;
                        pa.y = actuel->point.y;
                    }
                }
            }
        }
        SDL_RenderPresent(fiche->render);
        return ok;
    }
}
