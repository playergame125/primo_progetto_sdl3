#pragma once
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <iostream>
#include "json.hpp"

struct Startmenu {
	using json = nlohmann::json;
	
	int top_score;
	SDL_Renderer* renderer;
	SDL_FRect* boxUtente;
	SDL_FRect* nome_utenteBox;
	SDL_Texture* textureNomeUtente1;
	SDL_Texture* textureNomeUtente2;
	SDL_Texture* premiSpazioTexture;
	SDL_FRect* nomePlayer1;
	std::string nomeUtente1;
	SDL_FRect* nomePlayer2;
	std::string nomeUtente2;
	SDL_FRect* premiSpazio;
	int windowWidth;
	int windowHeight;
	int times = 0;
	void init(SDL_Renderer* input,int windowheight,int windowwidth) {
		renderer = input;
		windowHeight = windowheight;
		windowWidth = windowwidth;
		boxUtente = new SDL_FRect{ 0, 0, 250, 150 };
		nomePlayer1 = new SDL_FRect{ 0, 0, 250, 75 };
		nomePlayer2 = new SDL_FRect{ 0, 75, 250, 75 };
		premiSpazio = new SDL_FRect{((float)windowWidth-600)/2 ,(float)windowHeight - 100,600,75 };
		nomeUtente1 = "matteo";
		nomeUtente2 = "francesco";
	}

	void RenderStartMenu() {
		//this function will be called from the main when the game is the start menu state
		SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
		SDL_RenderClear(renderer);
		//render utente box edge
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 170);
		SDL_RenderRect(renderer, boxUtente);
		//render first player name
		SDL_RenderTexture(renderer, textureNomeUtente1, NULL, nomePlayer1);
		//render second player name
		SDL_RenderTexture(renderer, textureNomeUtente2, NULL, nomePlayer2);
		//render premi spazio per giocare
		SDL_RenderTexture(renderer, premiSpazioTexture, NULL, premiSpazio);
		SDL_RenderPresent(renderer);


	}

	 void createTesto(const char* testo,SDL_Texture** player,int r,int g,int b,int t) {
		TTF_Font* font = TTF_OpenFont("./fonts/EthnocentricRgIt.otf", 24);
		SDL_Surface* surfaceText= TTF_RenderText_Solid(font, testo, 0, SDL_Color{ (Uint8)r,(Uint8)g,(Uint8)b,(Uint8)t });
		*player= SDL_CreateTextureFromSurface(renderer,surfaceText);
		TTF_CloseFont(font);
		SDL_DestroySurface(surfaceText);

	 }

	void  UpdateStartMenu() {
		//this function will be called from the main when the game is the start menu state
		if (times == 0) {
			std::cout << "aggiornato nome--";
			//insert load from file to test

			createTesto(nomeUtente1.c_str(), &textureNomeUtente1, 255, 255, 255, 255);
			createTesto(nomeUtente2.c_str(), &textureNomeUtente2, 255, 255, 255, 255);
			createTesto("premi spazio per giocare", &premiSpazioTexture, 127, 127, 127, 255);
			times = 1;
		}
	}
};