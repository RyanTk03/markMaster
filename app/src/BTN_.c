#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "../inc/BTN_.h"

BTN_Button *BTN_CreateButtonWithTexture(Uint8 type, int w, int h, int x, int y, const char *name, const char *font)
{
    int i = 0;
    BTN_Button *bouton = malloc(sizeof(*bouton));
    if(bouton == NULL)
    {
        fprintf(stderr, "Impossible de creer le bouton :: BTN_CreateButtonWithTexture().\n");
        return NULL;
    }
    bouton->fontColor.r = 0;
    bouton->fontColor.g = 0;
    bouton->fontColor.b = 0;
    bouton->fontColor.a = 255;

    /*police du bouton*/
    if(font == NULL)
    {
        fprintf(stderr, "La police n'a pas été donne :: BTN_CreateButtonWithTexture().\n");
        return NULL;
    }
    else
    {
        bouton->fontName = malloc((strlen(font)+1)*sizeof(char));
        for(i = 0; i < strlen(font); i++)
            bouton->fontName[i] = font[i];
        bouton->fontName[i] = '\0';
        bouton->font = TTF_OpenFont(bouton->fontName, 12);
        if(bouton->font == NULL)
        {
            fprintf(stderr, "%s :: BTN_CreateButtonWithTexture()\n", TTF_GetError());
            return NULL;
        }
    }

    /*nom du bouton*/
    if(name == NULL)
    {
        fprintf(stderr, "Le nom n'a pas ete retrouver :: BTN_CreateButtonWithTexture().\n");
        return NULL;
    }
    bouton->name = malloc((strlen(name)+1)*sizeof(char));
    for(i = 0; i < strlen(name); i++)
        bouton->name[i] = name[i];
    bouton->name[i] = '\0';
    bouton->nameSurface = TTF_RenderText_Blended(bouton->font, bouton->name, bouton->fontColor);
    if(bouton->nameSurface == NULL)
    {
        fprintf(stderr, "Les surfaces du bouton n'ont pas pu etre creer :: BTN_CreateButtonWithTexture().\n");
        return NULL;
    }
    //Maintenant le rectangle qui contient le bouton:
    bouton->rect = SDL_malloc(sizeof(*bouton->rect));
    if(bouton->rect == NULL)
    {
        fprintf(stderr, "Le rectangle du bouton n'a pas pu etre creer :: BTN_CreateButtonWithTexture().\n");
        return NULL;
    }
    bouton->rect->x = x;
    bouton->rect->y = y;
    bouton->rect->w = w;
    bouton->rect->h = h;

    //Remplir la couleur de fond
    bouton->backgroundColor.r = 255;
    bouton->backgroundColor.g = 255;
    bouton->backgroundColor.b = 255;
    bouton->backgroundColor.a = 255;

    //On blite le nom du bouton dans la surface
    bouton->nameRect = SDL_malloc(sizeof(*bouton->nameRect));
    if(bouton->nameRect == NULL)
    {
        fprintf(stderr, "Le rectangle du nom n'a pas pu etre creer :: BTN_CreateButtonWithTexture().\n");
        return NULL;
    }
    if(bouton->nameSurface->w > bouton->rect->w)
        bouton->nameRect->w = bouton->rect->w;
    else
        bouton->nameRect->w = bouton->nameSurface->w;
    if(bouton->nameSurface->h > bouton->rect->h)
        bouton->nameRect->h = bouton->rect->h;
    else
        bouton->nameRect->h = bouton->nameSurface->h;
    bouton->nameRect->x = (bouton->rect->w/2) - (bouton->nameRect->w/2);
    bouton->nameRect->y = (bouton->rect->h/2) - (bouton->nameRect->h/2);

    //On donne une valeur aux autres sous-variables:
    //Le fond à rendre transparent:
    bouton->effectColor.r = 0;
    bouton->effectColor.g = 0;
    bouton->effectColor.b = 255;
    bouton->effectColor.a = 128;
    //Le type
    bouton->type = type;
    bouton->texture = NULL;
    return bouton;
}

SDL_Rect *BTN_CopyButton(BTN_Button *button, SDL_Renderer *renderer)
{
    SDL_Texture *target = SDL_GetRenderTarget(renderer);
    SDL_Texture *t = NULL;
    button->texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, button->rect->w, button->rect->h);
    if(button->texture == NULL)
    {
        fprintf(stderr, "BTN_CopyButton: can't create texture\n");
        return NULL;
    }
    SDL_SetTextureBlendMode(button->texture, SDL_BLENDMODE_BLEND);
    button->surface = SDL_CreateRGBSurface(SDL_SWSURFACE, button->rect->w, button->rect->h, 32, 0, 0, 0, 0);
    if(button->surface == NULL)
    {
        fprintf(stderr, "BTN_CopyButton: can't create surface\n");
        return NULL;
    }
    SDL_SetSurfaceBlendMode(button->surface, SDL_BLENDMODE_BLEND);
    SDL_FillRect(button->surface, NULL, SDL_MapRGB(button->surface->format, button->backgroundColor.r, button->backgroundColor.g, button->backgroundColor.b));
    SDL_SetSurfaceAlphaMod(button->surface, button->backgroundColor.a);
    SDL_BlitSurface(button->nameSurface, NULL, button->surface, button->nameRect);
    t = SDL_CreateTextureFromSurface(renderer, button->surface);
    SDL_FreeSurface(button->surface);
    SDL_SetRenderTarget(renderer, button->texture);
    SDL_RenderCopy(renderer, t, NULL, NULL);
    if(SDL_SetRenderDrawColor(renderer, button->effectColor.r, button->effectColor.g, button->effectColor.b, button->effectColor.a) < 0)
    {
        fprintf(stderr, "BTN_CopyButton: can't draw rectangle color\n");
        return NULL;
    }
    if (SDL_RenderDrawRect(renderer, NULL) < 0)
    {
        fprintf(stderr, "BTN_CopyButton: can't draw rectangle\n");
        return NULL;
    }
    SDL_SetRenderTarget(renderer, target);
    SDL_RenderCopy(renderer, button->texture, NULL, button->rect);
    SDL_RenderPresent(renderer);
    SDL_DestroyTexture(t);
    return button->rect;
}

void BTN_FreeButton(BTN_Button *button)
{
    if(button != NULL)
    {
        if(button->surface != NULL)
        {
            fprintf(stderr, "BTN_FreeButton(): liberation de la surface\n");
            if(button->texture == NULL)
                    SDL_FreeSurface(button->surface);
        }
        if(button->texture != NULL)
        {
            fprintf(stderr, "BTN_FreeButton(): liberation de la texture\n");
            SDL_DestroyTexture(button->texture);
        }
        if(button->nameSurface != NULL)
        {
            fprintf(stderr, "BTN_FreeButton(): liberation de la surface du nom\n");
            SDL_FreeSurface(button->nameSurface);
        }
        if(button->font != NULL)
        {
            fprintf(stderr, "BTN_FreeButton(): liberation de la police\n");
            TTF_CloseFont(button->font);
        }
        if(button->name != NULL)
        {
            fprintf(stderr, "BTN_FreeButton(): liberation du nom\n");
            free(button->name);
        }
        if(button->fontName != NULL)
        {
            fprintf(stderr, "BTN_FreeButton(): liberation du nom de la police utilisee\n");
            free(button->fontName);
        }
        if(button->nameRect != NULL)
        {
            fprintf(stderr, "BTN_FreeButton(): liberation du rectangle du nom\n");
            SDL_free(button->nameRect);
        }
        if(button->rect != NULL)
        {
            fprintf(stderr, "BTN_FreeButton(): liberation du rectangle de la surface\n");
            SDL_free(button->rect);
        }
        if(button != NULL)
        {
            fprintf(stderr, "BTN_FreeButton(): liberation de la memoire alloue pour le bouton\n");
            free(button);
        }
    }
}

BTN_Button *BTN_CreateButtonWithSurface(Uint8 type, int w, int h, int d, int x, int y, char *name, char *font)
{
    int i = 0;
    BTN_Button *bouton = malloc(sizeof(*bouton));
    if(bouton == NULL)
    {
        fprintf(stderr, "Impossible de creer le bouton :: BTN_CreateButtonWithSurface().\n");
        return NULL;
    }
    bouton->fontColor.r = 0;
    bouton->fontColor.g = 0;
    bouton->fontColor.b = 0;

    /*police du bouton*/
    if(font == NULL)
    {
        fprintf(stderr, "La police du bouton n'a pas ete trouver :: BTN_CreateButtonWithSurface().\n");
        return NULL;
    }
    else
    {
        bouton->fontName = malloc((strlen(font)+1)*sizeof(char*));
        for(i = 0; i < strlen(font); i++)
            bouton->fontName[i] = font[i];
        bouton->fontName[i] = '\0';
        bouton->font = TTF_OpenFont(bouton->fontName, 10);
        if(bouton->font == NULL)
        {
            fprintf(stderr, "La police du bouton n'a pas pu etre cree :: BTN_CreateButtonWithSurface().r\n");
            return NULL;
        }
    }
    /*nom du bouton*/
    if(name == NULL)
    {
        fprintf(stderr, "Le nom du bouton n'a pas ete trouver :: BTN_CreateButtonWithSurface().\n");
        return NULL;
    }
    bouton->name = malloc((strlen(name)+1)*sizeof(char));
    for(i = 0; i < strlen(name); i++)
        bouton->name[i] = name[i];
    bouton->name[i] = '\0';
    bouton->nameSurface = TTF_RenderText_Blended(bouton->font, bouton->name, bouton->fontColor);
    bouton->surface = SDL_CreateRGBSurface(SDL_SWSURFACE, w, h, d, 0, 0, 0, 0);
    SDL_SetSurfaceBlendMode(bouton->surface, SDL_BLENDMODE_BLEND);
    if(bouton->surface == NULL || bouton->nameSurface == NULL)
    {
        fprintf(stderr, "Les surfaces du bouton n'ont pas pu etre creer :: BTN_CreateButtonWithSurface().\n");
        return NULL;
    }

    {
        //Remplir la surface du bouton avec la couleur de fond
        bouton->backgroundColor.r = 255;
        bouton->backgroundColor.g = 255;
        bouton->backgroundColor.b = 255;
        bouton->backgroundColor.a = 255;
        SDL_FillRect(bouton->surface, NULL, SDL_MapRGBA(bouton->surface->format, 255, 255, 255, 255));
    }

    {
        //Maintenant la position de la surface:
        bouton->rect = SDL_malloc(sizeof(*bouton->rect));
        if(bouton->rect == NULL)
        {
            fprintf(stderr, "Le rectangle du bouton n'a pas pu etre creer :: BTN_CreateButtonWithSurface().\n");
            return NULL;
        }
        bouton->rect->x = x;
        bouton->rect->y = y;
        bouton->rect->w = w;
        bouton->rect->h = h;
    }

    {
        //On blite le nom du bouton dans la surface
        bouton->nameRect = SDL_malloc(sizeof(*bouton->nameRect));
        if(bouton->nameRect == NULL)
        {
            fprintf(stderr, "Le rectangle du nom du bouton n'a pas pu etre creer :: BTN_CreateButtonWithSurface().\n");
            return NULL;
        }
        bouton->nameRect->w = bouton->nameSurface->w;
        bouton->nameRect->h = bouton->nameSurface->h;
        bouton->nameRect->x = bouton->surface->w/2 - bouton->nameSurface->w/2;
        bouton->nameRect->y = bouton->surface->h/2 - bouton->nameSurface->h/2;
        SDL_BlitSurface(bouton->nameSurface, NULL, bouton->surface, bouton->nameRect);
    }

    {
        //On donne une valeur aux autres sous-variables:
        {
            //Le fond à rendre transparent:
            bouton->effectColor.r = 0;
            bouton->effectColor.g = 0;
            bouton->effectColor.b = 255;
            bouton->effectColor.a = 128;
        }
        bouton->type = type;
    }
    bouton->texture = NULL;

    return bouton;
}

int BTN_BlitButton(BTN_Button *button, SDL_Surface *surface)
{
    return SDL_BlitSurface(button->surface, NULL, surface, button->rect);
}

int BTN_SetBackgroudImg(BTN_Button *button, char* file, SDL_Window *window)
{
    SDL_Renderer *btn_renderer = SDL_GetRenderer(window);
    if(window != NULL)
    {
        if(btn_renderer == NULL)
        {
            SDL_FreeSurface(button->surface);
            button->surface = IMG_Load(file);
            if(button->surface == NULL)
                return 0;
            SDL_BlitSurface(button->nameSurface, NULL, button->surface, button->nameRect);
            SDL_BlitSurface(button->surface, NULL, SDL_GetWindowSurface(window), button->rect);
        }
        if(btn_renderer != NULL)
        {
            button->surface = IMG_Load(file);
            if(button->surface == NULL)
                return 0;
            SDL_BlitSurface(button->nameSurface, NULL, button->surface, button->nameRect);
            button->texture = SDL_CreateTextureFromSurface(btn_renderer, button->surface);
            SDL_RenderCopy(btn_renderer, button->texture, NULL, button->rect);
        }
    }
    else
        return 0;
    button->backgroundColor.a = 0;
    return 1;
}

void BTN_ChangeButtonType(BTN_Button *button, Uint8 newType)
{
    button->type = newType;
}

char *BTN_ChangeButtonName(BTN_Button *button, char *name, SDL_Window *window)
{
    SDL_Renderer *btn_renderer = SDL_GetRenderer(window);
    int btn_i = 0;
    if(window != NULL)
    {
        if(btn_renderer == NULL)
        {
            if(button->name != NULL)
                free(button->name);
            button->name = malloc((strlen(name)+1)*sizeof(char));
            while(btn_i < strlen(name))
            {
                button->name[btn_i] = name[btn_i];
                btn_i++;
            }
            button->name[btn_i] = '\0';
            SDL_FreeSurface(button->nameSurface);
            button->nameSurface = TTF_RenderText_Blended(button->font, button->name, button->fontColor);
            SDL_BlitSurface(button->nameSurface, NULL, button->surface, button->nameRect);
            SDL_BlitSurface(button->surface, NULL, SDL_GetWindowSurface(window), button->rect);
        }
        else
        {
            if(button->name != NULL)
                free(button->name);
            button->name = malloc((strlen(name)+1)*sizeof(char));
            while(btn_i < strlen(name))
            {
                button->name[btn_i] = name[btn_i];
                btn_i++;
            }
            button->name[btn_i] = '\0';
            button->nameSurface = TTF_RenderText_Blended(button->font, button->name, button->fontColor);
            SDL_BlitSurface(button->nameSurface, NULL, button->surface, button->nameRect);
            button->texture = SDL_CreateTextureFromSurface(btn_renderer, button->surface);
            SDL_RenderCopy(btn_renderer, button->texture, NULL, button->rect);
            SDL_FreeSurface(button->nameSurface);
            SDL_FreeSurface(button->surface);
        }
    }
    else
        return NULL;
    return button->name;
}

TTF_Font *BTN_ChangeButtonFont(BTN_Button *button, char *file, int fontSize, SDL_Window *window)
{
    if(window != NULL)
    {
        int btn_i = 0;
        if(button->nameSurface != NULL)
            SDL_FreeSurface(button->nameSurface);
        TTF_CloseFont(button->font);
        button->font = TTF_OpenFont(file, fontSize);
        button->nameSurface = TTF_RenderText_Blended(button->font, button->name, button->fontColor);
        if(button->nameSurface->w > button->rect->w)
        {
            button->nameRect->w = button->rect->w;
            if(button->type == BTN_DYNAMIC_BUTTON_FORM_A)
                button->type = BTN_DYNAMIC_BUTTON_FORM_B;
        }
        else
            button->nameRect->w = button->nameSurface->w;
        if(button->nameSurface->h > button->rect->h)
        {
            button->nameRect->h = button->rect->h;
            if(button->type == BTN_DYNAMIC_BUTTON_FORM_A)
                button->type = BTN_DYNAMIC_BUTTON_FORM_B;
        }
        else
            button->nameRect->h = button->nameSurface->h;
        free(button->fontName);
        button->fontName = malloc((strlen(file)+1)*sizeof(char));
        for(btn_i = 0; btn_i < strlen(file); btn_i++)
            button->fontName[btn_i] = file[btn_i];
        button->fontName[btn_i] = '\0';
        if(button->surface != NULL)
            SDL_FreeSurface(button->surface);
        button->surface = SDL_CreateRGBSurface(SDL_SWSURFACE, button->rect->w, button->rect->h, 32, 0, 0, 0, 0);
        SDL_SetSurfaceBlendMode(button->surface, SDL_BLENDMODE_BLEND);
        SDL_FillRect(button->surface, NULL, SDL_MapRGB(button->surface->format, button->backgroundColor.r, button->backgroundColor.g,
                     button->backgroundColor.b));
        button->nameRect->x = (button->rect->w/2 - button->nameRect->w/2);
        button->nameRect->y = (button->rect->h/2 - button->nameRect->h/2);
        SDL_BlitSurface(button->nameSurface, NULL, button->surface, button->nameRect);
        SDL_Renderer *btn_renderer = SDL_GetRenderer(window);
        if(btn_renderer == NULL)
        {
            SDL_BlitSurface(button->surface, NULL, SDL_GetWindowSurface(window), button->rect);
        }
        else
        {
            SDL_DestroyTexture(button->texture);
            button->nameRect->x = button->rect->x + (button->rect->w/2 - button->nameRect->w/2);
            button->nameRect->y = button->rect->y + (button->rect->h/2 - button->nameRect->h/2);
            button->texture = SDL_CreateTextureFromSurface(btn_renderer, button->surface);
            SDL_RenderCopy(btn_renderer, button->texture, NULL, button->rect);
            SDL_FreeSurface(button->surface);
            SDL_FreeSurface(button->nameSurface);
        }
    }
    else
        return NULL;
    return button->font;
}

int BTN_ChangeFontColor(BTN_Button *button, Uint8 r, Uint8 g, Uint8 b, Uint8 a, SDL_Window *window)
{
    SDL_Renderer *btn_renderer = SDL_GetRenderer(window);
    button->fontColor.r = r;
    button->fontColor.g = g;
    button->fontColor.b = b;
    button->fontColor.a = a;
    if(window != NULL)
    {
        if(button->nameSurface == NULL)
            SDL_FreeSurface(button->nameSurface);
        button->nameSurface = TTF_RenderText_Blended(button->font, button->name, button->fontColor);
        if(button->surface == NULL)
            SDL_FreeSurface(button->surface);
        button->surface = SDL_CreateRGBSurface(SDL_SWSURFACE, button->rect->w, button->rect->h, 32, 0, 0, 0, 0);
        SDL_BlitSurface(button->nameSurface, NULL, button->surface, button->nameRect);
        if(btn_renderer != NULL)
        {
            SDL_DestroyTexture(button->texture);
            button->texture = SDL_CreateTextureFromSurface(btn_renderer, button->surface);
            if(SDL_RenderCopy(btn_renderer, button->texture, NULL, button->rect) < 0)
                return 0;
            return 1;
        }
        else
        {
            if(SDL_BlitSurface(button->nameSurface, NULL, SDL_GetWindowSurface(window), button->nameRect) < 0)
                return 0;
            else
                return 1;
        }
    }
    return 0;
}

int BTN_ChangeBackgroundColor(BTN_Button *button, Uint8 r, Uint8 g, Uint8 b, Uint8 a, SDL_Window *window)
{
    SDL_Renderer *btn_renderer = SDL_GetRenderer(window);
    button->backgroundColor.a = a;
    if(a == 0 && btn_renderer != NULL)
    {
        SDL_GetRGB(button->rect->x*button->rect->y, SDL_GetWindowSurface(window)->format, &r, &g, &b);
    }
    button->backgroundColor.r = r;
    button->backgroundColor.g = g;
    button->backgroundColor.b = b;
    if(btn_renderer != NULL)
    {
        SDL_Rect btn_rect = {button->rect->w/2-button->nameRect->w, button->rect->h/2-button->nameRect->h, button->rect->w, button->rect->h};
        SDL_Texture *btn_t = SDL_CreateTextureFromSurface(btn_renderer, button->nameSurface);
        SDL_SetRenderTarget(btn_renderer, button->texture);
        SDL_SetRenderDrawColor(btn_renderer, r, g, b, a);
        SDL_RenderFillRect(btn_renderer, NULL);
        SDL_RenderCopy(btn_renderer, btn_t, NULL, &btn_rect);
        SDL_SetRenderTarget(btn_renderer, NULL);
        SDL_RenderCopy(btn_renderer, button->texture, NULL, button->rect);
        SDL_DestroyTexture(btn_t);
    }
    else
    {
        SDL_FillRect(button->surface, NULL, SDL_MapRGB(SDL_GetWindowSurface(window)->format, r, g, b));
        SDL_BlitSurface(button->nameSurface, NULL, button->surface, button->nameRect);
        SDL_BlitSurface(button->surface, NULL, SDL_GetWindowSurface(window), button->rect);
    }
    if(button->backgroundColor.r == r && button->backgroundColor.g == g && button->backgroundColor.b == b && button->backgroundColor.a == a)
        return 1;
    else
        return 0;
}

int BTN_ChangeEffectColor(BTN_Button *button, Uint8 r, Uint8 g, Uint8 b, Uint8 a, SDL_Renderer *renderer)
{
    button->effectColor.r = r;
    button->effectColor.g = g;
    button->effectColor.b = b;
    button->effectColor.a = a;
    if(renderer != NULL)
    {
        SDL_SetRenderDrawColor(renderer, button->backgroundColor.r, button->backgroundColor.g, button->backgroundColor.b, 255);
        SDL_RenderDrawRect(renderer, button->rect);
        SDL_SetRenderDrawColor(renderer, r, g, b, 255);
        SDL_RenderDrawRect(renderer, button->rect);
    }
    if(button->effectColor.r == r && button->effectColor.g == g && button->effectColor.b == b && button->effectColor.a == a)
        return 1;
    else
        return 0;
}

void BTN_ChangeButtonRect(SDL_Renderer *render, SDL_Surface *surface, BTN_Button *button, int x, int y, int w, int h)
{
    if(button != NULL)
    {
        if(x >= 0)
            button->rect->x = x;
        if(y >= 0)
            button->rect->y = y;
        if(w >= 0)
            button->rect->w = w;
        if(h >= 0)
            button->rect->h = h;
        if(render != NULL)
        {
            SDL_RenderFillRect(render, button->rect);
            SDL_RenderCopy(render, button->texture, NULL, button->rect);
        }
        if(surface != NULL)
        {
            SDL_FillRect(surface, button->rect, SDL_MapRGB(surface->format, button->backgroundColor.r, button->backgroundColor.g,
                         button->backgroundColor.b));
            BTN_BlitButton(button, surface);
        }
    }
}

char* BTN_Stradd(const char *chaine1, const char *chaine2)
{
    char *chaine = malloc((strlen(chaine1)+strlen(chaine2)+1) * sizeof(char));
    strcat(chaine, chaine1);
    strcat(chaine, chaine2);
    return chaine;
}

int BTN_SurfaceMouseMotion(BTN_Button *button, SDL_Event event, SDL_Surface *surface)
{
    if(button == NULL)
        return 0;
    if(BTN_CURSOR_MOVE_IN(button, event))
    {
        SDL_Surface *btn_surface = SDL_CreateRGBSurface(SDL_SWSURFACE, button->rect->w, button->rect->h, 32, 0, 0, 0, 0);
        if(button->type == BTN_DYNAMIC_BUTTON_FORM_A || button->type == BTN_DYNAMIC_BUTTON_FORM_B)
        {
            int btn_taillePolice = 0;
            TTF_Font *btn_font =  NULL;
            if(button->type == BTN_DYNAMIC_BUTTON_FORM_A)
                btn_taillePolice = TTF_FontAscent(button->font)+2;
            if(button->type == BTN_DYNAMIC_BUTTON_FORM_B)
                btn_taillePolice = TTF_FontAscent(button->font)-2;
            btn_font = TTF_OpenFont(button->fontName, btn_taillePolice);
            /*On agrandi ou reduit le nom*/

            SDL_FillRect(btn_surface, NULL, SDL_MapRGB(button->surface->format, button->backgroundColor.r, button->backgroundColor.g,
                         button->backgroundColor.b));
            SDL_Surface *btn_s = TTF_RenderText_Blended(btn_font, button->name, button->fontColor);
            //On recentre le texte agrandi
            SDL_Rect btn_r = {button->surface->w/2 - btn_s->w/2, button->surface->h/2 - btn_s->h/2,
                              btn_s->w, btn_s->h
                             };
            SDL_BlitSurface(btn_s, NULL, btn_surface, &btn_r);
            SDL_BlitSurface(btn_surface, NULL, surface, button->rect);
            TTF_CloseFont(btn_font);
            SDL_FreeSurface(btn_surface);
            SDL_FreeSurface(btn_s);
        }
        else
        {
            SDL_BlitSurface(button->surface, NULL, surface, button->rect);
        }
        btn_surface = SDL_CreateRGBSurface(SDL_SWSURFACE, button->rect->w, button->rect->h, 32, 0, 0, 0, 0);
        SDL_SetSurfaceBlendMode(btn_surface, SDL_BLENDMODE_BLEND);
        SDL_FillRect(btn_surface, NULL, SDL_MapRGB(button->surface->format, button->effectColor.r, button->effectColor.g,
                     button->effectColor.b));
        SDL_SetSurfaceAlphaMod(btn_surface, button->effectColor.a);
        SDL_BlitSurface(btn_surface, NULL, surface, button->rect);
        SDL_FreeSurface(btn_surface);
        return 1;
    }
    else
    {
        SDL_BlitSurface(button->surface, NULL, surface, button->rect);
        return 0;
    }
}

int BTN_SurfaceMouseClic(BTN_Button *button, SDL_Event event, SDL_Surface *surface)
{
    if(button == NULL)
        return 0;

    if(BTN_CURSOR_CLIC_IN(button, event))
    {
        while(event.type != SDL_MOUSEBUTTONUP)
        {
            BTN_SurfaceMouseMotion(button, event, surface);
            SDL_PollEvent(&event);
        }
        //une fois le bouton de la souris relaché,
        //on vérifie si le curseur est toujours sur la surface du bouton
        //Pour ça, on envoie le bouton et 'event' à SDL_SurfaceMouseClicUp
        return BTN_SurfaceMouseClicUp(button, event, surface);
    }
    else
        return 0;
}

int BTN_SurfaceMouseClicDown(BTN_Button *button, SDL_Event event, SDL_Surface *surface)
{
    if(button == NULL)
        return 0;
    if(event.button.x >= button->rect->x && event.button.y >= button->rect->y && event.button.x <= button->rect->x + button->surface->w && event.button.y <= button->rect->y + button->surface->h)
    {
        if(button->type == BTN_DYNAMIC_BUTTON_FORM_A || button->type == BTN_DYNAMIC_BUTTON_FORM_B)
            SDL_BlitSurface(button->surface, NULL, surface, button->rect);
        SDL_Surface *btn_surface = SDL_CreateRGBSurface(SDL_SWSURFACE, button->rect->w, button->rect->h, 32, 0, 0, 0, 0);
        SDL_SetSurfaceBlendMode(btn_surface, SDL_BLENDMODE_BLEND);
        SDL_FillRect(btn_surface, NULL, SDL_MapRGB(button->surface->format, button->effectColor.r, button->effectColor.g,
                     button->effectColor.b));
        SDL_SetSurfaceAlphaMod(btn_surface, button->effectColor.a);
        SDL_BlitSurface(btn_surface, NULL, surface, button->rect);
        SDL_FreeSurface(btn_surface);
        return 1;
    }
    else
        return 0;
}

int BTN_SurfaceMouseClicUp(BTN_Button *button, SDL_Event event, SDL_Surface *surface)
{
    if(button == NULL)
        return 0;
    if(event.button.x >= button->rect->x && event.button.y >= button->rect->y && event.button.x <= button->rect->x + button->surface->w && event.button.y <= button->rect->y + button->surface->h)
        return BTN_SurfaceMouseMotion(button, event, surface);
    else
        return 0;
}

int BTN_RenderMouseMotion(BTN_Button *button, SDL_Event event, SDL_Renderer *renderer)
{
    if(button == NULL)
        return 0;
    SDL_Texture *t = NULL;
    if(BTN_CURSOR_MOVE_IN(button, event))
    {
        if(button->type == BTN_DYNAMIC_BUTTON_FORM_A || button->type == BTN_DYNAMIC_BUTTON_FORM_B)
        {
            int ptsize = 0;
            if(button->type == BTN_DYNAMIC_BUTTON_FORM_A)
                ptsize = TTF_FontAscent(button->font)+2;
            else if(button->type == BTN_DYNAMIC_BUTTON_FORM_B)
                ptsize = TTF_FontAscent(button->font)-2;
            TTF_Font *police = TTF_OpenFont(button->fontName, ptsize);
            SDL_Surface *s = TTF_RenderText_Blended(police, button->name, button->fontColor);
            SDL_Rect r = {(button->rect->w/2)-(s->w/2), (button->rect->h/2)-(s->h/2), s->w, s->h};
            button->surface = SDL_CreateRGBSurface(SDL_SWSURFACE, button->rect->w, button->rect->h, 32, 0, 0, 0, 0);
            SDL_FillRect(button->surface, NULL, SDL_MapRGBA(button->surface->format, button->backgroundColor.r,
                         button->backgroundColor.g, button->backgroundColor.b, button->backgroundColor.a));
            SDL_BlitSurface(s, NULL, button->surface, &r);
            t = SDL_CreateTextureFromSurface(renderer, button->surface);
            SDL_RenderCopy(renderer, t, NULL, button->rect);
            SDL_RenderPresent(renderer);
        }
        else
        {
            SDL_RenderCopy(renderer, button->texture, NULL, button->rect);
        }
        button->surface = SDL_CreateRGBSurface(SDL_SWSURFACE, button->rect->w, button->rect->h, 32, 0, 0, 0, 0);
        SDL_SetSurfaceBlendMode(button->surface, SDL_BLENDMODE_BLEND);
        SDL_FillRect(button->surface, NULL, SDL_MapRGB(button->surface->format, button->effectColor.r, button->effectColor.g,
                     button->effectColor.b));
        SDL_SetSurfaceAlphaMod(button->surface, button->effectColor.a);
        t = SDL_CreateTextureFromSurface(renderer, button->surface);
        SDL_RenderCopy(renderer, t, NULL, button->rect);
        SDL_RenderPresent(renderer);
        SDL_DestroyTexture(t);
        SDL_FreeSurface(button->surface);
        return 1;
    }
    else
    {
        SDL_RenderCopy(renderer, button->texture, NULL, button->rect);
        SDL_RenderPresent(renderer);
        SDL_DestroyTexture(t);
        return 0;
    }
}

int BTN_RenderMouseClicDown(BTN_Button *button, SDL_Event event, SDL_Renderer *renderer)
{
    if(button == NULL)
        return 0;
    if(BTN_CURSOR_CLIC_IN(button, event) && event.type == SDL_MOUSEBUTTONDOWN)
    {
        if(button->type == BTN_DYNAMIC_BUTTON_FORM_A && button->type == BTN_DYNAMIC_BUTTON_FORM_B)
        {
            SDL_RenderCopy(renderer, button->texture, NULL, button->rect);
            SDL_RenderPresent(renderer);
        }
        return 1;
    }
    else
        return 0;
}

int BTN_RenderMouseClicUp(BTN_Button *button, SDL_Event event, SDL_Renderer *renderer)
{
    if(button == NULL)
        return 0;
    if(BTN_CURSOR_CLIC_IN(button, event) && event.type == SDL_MOUSEBUTTONUP)
    {
        return BTN_RenderMouseMotion(button, event, renderer);
    }
    else
        return 0;
}

int BTN_RenderMouseClic(BTN_Button *button, SDL_Event event, SDL_Renderer *renderer)
{
    if(button == NULL)
        return 0;
    if(!BTN_RenderMouseClicDown(button, event, renderer))
        return 0;
    else
        return BTN_RenderMouseClicUp(button, event, renderer);
}
