#ifndef VARIABLES_H_INCLUDED
#define VARIABLES_H_INCLUDED

#include "BTN_.h"
#include "constantes.h"

typedef struct NotePoint NotePoint;
struct NotePoint
{
    double note;
    SDL_Point point;
    short indice;
    NotePoint *suivant;
};

typedef struct Graphe Graphe;
struct Graphe
{
    NotePoint *premiere;
    short nbre_point;
    short indices[MAX_NOTE];
};

typedef struct Bilan Bilan;
struct Bilan
{
    double totaux_coefficients;
    double moyInt;
    double moyDev;
    double moyCoef;
    double moySur20;
    char *mention;
};

typedef enum TypeNote
{
    devoir, interrogation
} TypeNote;

typedef struct Liste_n Liste_n;
struct Liste_n
{
    TypeNote type;
    short max_note;
    double *notes;
    double somme;
    int n;
};

typedef struct Matiere Matiere;
struct Matiere
{
    char *nom;
    Liste_n *interrogations;
    Liste_n *devoirs;
    double coefficient;
    double moy_int;
    double moy_dev;
    double moy_totale;
    double moy_coeff;
    SDL_Texture *ligne;
    int indice;
    char *appreciation;
};

typedef struct Liste_m Liste_m;
struct Liste_m
{
    Matiere *addrMatiere[MAX_MATIERE];
    int n;
    Bilan bilan;
};

typedef struct Volet Volet;
struct Volet
{
    SDL_Texture *texture;
    SDL_Texture *textureActu;
    SDL_Rect *rectangle;
    BTN_Button *supp;
};

typedef enum StatutFiche
{
    actuelle, arriere_plan
} StatutFiche;

typedef struct Fiche Fiche;
struct Fiche
{
    char *nom;
    SDL_Renderer *render;
    SDL_Texture *texture;
    SDL_Rect curseur;
    Liste_m *list_matiere;
    Volet *volet_titre;
    SDL_Rect surface_rect;
    BTN_Button *bouton_plus;
    StatutFiche statut;
    Graphe *graphe;
};

typedef struct Liste_f Liste_f;
struct Liste_f
{
    Fiche *addrFiche[MAX_FICHE];
    int n;
};

typedef struct BoiteOutils BoiteOutils;
struct BoiteOutils
{
    SDL_Rect rectangle;
    BTN_Button *addMatiere;
    BTN_Button *suppMatiere;
    BTN_Button *addFiche;
    BTN_Button *suppFiche;
    BTN_Button *saveFiche;
    BTN_Button *klculBilan;
    BTN_Button *chargeFiche;
};

Fiche *CreerFiche();//Fini
Fiche* DetruireFiche(Liste_f *fiches, Fiche *fiche);//Fini
Liste_f* CreerListeFiche();//Fini
void SupprimerListeFiche(Liste_f *fiches);//Fini
Matiere* CreerMatiere(Fiche *fiche, char *nom, double coefficient);//Fini
void SupprimerMatiere(Matiere *matiere, Fiche *fiche);//Fini
Liste_m* CreerListeMatiere();//Fini
void SupprimerListeMatiere(Liste_m *matieres, Fiche *fiche);//Fini
Liste_n *InititialiserNotes(TypeNote typeNote);
Volet *Creer_AfficherVolet(Liste_f *fiches);//Fini
Return_Bool mouseMoveInVolet(Fiche *fiche, SDL_Event event);//Fini
void SupprimerVolet(Liste_f *fiches, Fiche *fiche);//Fini
BoiteOutils *CreerOutils();
void DetruireOutils(BoiteOutils *outils);
void afficherOutils(SDL_Renderer *render, BoiteOutils *outils);
Graphe* CreerGraphe(void);
void LibererGraphe(Graphe *graphe);
short CheckArray(short *tableau, int taille, short toCheck);

#endif // VARIABLES_H_INCLUDED
