#include"tools.h"

int TOOLS_Random_Number(int start, int stop)
{
	if(SRAND_CALL!=-1){
		srand(time(0));
		SRAND_CALL = -1;
	}
	return (rand()%(stop-start+1))+start;
}

float TOOLS_Random_Float(float stop)
{
	if(SRAND_CALL!=-1){
		srand(time(0));
		SRAND_CALL = -1;
	}
	return ((float)rand()/RAND_MAX)*stop;
}


void get_error()
{
	printf("Error with sdl: %s\n", SDL_GetError());
	fflush(stdout);
	exit(0);
}

void TOOLS_SDL_Text_RenderCopy(SDL_Renderer* r, TTF_Font* f, char* s, int x, int y, int w, int h, SDL_Color c)
{
	SDL_Surface* surf = TTF_RenderText_Blended_Wrapped(f, s, c, w);
	SDL_Texture* t = SDL_CreateTextureFromSurface(r, surf);
	SDL_FreeSurface(surf);
	SDL_RenderCopy(r, t, NULL, &(SDL_Rect){x, y, w, h});
	SDL_DestroyTexture(t);
}

SDL_Rect TOOLS_Get_Image_Src(int x, int y, int t_w, int t_h)
{
	SDL_Rect r = {x*t_w, y*t_h, t_w, t_h};
	return r;
}

void TOOLS_Render_Image_From_Texture(SDL_Renderer* rend, SDL_Texture* tex, SDL_Rect* src, int x, int y, int w, int h)
{
	SDL_Rect dest = {x, y, w, h};
	SDL_RenderCopy(rend, tex, src, &dest);
}

int TOOLS_Collide_Rect(SDL_Rect r1, SDL_Rect r2)
{
	return r1.x<r2.x+r2.w &&
		r1.x+r1.w>r2.x &&
		r1.y<r2.y+r2.h &&
		r1.y+r1.h>r2.y;
}
