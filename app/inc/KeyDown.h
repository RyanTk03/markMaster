#ifndef KEYDOWN_H_INCLUDED
#define KEYDOWN_H_INCLUDED

typedef enum CharType CharType;
enum CharType{
    Alpha, AlphaNum, Digit, Space, All
};
char GetCharPress(SDL_Event event, char *text);

#endif // KEYDOWN_H_INCLUDED
