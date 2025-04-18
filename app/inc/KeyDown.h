#ifndef KEYDOWN_H_INCLUDED
#define KEYDOWN_H_INCLUDED

#include <SDL2/SDL.h>

typedef enum CharType {
    Alpha, AlphaNum, Digit, Space, All
} CharType;
char GetCharPress(SDL_Event event, char *text);

#endif // KEYDOWN_H_INCLUDED

