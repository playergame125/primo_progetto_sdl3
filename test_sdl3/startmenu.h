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
			//appendPlayer("francesco", 23);
			//appendPlayer("ugo", 43);
			//std::cout<<doesItExist("lollo");
			//std::cout <<doesItExist("laura");
			updatePlayerData("lorenso", "laura");
			std::cout << "output get info=" << getInfoPlayer("laura");
			std::cout << "score di matteo" << getInfoPlayer("matteo", true);

			//updatePlayerData("sugo", 2);


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

	//this funciton is called when in need to know if a player exists
	bool doesItExist(std::string nome){
		bool result=false;
		//i open the file
		std::ifstream file(jsonPath);
		//variabile temporanea per contenuti del json
		json temp;
		//i put the stream from the file into the variable
		file >> temp;//it autmatically parse it
		for (int x = 0; x < temp.size(); x++) {
			std::cout << "nome n:" << x << "=" << temp[x]["name"]<< std::endl;
			if (temp[x]["name"] == nome) {
				result = true;
				break;
			}
		}
		//std::cout << "----------------------------------------------------------------------------" << std::endl;
		file.close();
		return result;
	}

	int doesItExist(std::string nome,bool tipo) {
		int result = 0;
		//i open the file
		std::ifstream file(jsonPath);
		//variabile temporanea per contenuti del json
		json temp;
		//i put the stream from the file into the variable
		file >> temp;//it autmatically parse it
		for (int x = 0; x < temp.size(); x++) {
			std::cout << "nome n:" << x << "=" << temp[x]["name"] << std::endl;
			if (temp[x]["name"] == nome) {
				result = x;
				break;
			}
		}
		//std::cout << "----------------------------------------------------------------------------" << std::endl;
		file.close();
		return result;
	}



	//this function si called when i need to modify a specific player data given in input overloading by aurgument
	//i call this function when i need to change the score by a given name
	void updatePlayerData(std::string name,int score) {
		if (!doesItExist(name)) {
			std::cout << "player con quel nome inesistente";
			return;
		}
		std::ifstream in(jsonPath);
		json temp;
		in >> temp;
		in.close();

		temp[doesItExist(name, true)]["score"] = score;
		std::ofstream out(jsonPath);
		out << temp.dump(4);
		out.close();
	}


	//i call this function when i need to update the name of a player
	void updatePlayerData(std::string name, std::string newName) {
		if (!doesItExist(name)) {
			return;
		}
		if (doesItExist(newName)) {
			//nuovo nome giá presente non possibile updatarlo
			std::cout << "nome giá esistente" << std::endl;
			return;
		}
		std::ifstream in(jsonPath);
		json temp;
		in >> temp;
		in.close();
		
		temp[doesItExist(name, true)]["name"] = newName;
		std::ofstream out(jsonPath);
		out << temp.dump(4);
		out.close();
	}
	
	//this function is called when i need to get the name of the player in return if it exists
	std::string getInfoPlayer(std::string nome) {
		if (doesItExist(nome)) {
			
			return nome;
		}
		else {
			
			return NULL;
		}
	}
	//this function is called when i need to get the score of a specified player
	int getInfoPlayer(std::string nome,bool s) {
		if (!doesItExist(nome)) {
			return NULL;
		}

		std::ifstream file(jsonPath);
		json temp;

		file >> temp;
		file.close();

		return temp[doesItExist(nome, true)]["score"];
	}



	//this function is called when i need to get the score of a player
	
	//this function is called when i need to delte them from the 
};