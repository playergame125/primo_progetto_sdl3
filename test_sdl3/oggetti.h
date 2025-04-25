#pragma once
#include <SDL3/SDL.h>
#include <stdbool.h>
#include <iostream>
#include <cmath>
#include <random>
#define PI 3.14159




#define BIANCO SDL_Color color{255,255,255}
#define NERO SDL_Color color{0,0,0}


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
    int las_direction;
    int point = 0;

    void muoviti(int direzione,float deltatime,int windowheight) {
        las_direction = direzione;
        if (direzione > 0) {
            direzione = 1;
        }
        else
            if (direzione != 0) {
                direzione = -1;
            }
        
        if(checkBoundaries(windowheight,direzione))
            body.y =body.y - (direzione * speed)*deltatime;
    }

    void addPoint(int punti) {
        point += punti;
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
    Ball(SDL_FRect body, float speed, int r, int g, int b) :body(body), speed(speed), color{ (Uint8)r,(Uint8)g,(Uint8)b } {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<float> dist(-1.0f, 1.0f);
        //std::uniform_int_distribution<int> fas(-400, 400);
        direzione[0] = dist(gen);
        direzione[1] = dist(gen);
        //last_bounce[0] = fas(gen);
        //last_bounce[1] = fas(gen);
    }
    SDL_FRect body;
    float speed;
    float angle;
    int last_bounce[2] = { 400,400 };
    int bounced_by_left=0;//0=none 1=sinistra -1=destra
    int last_touch=3;
    SDL_Color color;
    //
    float direzione[2] = {-1,1};//x y 
    bool muovi(int windowheight,int windowwidth,Players* player1, Players* player2,std::mt19937& generatore_random) {
        if (body.x>0 && body.x<windowwidth) {
            if (check_boundaries(windowheight, windowwidth, player1->body, player2->body)) {
                body.x = body.x + (speed * direzione[0]);
                body.y = body.y - (speed * direzione[1]);
            }
            else
            {
                bounce(player1, player2, windowwidth, generatore_random);
            }
            return true;
        }
        else {
            return false;
        }
    }

    bool outsideLeft() {
        if (body.x > 0)
            return false;//exited on the right(player2)
        else
            return true;//exited on the left
    }

    void bounce(Players* p1, Players* p2,int windowwidth, std::mt19937& generatore_random) {
        //calculate the angle with the older coordinates
        if (last_touch != bounced_by_left || last_touch==0) {
            angle = calculate_angle();
            
            //change the direction based on the angle just calculated
            std::cout << "calculated angle (degrees)=" << angle * 180.0f / PI << "\n";
            if (bounced_by_left == 1 || bounced_by_left == -1) {
                bool boos_up = false;// true if needs to be bounced up(+5/0 degre) else false(-5/0 degree)
                //enters if bounced by player
                if (bounced_by_left==1) {
                    //vounced by player1
                    
                    //debug 
                    //--------------
                    //p1->addPoint(1);
                    //---------------
                    float changes= rand() % 88;
                    std::cout << "bounced by player 1" << "\n";
                    switch (p1->las_direction)
                    {
                        case 1:
                            if (direzione[1] > 0) {
                                //bounced in accordanza qunidi si deve boostare
                                
                                boos_up = true;
                                //std::cout << "bounced in accordanza"<<randomize_paddle_bounce(boos_up, generatore_random)<<"\n";
                                std::cout << "angolo bounce prima=" <<angle * 180.0f / PI;
                                angle += randomize_paddle_bounce(boos_up, generatore_random);
                                std::cout << "-angolo dopo=" << angle * 180.0f / PI << "\n";
                                

                            }
                            else {
                                boos_up = false;
                                //std::cout << "bounced non accordanza" << randomize_paddle_bounce(boos_up, generatore_random) << "\n";
                                std::cout << "angolo bounce prima=" << angle * 180.0f / PI;
                                angle -= randomize_paddle_bounce(boos_up, generatore_random);
                                std::cout << "-angolo dopo=" << angle * 180.0f / PI << "\n";
                            }
                            break;
                        case -1:
                            if (direzione[1] < 0) {
                                //bounced non in accordanza quindi é da nerfare

                                boos_up = true;
                                //std::cout << "bounced in accordanza" << randomize_paddle_bounce(boos_up, generatore_random) << "\n";
                                std::cout << "angolo bounce prima=" << angle * 180.0f / PI;
                                angle -= randomize_paddle_bounce(boos_up, generatore_random);
                                std::cout << "-angolo dopo=" << angle * 180.0f / PI << "\n";
                            }
                            else {
                                boos_up = false;
                                //std::cout << "bounced non accordanza" << randomize_paddle_bounce(boos_up, generatore_random) << "\n";
                                std::cout << "angolo bounce prima=" << angle * 180.0f / PI;
                                angle += randomize_paddle_bounce(boos_up, generatore_random);
                                std::cout << "-angolo dopo=" << angle * 180.0f / PI << "\n";
                            }
                            break;

                    default:
                        std::cout << "bounce base" << "\n";
                        break;
                    }
                }
                else {
                    std::cout << "bounced by player 2" << "\n";
                    //p2->addPoint(1);
                    switch (p2->las_direction)
                    {
                    case 1:
                        if (direzione[1] > 0) {
                            //bounced in accordanza qunidi si deve boostare

                            boos_up = true;
                            //std::cout << "bounced in accordanza"<<randomize_paddle_bounce(boos_up, generatore_random)<<"\n";
                            std::cout << "angolo bounce prima=" << angle * 180.0f / PI;
                            angle += randomize_paddle_bounce(boos_up, generatore_random);
                            std::cout << "-angolo dopo=" << angle * 180.0f / PI << "\n";


                        }
                        else {
                            boos_up = false;
                            //std::cout << "bounced non accordanza" << randomize_paddle_bounce(boos_up, generatore_random) << "\n";
                            std::cout << "angolo bounce prima=" << angle * 180.0f / PI;
                            angle -= randomize_paddle_bounce(boos_up, generatore_random);
                            std::cout << "-angolo dopo=" << angle * 180.0f / PI << "\n";
                        }
                        break;
                    case -1:
                        if (direzione[1] < 0) {
                            //bounced non in accordanza quindi é da nerfare

                            boos_up = true;
                            //std::cout << "bounced in accordanza" << randomize_paddle_bounce(boos_up, generatore_random) << "\n";
                            std::cout << "angolo bounce prima=" << angle * 180.0f / PI;
                            angle -= randomize_paddle_bounce(boos_up, generatore_random);
                            std::cout << "-angolo dopo=" << angle * 180.0f / PI << "\n";
                        }
                        else {
                            boos_up = false;
                            //std::cout << "bounced non accordanza" << randomize_paddle_bounce(boos_up, generatore_random) << "\n";
                            std::cout << "angolo bounce prima=" << angle * 180.0f / PI;
                            angle += randomize_paddle_bounce(boos_up, generatore_random);
                            std::cout << "-angolo dopo=" << angle * 180.0f / PI << "\n";
                        }
                        break;
                    default:
                        std::cout << "bounce base" << "\n";
                        break;
                    }
                }

                direzione[0] = -1 * cos(angle);
                direzione[1] = -1 * sin(angle);

            }
            else {
                direzione[0] = 1 * cos(angle);
                direzione[1] = sin(angle);
            }
            
            
            std::cout << "nuova direzione x=" << direzione[0] << ",y=" << direzione[1] << std::endl;
            //save where the bounce coordinate are
            
                last_bounce[0] = body.x;
                last_bounce[1] = body.y;
            
        }
    }

    float randomize_paddle_bounce(bool is_accordanza,std::mt19937& generatore_random) {
        //calculate the degree ge needs to change bu a random number
        
        std::uniform_real_distribution<float> dist(0.0f, 0.087f);
        if (is_accordanza) {
            return dist(generatore_random);

        }
        else
        {
            return -1*dist(generatore_random);
        }
    }

    float calculate_angle() {
        float angle;
        
           angle = atan2f(body.y - last_bounce[1], body.x - last_bounce[0 ]);

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
            if (direzione[1] >= 0) {
                return true;
                
            }
            else {
                //std::cout << "clamp down" << "\n";
                return false;
            }
                
        }

        //check collisione con players
        if (body.x < windowwidth / 2) {
            //std::cout << "check player1 parte" << std::endl;
            if (SDL_HasRectIntersectionFloat(&body, &player1)) {
                //std::cout << "detected collision with player 1" << std::endl;
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
