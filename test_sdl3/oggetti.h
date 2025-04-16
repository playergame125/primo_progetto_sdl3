#pragma once
#include <SDL3/SDL.h>
#include <stdbool.h>
#include <iostream>
#include <cmath>
#define PI 3.14159


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
    float angle;
    int last_bounce[2] = { 400,400 };
    int bounced_by_left=0;//0=none 1=sinistra -1=destra
    int last_touch=3;
    SDL_Color color;
    //
    float direzione[2] = {-1,1};//x y 
    void muovi(int windowheight,int windowwidth,SDL_FRect player1, SDL_FRect player2) {
        if (check_boundaries(windowheight,windowwidth,player1,player2)) {
            body.x = body.x + (speed * direzione[0]);
            body.y = body.y - (speed * direzione[1]);
        }
        else
        {
            bounce();
        }
    }


    void bounce() {
        //calculate the angle with the older coordinates
        if (last_touch != bounced_by_left) {
            angle = calculate_angle();

            //change the direction based on the angle just calculated
            std::cout << "calculated angle (degrees)=" << angle * 180.0f / PI << "\n";
            if(bounced_by_left==1 || bounced_by_left==-1)
                direzione[0] = -1 * cos(angle);
            else {
                direzione[0] = 1 * cos(angle);
            }
                direzione[1] = sin(angle);
            
            std::cout << "nuova direzione x=" << direzione[0] << ",y=" << direzione[1] << std::endl;
            //save where the bounce coordinate are
            
                last_bounce[0] = body.x;
                last_bounce[1] = body.y;
            
        }
    }

    float calculate_angle() {
        float angle;
        
           angle = atan2f(body.y - last_bounce[1], body.x - last_bounce[0]);

        std::cout << "lastbounce x=" << last_bounce[0] << ",y=" << last_bounce[1] << "\n attuale x=" << body.x << ",y=" << body.y << "\n differenza x=" << body.x - last_bounce[0] << ",y=" << body.y - last_bounce[1];
        std::cout << "angolo calcolato=" << angle << std::endl;
        std::cout << "calculated angle (degrees)=" << angle * 180.0f / PI << "\n";
        
        last_touch = bounced_by_left;

        return angle;
    }
    
    bool check_boundaries(int windowheigt,int windowwidth,SDL_FRect player1, SDL_FRect player2) {
        //check boundaries up and down
        bounced_by_left = 0;
        if (body.y <= 0) {
            
            //clamp up
            if (direzione[1] <= 0)
                return true;
            else
                return false;
        }
        if (body.y + body.h >= windowheigt) {
            //clamp down
            if (direzione[1] >= 0)
                return true;
            else
                return false;
        }

        //check collisione con players
        if (body.x < windowwidth / 2) {
            //std::cout << "check player1 parte" << std::endl;
            if (SDL_HasRectIntersectionFloat(&body, &player1)) {
                std::cout << "detected collision with player 1" << std::endl;
                bounced_by_left = 1;
                if (last_touch == 1) {
                    //std::cout << "falsificato risultati" << std::endl;
                    return true;
                }
                    
                else
                return false;
            }   
            else {
                return true;
                
            }
        } 
        else {
            //std::cout << "check player2 parte" << std::endl;

            if (SDL_HasRectIntersectionFloat(&body, &player2)) {
                //std::cout << "intersect con player 2" << std::endl;
                bounced_by_left = -1;
                if (last_touch == -1)
                    return true;
                else
                return false;

            }
                
            else {
                return true;
                
            }
                
        }

    }
};