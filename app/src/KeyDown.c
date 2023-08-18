#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <SDL2/SDL.h>

char GetCharPress(SDL_Event event, char *text)
{
    char c = '\0';
    if(SDL_GetModState())
    {
        c = '\0';
        if((SDL_GetModState()&(KMOD_CAPS|KMOD_SHIFT|KMOD_LSHIFT|KMOD_RSHIFT)) && event.key.keysym.sym != SDLK_CAPSLOCK && event.key.keysym.sym != SDLK_LSHIFT)
        {
            c = toupper((int)event.key.keysym.sym);
            if(SDL_GetModState()&(KMOD_SHIFT|KMOD_LSHIFT|KMOD_RSHIFT) && SDL_GetModState()&KMOD_CAPS)
                c = tolower(c);
            else
            {
                if(c == ',')
                    c = '?';
                if(c == ';')
                    c = '.';
                if(c == ':')
                    c = '/';
                if(c == '!')
                    c = '§';
                if(c == '^')
                    c = '¨';
                if(c == '$')
                    c = '£';
                if(c == 'ù')
                    c = '%';
                if(c == '*')
                    c = 'µ';
            }
            c = (char)c;
        }
        else if((SDL_GetModState()&KMOD_NUM) && event.key.keysym.sym != SDLK_NUMLOCKCLEAR)
            c = (char)event.key.keysym.sym;
    }
    else
    {
        if(event.key.keysym.sym == SDLK_CAPSLOCK || event.key.keysym.sym == SDLK_BACKSPACE || event.key.keysym.sym == SDLK_RETURN || event.key.keysym.sym == SDLK_NUMLOCKCLEAR)
            c = '\0';
        else
        {
            c = (char)event.key.keysym.sym;
            if(c == '1')
                c = '&';
            if(c == '2')
                c = 'é';
            if(c == '3')
                c = '"';
            if(c == '4')
                c = '\'';
            if(c == '5')
                c = '(';
            if(c == '6')
                c = '-';
            if(c == '7')
                c = 'è';
            if(c == '8')
                c = '_';
            if(c == '9')
                c = 'ç';
            if(c == '0')
                c = 'à';
            if(c == '°')
                c = ')';
            if(c == '+')
                c = '=';
        }
    }

    if(text != NULL)
    {
        text[strlen(text)] = c;
        text[strlen(text)+1] = '\0';
    }
    return c;
}
