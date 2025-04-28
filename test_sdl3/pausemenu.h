#pragma once
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>



struct PauseMenu
{
	SDL_Texture* offuscateTexture;
	SDL_FRect* offuscate;
	SDL_Renderer* renderer;
	int windowheight;
	int windowwidth;
	void init(SDL_Renderer* renderer, int windowheight, int windowwidth) {
		this->renderer = renderer;
		this->windowwidth = windowwidth;
		this->windowheight = windowheight;
		offuscate = new SDL_FRect{ 0,0,(float)windowwidth,(float)windowheight};
	}
	void update() {

	}
	void render() {
		SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
		//SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		SDL_RenderFillRect(renderer, offuscate);
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 100);
		SDL_RenderFillRect(renderer, offuscate);
		SDL_RenderPresent(renderer);
	}

	void createTxture() {
		offuscateTexture= SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, windowwidth / 4, windowheight / 4);
		
		// 1. Set render target to smallTexture
		SDL_SetRenderTarget(renderer, offuscateTexture);


	}
};
