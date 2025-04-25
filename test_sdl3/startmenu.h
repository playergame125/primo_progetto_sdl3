#pragma once
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>

struct Startmenu {
	
	Startmenu(SDL_Renderer* renderer) :renderer(renderer){}
	int top_score;
	SDL_Renderer* renderer;

	void RenderStartMenu() {
		//this function will be called from the main when the game is the start menu state

	}
	void  UpdateStartMenu() {
		//this function will be called from the main when the game is the start menu state
	}
};