#pragma once
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <iostream>
#include <fstream>
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
	std::string jsonPath;
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
		jsonPath = "./data/score.json";
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
			//createJsonFile(jsonPath);
			appendPlayer("francesco", 23);
			appendPlayer("ugo", 43);


			times = 1;
		}
	}
	//this function is called when are sure that there is no json file and it creates it in the path that it gives to me
	void createJsonFile(std::string path) {
		//create/open file
		std::ofstream myfile(path);
		if (!myfile) {
			std::cout << "tutto é andato a puttane" << std::endl;
		}
		else
		{
			myfile.close();
			std::cout << "file creato" << std::endl;
		}
	}
	//this funciton is called when i need to append a player in the file
	void appendPlayer(std::string nome, int punteggio) {
		std::ifstream file(jsonPath);
		json data;

		if (file) {
			try {
				file >> data;
				if (!data.is_array()) {
					// If not an array, reset to empty array
					data = json::array();
				}
			}
			catch (const json::parse_error& e) {
				// If parse error, reset to empty array
				data = json::array();
			}
		}
		else {
			// File doesn't exist, create empty array
			data = json::array();
		}
		file.close();

		// Append new player entry
		data.push_back({ {"name", nome}, {"score", punteggio} });

		// Write updated JSON back to the file (overwrite)
		std::ofstream myfile(jsonPath);
		if (!myfile) {
			std::cerr << "Error opening file for writing\n";
			return;
		}
		myfile << data.dump(4);  // pretty print with indentation
		myfile.close();

		std::cout << "Player added successfully\n";
	}
	//this function si called when i need to modify a specific player data given in input
	//this function is called when i need to delte them from the fi
};