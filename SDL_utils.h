#ifndef SDL_UTILS__H
#define SDL_UTILS__H
#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <ctime>
#include <chrono>
#include <vector>
#include <fstream>
using namespace std;

void initSDL(SDL_Window*& window,SDL_Renderer*& renderer, int SCREEN_WIDTH, int SCREEN_HEIGHT );
void playground(SDL_Renderer* renderer, int PLAYGROUND_WIDTH, int PLAYGROUND_HEIGHT);
void SpeedUp(int score);
void GameOver(SDL_Renderer* renderer,SDL_Surface* surface, SDL_Texture* texture, TTF_Font* font,SDL_Event e);
int Highscore(int score);
void Quit(SDL_Window*& window,SDL_Renderer*& renderer);

#endif