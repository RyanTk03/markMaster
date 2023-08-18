#include <stdio.h>
#include <stdlib.h>

#include "../inc/variables.h"
#include "../inc/sauvegarde.h"

Return_Bool enregistrer_fiche(Fiche *aSauver, char *abs_patch)
{
    if(aSauver == NULL)
        return error;
    int i = 0, j = 0;
    char patch[strlen(abs_patch)+6+strlen(aSauver->nom)+1];
    for(i = strlen(abs_patch)-1; abs_patch[i] != '\\'; i--){}
    for(j = 0; j <= i; j++)
        patch[j] = abs_patch[j];
    patch[j] = 'F'; j++;
    patch[j] = 'i'; j++;
    patch[j] = 'c'; j++;
    patch[j] = 'h'; j++;
    patch[j] = 'e'; j++;
    patch[j] = '/'; j++;
    for(i = 0; i < strlen(aSauver->nom); i++)
    {
        patch[j] = aSauver->nom[i];
        j++;
    }
    patch[j] = '\0';
    fprintf(stdout, "Debut %s :: enregistrer_fiche()\n", patch);
    FILE *fichier = fopen(patch, "w+");
    if(fichier == NULL)
    {
        fprintf(stderr, "Impossible d'ouvrir le fichier :: enregistrer_fiche()\n");
        return error;
    }
    NotePoint *actuel = aSauver->graphe->premiere;
    i = 0;
    j = 0;
    fprintf(fichier, "%d\n", aSauver->list_matiere->n);
    for(i = 0; i < 167; i++)
        fprintf (fichier, "*");
    for(i = 0; i < aSauver->list_matiere->n; i++)
    {
        fprintf(fichier, "\n");
        fprintf(fichier, "%s ", aSauver->list_matiere->addrMatiere[i]->nom);
        fprintf(fichier, "%.2lf\n", aSauver->list_matiere->addrMatiere[i]->coefficient);
        fprintf(fichier, "%d ", aSauver->list_matiere->addrMatiere[i]->interrogations->n);
        for(j = 0; j < 6; j++)
            fprintf(fichier, "%.2lf-", aSauver->list_matiere->addrMatiere[i]->interrogations->notes[j]);
        fprintf(fichier, "\n");
        fprintf(fichier, "%d ", aSauver->list_matiere->addrMatiere[i]->devoirs->n);
        for(j = 0; j < 2; j++)
            fprintf(fichier, "%.2lf-", aSauver->list_matiere->addrMatiere[i]->devoirs->notes[j]);
        fprintf(fichier, "\n");
        fprintf(fichier, "%.2lf-", aSauver->list_matiere->addrMatiere[i]->moy_int);
        fprintf(fichier, "%.2lf-", aSauver->list_matiere->addrMatiere[i]->moy_dev);
        fprintf(fichier, "%.2lf-", aSauver->list_matiere->addrMatiere[i]->moy_totale);
        fprintf(fichier, "%.2lf-", aSauver->list_matiere->addrMatiere[i]->moy_coeff);
        fprintf(fichier, "\n");
        fprintf(fichier, "%s\n", aSauver->list_matiere->addrMatiere[i]->appreciation);
        for(j = 0; j < 167; j++)
            fprintf(fichier, "*");
    }
    fprintf(fichier, "\n");
    fprintf(fichier, "%d ", aSauver->graphe->nbre_point);
    for(j = 0; j < aSauver->graphe->nbre_point; j++)
    {
        fprintf(fichier, "%d#%.2lf-", actuel->indice, actuel->note);
        actuel = actuel->suivant;
    }
    fprintf(stdout, "Fin :: enregistrer_fiche()\n");
    fclose(fichier);
    return ok;
}
