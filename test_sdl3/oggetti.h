#pragma once
#include <SDL3/SDL.h>
#include <stdbool.h>
#include <iostream>


struct Players {
public:
    /*Players(SDL_FRect body, float speed, int r, int g, int b) {
        this->body=body;   vecchio modo di lavorare
        this->speed=speed;
        this->color = SDL_Color{ r,g,b,255 };
    };*/
    Players(SDL_FRect body, float speed, int r, int g, int b):body(body), speed(speed), color{ (Uint8)r,(Uint8)g,(Uint8)b } {}


    SDL_FRect body;
    float speed;
    SDL_Color color;
    void muoviti(int direzione,float deltatime,int windowheight) {
        if (direzione > 0)
            direzione = 1;
        else
            if(direzione != 0)
                direzione = -1;
        if(checkBoundaries(windowheight,direzione))
            body.y =body.y - (direzione * speed)*deltatime;
    }


    bool checkBoundaries(int window_height,int direzione) {
        if (body.y <=0) {
            //std::cout << "clamp up" << std::endl;
            switch (direzione) {
            case 1:
                return false;
                break;
            case -1:
                return true;
                break;
            default:
                break;
            }
        }
        else {
            if (body.y + body.h >= window_height) {
                //std::cout << "clamp down" << std::endl;
                switch (direzione) {
                case -1:
                    return false;
                    break;
                case 1:
                    return true;
                    break;
                default:
                    break;
                }
            }
        }
    }
};
struct Ball {
public:
    Ball(SDL_FRect body, float speed, int r, int g, int b) :body(body), speed(speed), color{ (Uint8)r,(Uint8)g,(Uint8)b } {}
    SDL_FRect body;
    float speed;
    SDL_Color color;
    float direzione[2] = {0,0};//x y
    void muovi() {

    }
};