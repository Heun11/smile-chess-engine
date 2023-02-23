#ifndef TOOLS_H
#define TOOLS_H

#include<stdlib.h>
#include<stdio.h>
#include<time.h>
#include"sdl.h"
#include"main.h"

int SRAND_CALL;

SDL_Rect TOOLS_Get_Image_Src(int x, int y, int t_w, int t_h);
void TOOLS_Render_Image_From_Texture(SDL_Renderer* rend, SDL_Texture* tex, SDL_Rect* src, int x, int y, int w, int h);

void TOOLS_SDL_Text_RenderCopy(SDL_Renderer* r, TTF_Font* f, char* s, int x, int y, int w, int h, SDL_Color c);
int TOOLS_Random_Number(int start, int stop);
float TOOLS_Random_Float(float stop);
int TOOLS_Collide_Rect(SDL_Rect r1, SDL_Rect r2);

void get_error();

#endif
