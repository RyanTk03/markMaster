#ifndef CONSTANTES_H_INCLUDED
#define CONSTANTES_H_INCLUDED

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

typedef enum Return_Bool
{
    quit_window = -1, error = 0, ok = 1
} Return_Bool;

static SDL_Rect ligneActuelle;
static SDL_DisplayMode resolution;
static TTF_Font *myFont;
static SDL_Color noire;

//La fenêtre principale
#define DECLARE_FONT() myFont = TTF_OpenFont("appData/fonts/taile.ttf", 18)
#define DECLARE_FONT_COLORE() noire = {0, 0, 0}
#define MA_LARGEUR_FENETRE 1366
#define MA_HAUTEUR_FENETRE 700
#define MA_RESOLUTION_W 1366
#define MA_RESOLUTION_H 768
#define UL_X ((double)resolution.w/(double)MA_RESOLUTION_W)
#define UL_Y ((double)resolution.h/(double)MA_RESOLUTION_H)
#define LARGEUR_UTILISATEUR (MA_LARGEUR_FENETRE*UL_X)
#define HAUTEUR_UTILISATEUR (MA_HAUTEUR_FENETRE*UL_Y)
//La boite à outils
#define OUTILS_W (1364 * UL_X)
#define OUTILS_H (80 * UL_Y)
#define OUTILS_X (LARGEUR_UTILISATEUR/2 - OUTILS_W/2)
#define OUTILS_Y (20 * UL_Y)
#define LITLE_BUTTON_W (100 * UL_X)
#define LITLE_BUTTON_H (39 * UL_Y)
#define SMALL_BUTTON_W (100 * UL_X)
#define SMALL_BUTTON_H (78 * UL_Y)
//La fenêtre de nouvelle matiere et les éléments à l'intérieur
#define NEW_SUBJECT_WINDOW_W (480 * UL_X)
#define NEW_SUBJECT_WINDOW_H (200 * UL_Y)
#define NEW_SUBJECT_NAME_W (140 * UL_X)
#define NEW_SUBJECT_NAME_H (30 * UL_Y)
#define NEW_SUBJECT_NAME_X (130 * UL_X)
#define NEW_SUBJECT_NAME_Y (65 * UL_Y)
#define NEW_SUBJECT_COEFF_W (50 * UL_X)
#define NEW_SUBJECT_COEFF_H (30 * UL_Y)
#define NEW_SUBJECT_COEFF_X (130 * UL_X)
#define NEW_SUBJECT_COEFF_Y (105 * UL_Y)
#define NEW_FICHE_WINDOW_W (300 * UL_X)
#define NEW_FICHE_WINDOW_H (100 * UL_Y)
#define NEW_FICHENAME_AREA_W (144 * UL_X)
#define NEW_FICHENAME_AREA_H (30 * UL_Y)
#define NEW_FICHENAME_AREA_X (100 * UL_X)
#define NEW_FICHENAME_AREA_Y (NEW_FICHE_WINDOW_H/2 - NEW_FICHENAME_AREA_H/2)
#define CHAR_RECT_W (19 * UL_X)

//Les caractéristiques d'une fiche
#define FICHE_W (1360 * UL_X)
#define FICHE_H (560 * UL_Y)
#define FICHE_X (3 * UL_X)
#define FICHE_Y ((NAVIG_VOLET_Y+NAVIG_VOLET_H)*UL_Y)
#define LARGEUR_FOND (310 * UL_X)
#define HAUTEUR_FOND (265 * UL_Y)
#define GRAPHE_W (FICHE_W - (TABLEAU_TETE_X+TABLEAU_TETE_W+UL_X))
#define GRAPHE_X (TABLEAU_TETE_X+TABLEAU_TETE_W+UL_X)
#define GRAPHE_H (FICHE_H - 2)
#define GRAPHE_Y (1)
#define GRAPHE_POINT_Y ((GRAPHE_H-3)/20)

//Les caractéristiques de l'entête du tableau
#define TABLEAU_TETE_W (880 * UL_X)
#define TABLEAU_TETE_H (60 * UL_Y)
#define TABLEAU_TETE_X (BOUTON_PLUS_W + (1*UL_X))
#define TABLEAU_TETE_Y (5)

//Les caractéristiques d'une ligne du tableau
#define MATIERE_W (140 * UL_X)
#define MATIERE_H (30 * UL_Y)
#define MATIERE_X 0
#define MATIERE_Y 0
#define COEFFICIENT_W (30 * UL_X)
#define COEFFICIENT_H (30 * UL_Y)
#define COEFFICIENT_X MATIERE_W
#define COEFFICIENT_Y MATIERE_Y
#define LIGNE_X TABLEAU_TETE_X
#define LIGNE_DEPART_Y (TABLEAU_TETE_Y + TABLEAU_TETE_H - 1)
#define LIGNE_Y(fiche) (LIGNE_DEPART_Y + (LIGNE_H * fiche->list_matiere->n))
#define LIGNE_W (880 * UL_X)
#define LIGNE_H (30 * UL_Y)
#define NOTE_W (49*UL_X)
#define CASE_W (49 * UL_X)
#define CASE_H (30 * UL_Y)
#define MOY_INT_X (464 * UL_X)
#define MOY_DEV_X (612 * UL_X)
#define MOY_SUR_20_X (661 * UL_X)
#define MOY_COEF_X (710 * UL_X)
#define APPRECIATION_W (121 * UL_X)
#define APPRECIATION_H (30 * UL_Y)
#define APPRECIATION_X (759 * UL_X)
#define ACTUALIZE_RECT_LINE(fiche) ligneActuelle.w = LIGNE_W;\
                               ligneActuelle.h = LIGNE_H;\
                               ligneActuelle.x = TABLEAU_TETE_X;\
                               ligneActuelle.y = LIGNE_Y(fiche)
/*Les boutons */
#define BOUTON_PLUS_W (13 * UL_X)
#define BOUTON_PLUS_H (13 * UL_Y)
#define BOUTON_PLUS_X (TABLEAU_TETE_X - BOUTON_PLUS_W + 1)
#define BOUTON_PLUS_Y(fiche) (LIGNE_DEPART_Y + (LIGNE_H * fiche->list_matiere->n))

#define DEBUT_NOTE_X (TABLEAU_TETE_X + MATIERE_W + COEFFICIENT_W)
#define DECAL_CURSEUR_X (3)
#define CURSEUR_W (1)
#define CURSEUR_H (24 * UL_Y)

#define NAVIG_VOLET_W (1360 * UL_X)
#define NAVIG_VOLET_H (22 * UL_Y)
#define NAVIG_VOLET_X (3)
#define NAVIG_VOLET_Y (103 * UL_Y)
#define VOLET_W (250 * UL_X)
#define VOLET_H (22 * UL_Y)
#define VOLET_X(fiches) (NAVIG_VOLET_X + (fiches->n * VOLET_W))
#define SUPP_FICHE_W (20 * UL_X)
#define SUPP_FICHE_H VOLET_H

#define MAX_FICHE 5
#define MAX_MATIERE 9
#define MAX_NOTE 64
#define MAX_NAME_CHAR 12
#define MAX_MENTION_CHAR 12

#define BILAN_X (0 * UL_X)
#define BILAN_Y (LIGNE_DEPART_Y+(MAX_MATIERE*LIGNE_H)+(50*UL_Y))
#define BILAN_W (FICHE_W)
#define BILAN_H (FICHE_H-BILAN_Y)
#define BILAN_TITRE_DECALAGE_X (20*UL_X)
#define BILAN_MOYENNE_DECALAGE_X (10*UL_X)
#define BILAN_TITRE_DECALAGE_Y (5)

#define SET_WHITE(render) SDL_SetRenderDrawColor(render, 255, 255, 255, 255)
#define SET_JPBLUE(render) SDL_SetRenderDrawColor(render, 160, 180, 255, 255)
#define SET_JPYELLOW(render) SDL_SetRenderDrawColor(render, 255, 255, 50, 255)
#define SET_INDIGO(render) SDL_SetRenderDrawColor(render, 63, 72, 204, 255)
#define SET_BLACK(render) SDL_SetRenderDrawColor(render, 0, 0, 0, 255)
#define SET_GREY(render) SDL_SetRenderDrawColor(render, 240, 240, 240, 255)
#define SET_GREEN(render) SDL_SetRenderDrawColor(render, 0, 255, 255, 255)

#endif // CONSTANTES_H_INCLUDED
