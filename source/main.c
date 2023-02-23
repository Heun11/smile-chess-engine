#include<stdio.h>
#include<math.h>

#include"main.h"

SDL_Renderer* rend;
SDL_Texture* tex;
TTF_Font* font;
int SCREEN_WIDTH;
int SCREEN_HEIGHT;
float elapsed;

int main(int argc, char** argv)
{
	if(SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER|SDL_INIT_AUDIO)!=0){
		SDL_Quit();
		return 1;
	}
	
	SDL_DisplayMode DM;
	SDL_GetCurrentDisplayMode(0, &DM);
	SCREEN_WIDTH = DM.w;
	SCREEN_HEIGHT = DM.h;
	SCREEN_WIDTH = 800;
	SCREEN_HEIGHT = 600;
	SDL_Window* win = SDL_CreateWindow(":)", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
	if(!win){
		SDL_Quit();
		return 1;
	}

	Uint32 render_flags = SDL_RENDERER_ACCELERATED|SDL_RENDERER_PRESENTVSYNC;
	rend = SDL_CreateRenderer(win, -1, render_flags);
	if(!rend){
		SDL_DestroyWindow(win);
		SDL_Quit();
		return 1;
	}
	SDL_SetRenderDrawBlendMode(rend, SDL_BLENDMODE_BLEND);

	TTF_Init();
	font = TTF_OpenFont("resources/vogue.ttf", 50);
	SDL_Color text_color = {252, 3, 215};

	int key;
	elapsed = 0.016f;
	char fps_str[10];
	int fps = 60;

	SDL_Surface* tileset = IMG_Load("resources/chess.png");
	tex = SDL_CreateTextureFromSurface(rend, tileset);
	SDL_FreeSurface(tileset);

	CHESS_LoadPieces();

	int run = 1;
	while(run){

		Uint64 start = SDL_GetPerformanceCounter();

		SDL_Event event;
		while(SDL_PollEvent(&event)){
			switch(event.type){
				case SDL_QUIT:
					run = 0;
					break;
				case SDL_KEYDOWN:
					key = event.key.keysym.scancode;
					switch(key){
						case SDL_SCANCODE_ESCAPE:
							run = 0;
							break;
						default:
							break;
					}
					break;
				case SDL_KEYUP:
					key = event.key.keysym.scancode;
					switch(key){
						default:
							break;
					}
					break;
			}
		}

		SDL_RenderClear(rend);

		CHESS_DrawBoardAndPieces();
		CHESS_MovePiece();

		fps = (int)(1.0f/elapsed);
		snprintf(fps_str, 10, "fps:%d", fps);
		TOOLS_SDL_Text_RenderCopy(rend, font, fps_str, SCREEN_WIDTH-150, SCREEN_HEIGHT-80, 150, 80, text_color);
        SDL_SetRenderDrawColor(rend, 0, 0, 0, 255);
		SDL_RenderPresent(rend);
	
		Uint64 end = SDL_GetPerformanceCounter();
		elapsed = (end - start) / (float)SDL_GetPerformanceFrequency();

	}
	
	SDL_DestroyRenderer(rend);
	SDL_DestroyWindow(win);
	SDL_Quit();
	TTF_Quit();
	return 0;
}