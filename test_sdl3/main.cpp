#include <SDL3/SDL.h>
#include "./oggetti.h"
#include <iostream>
#include <random>
#include <string>
#include <sstream>
#include <SDL3_ttf/SDL_ttf.h>
#include "./startmenu.h"
#include "./pausemenu.h"



#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define POINTBOXW 200//width point box
#define POINTBOXH 200//heigtpoint box
#define FPS_CAP 80

std::random_device generatore_random;
std::mt19937 gen(generatore_random());

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
Ball* palla = new Ball(*balls, 1, 255, 255,255);



TTF_Font* myFont;


//tutto temporaneo pere test libnreria
SDL_FRect* pointBox = new SDL_FRect{ WINDOW_WIDTH/2-POINTBOXW/2,0,POINTBOXW,POINTBOXH};
SDL_Texture* pointText;//texture punteggio



const Uint8* keystates = SDL_GetKeyboardState(NULL);

enum GameState
{
    STARTMENU,
    RUNNINGGAME,
    PAUSEMENU,
    SCOREBOARD,
    LOGIN
};

int game = STARTMENU;

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
        return 0;
        break;
    case 2:
        if (keystates[SDL_SCANCODE_DOWN])
            return -1;

        if (keystates[SDL_SCANCODE_UP])
            return 1;
        break;
        return 0;
    default:
        break;
    }
        
    
}
void updateTextPoints(SDL_Renderer* renderer) {
    SDL_DestroyTexture(pointText);
    std::ostringstream mult;//create string stream
    mult << player1->point << ":" << player2->point;
    SDL_Surface* temp = TTF_RenderText_Solid(myFont,mult.str().c_str(), 0, SDL_Color{255,255,255});
    pointText = SDL_CreateTextureFromSurface(renderer, temp);
    SDL_DestroySurface(temp);
}
SDL_Color color= SDL_Color{ 255,255,255,255 };


void fattogoal(Players* vincitore, SDL_Renderer* renderer) {
    //update player point
    vincitore->addPoint(1);
    //update pointbox
    updateTextPoints(renderer);
    //destroy palla attuale
    delete palla;
    //instantiate new ball
    palla = new Ball(*balls, .5, 255, 255, 255);
}



void Update(float deltaTime,SDL_Renderer* renderer) {
    // Your game logic here
    player1->muoviti(determineDirectionPlayers(1), deltaTime,WINDOW_HEIGHT);
    player2->muoviti(determineDirectionPlayers(2), deltaTime,WINDOW_HEIGHT);
   // std::cout << "bounced by vakire=" << palla->bounced_by_left<< std::endl;

    //update della palla
    if (!is_paused) {
        if (!palla->muovi(WINDOW_HEIGHT, WINDOW_WIDTH, player1, player2, gen)) {
            if (!palla->outsideLeft())
                fattogoal(player1, renderer);
            else
                fattogoal(player2, renderer);
        }
        palla->randomize_paddle_bounce(true, gen);
        

    }

    // debug da eliminare poi
    if (keystates[SDL_SCANCODE_K]) {
        player1->speed -= 30;
        player2->speed -= 30;
        palla->speed -= .2;
    }
    if (keystates[SDL_SCANCODE_L]) {
        player1->speed += 30;
        player2->speed += 30;
        palla->speed += .2;
    }
    //std::cout << "posizione player 1=" << player1->body.y << std::endl;
    //std::cout << "posizione player 2=" << player2->body.y<< std::endl
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
    

    //render text box(solo test temporaneo)
   
    SDL_RenderTexture(renderer, pointText, NULL, pointBox);
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
    
    bool SpaceLastPress = false;
    game = STARTMENU;
    Startmenu* startmenu = new Startmenu();
    PauseMenu* pausemenu = new PauseMenu();
    
    //initialize the ttf library
    if (TTF_Init() == -1) {
        std::cout << "non é styato possibbile inizial;izzare ttf:" << SDL_GetError();
    }
    else {
        std::cout << "riuscito ad inizializzare"<<std::endl;
    }
    

    //load the font
    myFont = TTF_OpenFont("./fonts/EthnocentricRgIt.otf", 32);

    if (myFont == nullptr) {
        std::cout << "font non caricato" << std::endl;
    }


    
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


    //solo per test della ttf libreria
    //create the texture surface from a font
    //SurfaceText= TTF_RenderText_Solid(myFont, "22", 0, SDL_Color{ 0,255,255 });
    //convert the surface in a texture so it can be applied to a rectangle and displayed
    //TextureText = SDL_CreateTextureFromSurface(renderer, SurfaceText);
    //libreo la superfice
    //SDL_DestroySurface(SurfaceText);
    //create the rectangle on wich the texture will be displayed
    

    //initialize the point scoreboard
    updateTextPoints(renderer);

    //initializa the start menu
    startmenu->init(renderer,WINDOW_HEIGHT,WINDOW_WIDTH);
    pausemenu->init(renderer, WINDOW_HEIGHT, WINDOW_WIDTH);
    while (running) {
        last = now;
        now = SDL_GetTicks();
        deltaTime = (now - last) / 1000.0f;

        frameCount++;  // ✅ Count frames

        if (now - fpsTimer >= 1000) { // 1 second passed
            fps = frameCount * 1000.0f / (now - fpsTimer);
            //std::cout << "FPS: " << (int)fps << std::endl;

            fpsTimer = now;
            frameCount = 0;
        }
        

        HandleEvents(&running);
        switch (game)
        {
        case STARTMENU:
            //remain stuck here as long game remain in starteemu state
            //std::cout << "fai tutte le cose per il menu di start" << std::endl;
            startmenu->UpdateStartMenu();
            startmenu->RenderStartMenu();
            break;
        case RUNNINGGAME:
            //remain stuck here as long game remain in running game state
            //std::cout << "fai tutte le cose per il game running" << std::endl;
            Update(deltaTime, renderer);
            Render(renderer);
            break;
        case PAUSEMENU:
            pausemenu->update();
            pausemenu->render();
            break;
        }
        

        // apply fps cap
        Uint32 frameTime = SDL_GetTicks() - now;
        if (frameTime < frameDelay) {
            SDL_Delay(frameDelay - frameTime);
        }


        keystates = SDL_GetKeyboardState(NULL);
        if (keystates[SDL_SCANCODE_ESCAPE])
            break;
        if (keystates[SDL_SCANCODE_0])
            game = STARTMENU;
       
        if (keystates[SDL_SCANCODE_SPACE]&&SpaceLastPress!= keystates[SDL_SCANCODE_SPACE]) {
            switch (game)
            {
            case STARTMENU:
                game = RUNNINGGAME;
                break;
            case RUNNINGGAME:
                game = PAUSEMENU;
                break;
            case PAUSEMENU:
                game = RUNNINGGAME;
                break;
            }
        }
            
        SpaceLastPress = keystates[SDL_SCANCODE_SPACE];
            
    }
    SDL_DestroyTexture(pointText);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_CloseFont(myFont);
    SDL_Quit();


    return 0;
}

