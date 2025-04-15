#include <SDL3/SDL.h>
#include "./oggetti.h"
#include <iostream>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define FPS_CAP 80

bool Init(SDL_Window** window, SDL_Renderer** renderer) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
        return false;
    }

    *window = SDL_CreateWindow("SDL3 Template", WINDOW_WIDTH, WINDOW_HEIGHT, 0);
    if (!*window) {
        SDL_Log("Could not create window: %s", SDL_GetError());
        return false;
    }

    *renderer = SDL_CreateRenderer(*window, NULL);  // No flags needed in SDL3
    if (!*renderer) {
        SDL_Log("Could not create renderer: %s", SDL_GetError());
        return false;
    }

    return true;
}

bool is_paused = false;

float altezza = 100;
float larghezza = 50;

SDL_FRect* body1 = new SDL_FRect{100,100,larghezza,altezza};
Players* player1 = new Players(*body1, 100, 0, 255, 255);

SDL_FRect* body2 = new SDL_FRect{ WINDOW_WIDTH - larghezza * 2,WINDOW_HEIGHT - altezza * 2,larghezza ,altezza};
Players* player2= new Players(*body2, 100, 255, 0, 255);

SDL_FRect* balls = new SDL_FRect{ WINDOW_WIDTH / 2,WINDOW_HEIGHT / 2,20,20 };
Ball* palla = new Ball(*balls, 2, 255, 255,255);

const Uint8* keystates = SDL_GetKeyboardState(NULL);


void HandleEvents(bool* running) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_EVENT_QUIT) {
            *running = false;
        }
    }
}



int determineDirectionPlayers(int num_player) {
    //scan the input key assegned to the player 1 and return the direction in wich i needs to move keys(w,s)
    switch (num_player)
    {
    case 1:
        if (keystates[SDL_SCANCODE_S])
            return -1;

        if (keystates[SDL_SCANCODE_W])
            return 1;
        break;
    case 2:
        if (keystates[SDL_SCANCODE_DOWN])
            return -1;

        if (keystates[SDL_SCANCODE_UP])
            return 1;
        break;
    default:
        break;
    }
        
    
}

void Update(float deltaTime) {
    // Your game logic here
    player1->muoviti(determineDirectionPlayers(1), deltaTime,WINDOW_HEIGHT);
    player2->muoviti(determineDirectionPlayers(2), deltaTime,WINDOW_HEIGHT);

    //update della palla
    if (!is_paused) {
        palla->muovi();
    }

    // debug da eliminare poi
    if (keystates[SDL_SCANCODE_K]) {
        player1->speed -= 30;
        player2->speed -= 30;
    }
    if (keystates[SDL_SCANCODE_L]) {
        player1->speed += 30;
        player2->speed += 30;
    }
    //std::cout << "posizione player 1=" << player1->body.y << std::endl;
    //std::cout << "posizione player 2=" << player2->body.y<< std::endl;

    

    
    


    (void)deltaTime;
}

void Render(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderClear(renderer);

    // render player 1
    SDL_SetRenderDrawColor(renderer, player1->color.r, player1->color.g, player1->color.b,255);
    SDL_RenderFillRect(renderer, &player1->body);
    // render player 2
    SDL_SetRenderDrawColor(renderer, player2->color.r, player2->color.g, player2->color.b, 255);
    SDL_RenderFillRect(renderer, &player2->body);

    //render ball
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderFillRect(renderer, &palla->body);
    SDL_RenderPresent(renderer);
}



Uint64 now = SDL_GetPerformanceCounter();
Uint64 last = 0;
double deltaTime = 0;

int frameCount = 0;
double fps = 0;
double fpsTimer = 0;



int main(int argc, char* argv[]) {
    (void)argc;
    (void)argv;

    SDL_Window* window = NULL;
    SDL_Renderer* renderer = NULL;

    if (!Init(&window, &renderer)) {
        return 1;
    }

    bool running = true;
    Uint64 now = SDL_GetTicks();
    Uint64 last = now;
    float deltaTime = 0;
    const Uint32 frameDelay = 1000 / FPS_CAP;

    // ✅ Add these:
    int frameCount = 0;
    float fps = 0.0f;
    Uint64 fpsTimer = SDL_GetTicks();

    while (running) {
        last = now;
        now = SDL_GetTicks();
        deltaTime = (now - last) / 1000.0f;

        frameCount++;  // ✅ Count frames

        if (now - fpsTimer >= 1000) { // 1 second passed
            fps = frameCount * 1000.0f / (now - fpsTimer);
            std::cout << "FPS: " << (int)fps << std::endl;

            fpsTimer = now;
            frameCount = 0;
        }

        HandleEvents(&running);
        Update(deltaTime);
        Render(renderer);

        // apply fps cap
        Uint32 frameTime = SDL_GetTicks() - now;
        if (frameTime < frameDelay) {
            SDL_Delay(frameDelay - frameTime);
        }


        const Uint8* keystates = SDL_GetKeyboardState(NULL);
        if (keystates[SDL_SCANCODE_ESCAPE])
            break;
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}

