#ifndef MAIN_H
#define MAIN_H

#include"tools.h"
#include"sdl.h"
#include"stdio.h"
#include"stdlib.h"
#include <string.h>
#include <ctype.h>
#include"math.h"

#include"move_log.h"

extern SDL_Renderer* rend;
extern SDL_Texture* tex;
extern TTF_Font* font;
extern MOVE_LOG_Log MoveLog;

extern int SCREEN_WIDTH;
extern int SCREEN_HEIGHT;
extern float elapsed;

#include"chess.h"

#endif