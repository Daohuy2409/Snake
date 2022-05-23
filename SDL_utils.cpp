#include "SDL_utils.h"

void initSDL(SDL_Window*& window, SDL_Renderer*& renderer, int SCREEN_WIDTH, int SCREEN_HEIGHT){
    
    if(SDL_Init(SDL_INIT_VIDEO) < 0){
        printf("Error: SDL failed to initialize\nSDL Error: '%s'\n", SDL_GetError());
        return ;
    }

    window = SDL_CreateWindow("Snake game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
    if(!window){
        printf("Error: Failed to open window\nSDL Error: '%s'\n", SDL_GetError());
        return ;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if(!renderer){
        printf("Error: Failed to create renderer\nSDL Error: '%s'\n", SDL_GetError());
        return ;
    }
    SDL_RenderSetLogicalSize(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);
}

void playground(SDL_Renderer* renderer, int PLAYGROUND_WIDTH, int PLAYGROUND_HEIGHT){
    SDL_SetRenderDrawColor(renderer,196, 208, 163, 255);
    SDL_RenderClear(renderer);
        
    //ve vien
    SDL_SetRenderDrawColor(renderer,87, 97, 64, 255);

    SDL_RenderDrawLine(renderer, 0, 0, PLAYGROUND_WIDTH+3, 0);
    SDL_RenderDrawLine(renderer, 0, 0, 0, PLAYGROUND_HEIGHT+3);
    SDL_RenderDrawLine(renderer, PLAYGROUND_WIDTH+3, 0, PLAYGROUND_WIDTH+3, PLAYGROUND_HEIGHT+3);
    SDL_RenderDrawLine(renderer, 0, PLAYGROUND_HEIGHT+3, PLAYGROUND_WIDTH+3, PLAYGROUND_HEIGHT+3);

    SDL_RenderDrawLine(renderer, 1, 1, PLAYGROUND_WIDTH+2, 1);
    SDL_RenderDrawLine(renderer, 1, 1, 1, PLAYGROUND_HEIGHT+2);
    SDL_RenderDrawLine(renderer, PLAYGROUND_WIDTH+2, 1, PLAYGROUND_WIDTH+2, PLAYGROUND_HEIGHT+2);
    SDL_RenderDrawLine(renderer, 1, PLAYGROUND_HEIGHT+2, PLAYGROUND_WIDTH+2, PLAYGROUND_HEIGHT+2);
}

void SpeedUp(int score){
    if(score<=10) SDL_Delay(100);
    else if(score<=20) SDL_Delay(80);
    else SDL_Delay(60);
}

void GameOver(SDL_Renderer* renderer,SDL_Surface* surface, SDL_Texture* texture, TTF_Font* font,SDL_Event e){//???
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255 );
    SDL_RenderClear(renderer);
    SDL_Color color_text={255, 255, 255};
    font=TTF_OpenFont("VeraMoBd.ttf",50);
    string text="Game Over!!!";
    surface = TTF_RenderText_Solid(font, text.c_str(), color_text);
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    SDL_Rect srcRest;
    SDL_Rect desRect;
    TTF_SizeText(font, text.c_str(), &srcRest.w, &srcRest.h);

    srcRest.x = 0;
    srcRest.y =  0;

    desRect.x = 100;
    desRect.y = 150;

    desRect.w = srcRest.w;
    desRect.h = srcRest.h;
    int running=true;
    while (running)
	{
		//event
		while (SDL_PollEvent(&e))
		{
			switch (e.type)
			{
				
				case SDL_QUIT:
				{
					running = false;
					break;
				}
				default:
				{
					break;
				}
			}
		}
		// clear the window to black
		SDL_RenderClear(renderer);
		//Copy a portion of the texture to the current rendering target.
		SDL_RenderCopy(renderer, texture, &srcRest, &desRect);
		//draw to screen
		SDL_RenderPresent(renderer);
	}
}

int Highscore(int score){
    ifstream input("Score.txt");
    if(!input.is_open()) return -1;
    int high_score;
    input>>high_score;
    if(score>high_score){
        ofstream output("Score.txt");
        high_score=score;
        output<<score;
        output.close();
    }
    input.close();
    return high_score;
}

void Quit(SDL_Window*& window,SDL_Renderer*& renderer){
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    TTF_Quit();
    SDL_Quit();
}
