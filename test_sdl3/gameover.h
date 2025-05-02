#pragma once
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <iostream>
#include "./oggetti.h"

struct GameOverMenu {
	SDL_Texture* gameOverText_texture;
	SDL_FRect* GameOverText_rect;
	SDL_Renderer* renderer;
	SDL_FRect* offuscate;
	Players* player1;
	Players* player2;
	std::string winner;
	int windowheight;
	int windowwidth;
	int times = 0;
	void init(SDL_Renderer* renderer, int windowheight, int windowwidth,Players* player1,Players* player2) {
		this->renderer = renderer;
		this->windowwidth = windowwidth;
		this->windowheight = windowheight;
		GameOverText_rect = new SDL_FRect{ (float)windowwidth / 2 - 300,(float)windowheight / 2 - 200,300,200 };
		offuscate = new SDL_FRect{ 0,0,(float)windowwidth,(float)windowheight };
		this->player1 = player1;
		this->player2 = player2;
	}
	void update() {

		if (times == 0) {
			std::cout << "aggiornato gameover--";

			
			times = 1;
		}

	}

	//this function is called to decide wich name to display
	void DetermineDisplayNamePlayer(int numPlayer) {
		switch (numPlayer) {
		case 1:
			//player 1 won
			createTesto(("il vincitore:"+player1->NomeUtente).c_str(), &gameOverText_texture, 255, 255, 255, 255);
			break;
		case 2:
			//player 2 won
			createTesto(("il vincitore:" + player2->NomeUtente).c_str(), &gameOverText_texture, 255, 255, 255, 255);
			break;
		}
	}

	void render() {
		SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		SDL_RenderTexture(renderer, gameOverText_texture, NULL, GameOverText_rect);
		SDL_SetRenderDrawColor(renderer, 22, 22, 22, 2);
		SDL_RenderFillRect(renderer, offuscate);
		SDL_RenderPresent(renderer);
	}
	void createTesto(const char* testo, SDL_Texture** target, int r, int g, int b, int t) {
		TTF_Font* font = TTF_OpenFont("./fonts/EthnocentricRgIt.otf", 24);
		SDL_Surface* surfaceText = TTF_RenderText_Solid(font, testo, 0, SDL_Color{ (Uint8)r,(Uint8)g,(Uint8)b,(Uint8)t });
		*target = SDL_CreateTextureFromSurface(renderer, surfaceText);
		//scale the textBox according to the needed size
		GameOverText_rect->w=30* strlen(testo);
		GameOverText_rect->x = (windowwidth / 2) - (GameOverText_rect->w / 2);
		TTF_CloseFont(font);
		SDL_DestroySurface(surfaceText);
	}

};