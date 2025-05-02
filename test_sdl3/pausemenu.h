#pragma once
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>



struct PauseMenu
{
	SDL_Texture* PauseText_texture;
	SDL_FRect* offuscate;
	SDL_FRect* PauseText_Rect;
	SDL_Renderer* renderer;
	int windowheight;
	int windowwidth;
	int times = 0;
	void init(SDL_Renderer* renderer, int windowheight, int windowwidth) {
		this->renderer = renderer;
		this->windowwidth = windowwidth;
		this->windowheight = windowheight;
		offuscate = new SDL_FRect{ 0,0,(float)windowwidth,(float)windowheight};
		PauseText_Rect = new SDL_FRect{ (float)windowwidth/2-150,(float)windowheight/2-100,300,200 };
	}
	void update() {

		if (times == 0) {
			std::cout << "aggiornato pauseMenu--";
			createTesto("paused", &PauseText_texture, 255, 255, 255, 255);
			times = 1;
		}

	}
	void render() {
		SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		SDL_RenderTexture(renderer,PauseText_texture,NULL,PauseText_Rect);
		SDL_SetRenderDrawColor(renderer, 22, 22, 22, 2);
		SDL_RenderFillRect(renderer, offuscate);
		SDL_RenderPresent(renderer);
	}
	void createTesto(const char* testo, SDL_Texture** target, int r, int g, int b, int t) {
		TTF_Font* font = TTF_OpenFont("./fonts/EthnocentricRgIt.otf", 24);
		SDL_Surface* surfaceText = TTF_RenderText_Solid(font, testo, 0, SDL_Color{ (Uint8)r,(Uint8)g,(Uint8)b,(Uint8)t });
		*target = SDL_CreateTextureFromSurface(renderer, surfaceText);
		TTF_CloseFont(font);
		SDL_DestroySurface(surfaceText);
	}
	
};
