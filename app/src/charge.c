#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "../inc/variables.h"
#include "../inc/constantes.h"
#include "../inc/charge.h"
#include "../inc/ajouter.h"

Fiche* charger_fiche(Liste_f *fiches, SDL_Renderer *render, char *fichier)
{
    Fiche *fiche = CreerFiche();
    SDL_Surface *tableau = IMG_Load("appData/IMGs/entete.jpg");
    SDL_Surface *fond = IMG_Load("appData/IMGs/fondIMG.jpg");

    SDL_Texture *texTableau = SDL_CreateTextureFromSurface(render, tableau);
    SDL_FreeSurface(tableau);
    SDL_Texture *texFond = SDL_CreateTextureFromSurface(render, fond);
    SDL_FreeSurface(fond);
    SDL_SetTextureBlendMode(texFond, SDL_BLENDMODE_BLEND);
    SDL_SetTextureAlphaMod(texFond, 20);

    SDL_Rect position;

    if(fiches->n < MAX_FICHE && fiche != NULL && fichier != NULL)
    {
        char *nom = malloc((strlen(fichier)+5)*sizeof(char));
        strcpy(nom, fichier);
        strcat(nom, ".jpf");
        free(fichier);
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
        SDL_Point point[5];
        point[0].x = TABLEAU_TETE_X+TABLEAU_TETE_W+UL_X;
        point[0].y = 1;
        point[1].x = FICHE_W;
        point[1].y = 1;
        point[2].x = FICHE_W;
        point[2].y = FICHE_H-2;
        point[3].x = point[0].x;
        point[3].y = FICHE_H-2;
        point[4].x = point[0].x;
        point[0].y = 1;
        SDL_RenderDrawLine(fiche->render, point[0].x, point[0].y, point[1].x, point[1].y);
        SDL_RenderDrawLine(fiche->render, point[1].x, point[1].y, point[2].x, point[2].y);
        SDL_RenderDrawLine(fiche->render, point[2].x, point[2].y, point[3].x, point[3].y);
        SDL_RenderDrawLine(fiche->render, point[3].x, point[3].y, point[0].x, point[0].y);
        SDL_SetRenderTarget(fiche->render, NULL);
        fiches->addrFiche[fiches->n] = fiche;
        fiche->volet_titre = Creer_AfficherVolet(fiches);
        fiches->n++;
        SDL_DestroyTexture(texFond);
        SDL_DestroyTexture(texTableau);
        SDL_RenderCopy(fiche->render, fiche->texture, NULL, &fiche->surface_rect);
        SDL_RenderPresent(fiche->render);
        charger_donnes(nom, fiche);
        return fiche;
    }
    else
    {
        SDL_DestroyTexture(texFond);
        SDL_DestroyTexture(texTableau);
        if(fiche != NULL)
            fprintf(stdout, "La fiche creer est NULL :: charger_fiche()");
        else if(fiches->n >= MAX_FICHE)
            fprintf(stdout, "Le nombre limite de fiche a ete atteint :: charger_fiche()");
        else
            fprintf(stdout, "Impossible de charger la fiche :: charger_fiche()");
        return NULL;
    }
}

Return_Bool charger_donnes(char *fichier, Fiche *fiche)
{
    char patch[25];
    strcpy(patch, "Fiche/");
    strcat(patch, fichier);
    FILE *file = fopen(patch, "r");
    if(fichier == NULL || file == NULL || fiche == NULL)
    {
        if(fichier == NULL)
            printf("fichier == NULL\n");
        else
            printf("%s\n", patch);
        if(file == NULL)
            printf("file == NULL\n");
        if(fiche == NULL)
            printf("fiche == NULL\n");
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, NULL, "Impossible de charger les donnes.", NULL);
        return error;
    }
    else
    {
        int i = 0, j = 0;
        char mot[MAX_NAME_CHAR+1];
        char *nom;
        double valeur;
        fscanf(file, "%d", &j);
        const short nbreMat = j;
        fseek(file, 169, SEEK_CUR);
        while(i < nbreMat)
        {
            nom = malloc((MAX_NAME_CHAR+1)*sizeof(char));
            fscanf(file, "%s %lf", nom, &valeur);
            charger_matiere(fiche, nom, valeur);
            SDL_SetRenderTarget(fiche->render, fiche->list_matiere->addrMatiere[i]->ligne);
            {
                short indice = 0;
                fscanf(file, "%d ", &fiche->list_matiere->addrMatiere[i]->interrogations->n);//nbre d'interro
                for(j = 0; j < fiche->list_matiere->addrMatiere[i]->interrogations->max_note; j++) //interrogations
                {
                    fscanf(file, "%lf", &valeur);
                    if(valeur >= 0)
                    {
                        charger_note(fiche, valeur, indice);
                        fiche->list_matiere->addrMatiere[i]->interrogations->notes[indice] = valeur;
                        fiche->list_matiere->addrMatiere[i]->interrogations->somme += valeur;
                    }
                    indice++;
                    fseek(file, 1, SEEK_CUR);
                }
            }
            {
                short indice = 0;
                fscanf(file, "%d ", &fiche->list_matiere->addrMatiere[i]->devoirs->n);//valeur de devoir
                for(j = 0; j < fiche->list_matiere->addrMatiere[i]->devoirs->max_note; j++) //devoirs
                {
                    fscanf(file, "%lf", &valeur);
                    if(valeur >= 0)
                    {
                        charger_note(fiche, valeur, indice+7);
                        fiche->list_matiere->addrMatiere[i]->devoirs->notes[j] = valeur;
                        fiche->list_matiere->addrMatiere[i]->devoirs->somme += valeur;
                    }
                    indice++;
                    fseek(file, 1, SEEK_CUR);
                }
            }
            for(j = 0; j < 4; j++) //toutes les moyennes
            {
                fscanf(file, "%lf", &valeur);
                fseek(file, 1, SEEK_CUR);
                if(valeur >= 0)
                {
                    if(j == 0)
                    {
                        charger_note(fiche, valeur, 6);
                        fiche->list_matiere->addrMatiere[i]->moy_int = valeur;
                    }
                    else
                    {
                        charger_note(fiche, valeur, j+8);
                        if(j == 1)
                            fiche->list_matiere->addrMatiere[i]->moy_dev = valeur;
                        if(j == 2)
                            fiche->list_matiere->addrMatiere[i]->moy_totale = valeur;
                        if(j == 3)
                            fiche->list_matiere->addrMatiere[i]->moy_coeff = valeur;
                    }
                }
            }
            fscanf(file, "%s", mot);
            if(strcmp(mot, "(null)") != 0)
            {
                char *appreciation = malloc((strlen(mot)+1)*sizeof(char));
                for(j = 0; j < strlen(mot); j++)
                    appreciation[j] = mot[j];
                appreciation[j] = '\0';
                fiche->list_matiere->addrMatiere[i]->appreciation = appreciation;
                SDL_Surface *s = TTF_RenderText_Blended(myFont, appreciation, noire);
                SDL_Texture *t = SDL_CreateTextureFromSurface(fiche->render, s);
                SDL_Rect r = {APPRECIATION_X+(APPRECIATION_W/2)-(s->w/2), (APPRECIATION_H/2)-(s->h/2), s->w, s->h};
                if(s->w > APPRECIATION_W)
                {
                    r.w = APPRECIATION_W;
                    r.x = APPRECIATION_X;
                }
                if(s->h > APPRECIATION_H)
                {
                    r.h = APPRECIATION_H;
                    r.y = 0;
                }
                SDL_RenderCopy(fiche->render, t, NULL, &r);
                SDL_FreeSurface(s);
            }
            SDL_SetRenderTarget(fiche->render, fiche->texture);
            SET_JPBLUE(fiche->render);
            SDL_RenderFillRect(fiche->render, fiche->bouton_plus->rect);
            SDL_RenderCopy(fiche->render, fiche->list_matiere->addrMatiere[i]->ligne, NULL, &ligneActuelle);
            fiche->bouton_plus->rect->y = BOUTON_PLUS_Y(fiche);
            BTN_CopyButton(fiche->bouton_plus, fiche->render);
            SDL_RenderPresent(fiche->render);
            fseek(file, 169, SEEK_CUR);
            i++;
        }
        {
            int indice = 0;
            int p = 0;
            fscanf(file, "%d ", &p);
            for(j = 0; j < p; j++)
            {
                fscanf(file, "%d", &indice);
                fseek(file, 1, SEEK_CUR);
                fscanf(file, "%lf", &valeur);
                TracerGraphe(fiche, valeur, indice);
                fseek(file, 1, SEEK_CUR);
            }
        }
        fiche->list_matiere->n = nbreMat;
        SDL_SetRenderTarget(fiche->render, NULL);
        SDL_RenderCopy(fiche->render, fiche->texture, NULL, &fiche->surface_rect);
        SDL_RenderPresent(fiche->render);
        fclose(file);
        return ok;
    }
}

Return_Bool charger_matiere(Fiche *fiche, char *nomMatiere, double coefficient)
{
    Matiere *matiere = CreerMatiere(fiche, nomMatiere, coefficient);
    if(fiche == NULL || matiere == NULL)
    {
        if(fiche == NULL)
            SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, NULL, "Impossible de charger la fiche.", NULL);
        else
            fprintf(stdout, "Impossible d'ajouter la matiere n°%d", fiche->list_matiere->n);
        return error;
    }
    else
    {
        if(fiche->list_matiere->n >= MAX_MATIERE)
        {
            SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_WARNING, NULL, "Le nombre limite de matiere a ete atteint!", NULL);
            return error;
        }
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
        fiche->bouton_plus = BTN_CreateButtonWithTexture(BTN_DYNAMIC_BUTTON_FORM_B, BOUTON_PLUS_W, BOUTON_PLUS_H,
                             BOUTON_PLUS_X, BOUTON_PLUS_Y(fiche), "+", "appData/fonts/taile.ttf");
        BTN_CopyButton(fiche->bouton_plus, fiche->render);
        SDL_SetRenderTarget(fiche->render, NULL);
        SDL_RenderCopy(fiche->render, fiche->texture, NULL, &fiche->surface_rect);
        SDL_RenderPresent(fiche->render);
        SDL_DestroyTexture(texName);
        SDL_FreeSurface(surName);
        SDL_DestroyTexture(texLigneImg);
        matiere->indice = fiche->list_matiere->n;
        printf("sortie de la fonction AjouterFiche()\n");
        return ok;
    }
}

void charger_note(Fiche *fiche, double note, short numero)
{
    char str_note[6];
    sprintf(str_note, "%.2lf", note);
    SDL_Surface *sur_note = TTF_RenderText_Blended(myFont, str_note, noire);
    SDL_Texture *tex_note = SDL_CreateTextureFromSurface(fiche->render, sur_note);
    SDL_Rect rect_note = {MATIERE_W+COEFFICIENT_W+DECAL_CURSEUR_X+(numero*NOTE_W), (CASE_H/2)-(CURSEUR_H/2), (sur_note->w*UL_X), (sur_note->h*UL_Y)};
    SDL_RenderCopy(fiche->render, tex_note, NULL, &rect_note);
    SDL_FreeSurface(sur_note);
    SDL_DestroyTexture(tex_note);
}
