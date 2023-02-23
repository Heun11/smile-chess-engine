#ifndef MAIN_H
#define MAIN_H

#include"tools.h"
#include"sdl.h"
#include"stdio.h"
#include"stdlib.h"
#include"math.h"

extern SDL_Renderer* rend;
extern SDL_Texture* tex;
extern TTF_Font* font;

extern int SCREEN_WIDTH;
extern int SCREEN_HEIGHT;
extern float elapsed;

#include"chess.h"

#endif