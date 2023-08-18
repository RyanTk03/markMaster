#ifndef BTN_BUTTON_H_INCLUDED
#define BTN_BUTTON_H_INCLUDED

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include "constantes.h"

#define BTN_STATIC_BUTTON 0
#define BTN_DYNAMIC_BUTTON_FORM_A 1
#define BTN_DYNAMIC_BUTTON_FORM_B 2
#define BTN_MAX_BUTTON_NAME 27
#define BTN_BLUE(surface) SDL_MapRGB(surface->format, 0, 0, 255);
#define BTN_RED(surface) SDL_MapRGB(surface->format, 255, 0, 0);
#define BTN_GREEN(surface) SDL_MapRGB(surface->format, 0, 255, 0);
#define BTN_GOLD(surface) SDL_MapRGB(surface->format, 255, 201, 14);
#define BTN_YELLOW_CLAIR(surface) SDL_MapRGB(surface->format, 239, 228, 176);
#define BTN_YELLOW_MODIFIED(surface) SDL_MapRGB(surface->format, 223, 201, 98);
#define BTN_PINK_DARKEN(surface) SDL_MapRGB(surface->format, 255, 168, 170);
#define BTN_PINK(surface) SDL_MapRGB(surface->format, 255, 174, 201);
#define BTN_ORANGE(surface) SDL_MapRGB(surface->format, 255, 127, 39);
#define BTN_YELLOW(surface) SDL_MapRGB(surface->format, 255, 255, 0);
#define BTN_BLACK(surface) SDL_MapRGB(surface->format, 0, 0, 0);
#define BTN_WHITE(surface) SDL_MapRGB(surface->format, 255, 255, 255);

typedef struct BTN_Button BTN_Button;
struct BTN_Button
{
    SDL_Surface *surface;
    SDL_Texture *texture;
    SDL_Surface *nameSurface;
    SDL_Rect *rect;
    SDL_Rect *nameRect;
    char *name;
    char *fontName;
    TTF_Font *font;
    SDL_Color backgroundColor; //At default, the surface is white
    SDL_Color effectColor; //Default color is blue with alpha = 128
    SDL_Color fontColor; //Default color is black
    Uint8 type;
};

#define BTN_CURSOR_CLIC_IN(button, event) event.button.x > button->rect->x && event.button.y > button->rect->y && event.button.x < button->rect->x + button->rect->w && event.button.y < button->rect->y + button->rect->h
#define BTN_CURSOR_MOVE_IN(button, event) event.motion.x > button->rect->x && event.motion.y > button->rect->y && event.motion.x < button->rect->x + button->rect->w && event.motion.y < button->rect->y + button->rect->h
#define BTN_MOUSE_MOVE_IN_RECT(rect, event) event.motion.x > rect->x && event.motion.y > rect->y && event.motion.x < rect->x + rect->w && event.motion.y < rect->y + rect->h
#define BTN_MOUSE_CLIC_ON_RECT(rect, event) event.button.x > rect->x && event.button.y > rect->y && event.button.x < rect->x + rect->w && event.button.y < rect->y + rect->h

BTN_Button *BTN_CreateButtonWithTexture(Uint8 type, int w, int h, int x, int y, const char *name, const char *font);
SDL_Rect *BTN_CopyButton(BTN_Button *button, SDL_Renderer *renderer);
void BTN_FreeButton(BTN_Button *button);

BTN_Button *BTN_CreateButtonWithSurface(Uint8 type, int w, int h, int d, int x, int y, char *name, char *font);
int BTN_BlitButton(BTN_Button *button, SDL_Surface *surface);
int BTN_SetBackgroudImg(BTN_Button *button, char* file, SDL_Window *window);
void BTN_ChangeButtonType(BTN_Button *button, Uint8 newType);
char *BTN_ChangeButtonName(BTN_Button *button, char *name, SDL_Window *window);
TTF_Font *BTN_ChangeButtonFont(BTN_Button *button, char *file, int fontSize, SDL_Window *window);
int BTN_ChangeFontColor(BTN_Button *button, Uint8 r, Uint8 g, Uint8 b, Uint8 a, SDL_Window *window);
int BTN_ChangeBackgroundColor(BTN_Button *button, Uint8 r, Uint8 g, Uint8 b, Uint8 a, SDL_Window *window);
int BTN_ChangeEffectColor(BTN_Button *button, Uint8 r, Uint8 g, Uint8 b, Uint8 a, SDL_Renderer *renderer);
void BTN_ChangeButtonRect(SDL_Renderer *render, SDL_Surface *surface, BTN_Button *button, int x, int y, int w, int h);
char* BTN_Stradd(const char *chaine1, const char *chaine2);

int BTN_SurfaceMouseMotion(BTN_Button *button, SDL_Event event, SDL_Surface *surface);
int BTN_SurfaceMouseClicDown(BTN_Button *button, SDL_Event event, SDL_Surface *surface);
int BTN_SurfaceMouseClicUp(BTN_Button *button, SDL_Event event, SDL_Surface *surface);
int BTN_SurfaceMouseClic(BTN_Button *button, SDL_Event event, SDL_Surface *surface);

int BTN_RenderMouseMotion(BTN_Button *button, SDL_Event event, SDL_Renderer *renderer);
int BTN_RenderMouseClicDown(BTN_Button *button, SDL_Event event, SDL_Renderer *renderer);
int BTN_RenderMouseClicUp(BTN_Button *button, SDL_Event event, SDL_Renderer *renderer);
int BTN_RenderMouseClic(BTN_Button *button, SDL_Event event, SDL_Renderer *renderer);

#endif // BTN_BUTTON_H_INCLUDED
