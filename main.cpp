
#include "SDL_utils.h"


#define SCREEN_WIDTH 544
#define SCREEN_HEIGHT 394
#define CELL_SIZE 15
#define MAX_SNAKE_SIZE 100
#define PLAYGROUND_WIDTH 540
#define PLAYGROUND_HEIGHT 300


enum CELL_TYPE{
    CELL_EMPTY=0,
    CELL_SNAKE,
    CELL_APPLE
};
enum Direction{
    DOWN=0,
    LEFT,RIGHT,UP
};
enum GameStatus{
    GAME_RUNNING=1,
    GAME_STOP=2,
    GAME_WIN=4|GAME_STOP,
    GAME_LOSE=8|GAME_STOP
};

//

void changedirection(Direction newdirection, Direction& direction);
void moveSnake(int direction, SDL_Rect &head);
void DrawSnake(SDL_Renderer* renderer, vector<SDL_Rect>& Snake, Direction direction, GameStatus& running);
void CreateApples(SDL_Renderer* renderer ,SDL_Rect &apple);
void SnakeEatApple(SDL_Renderer* renderer, vector<SDL_Rect>& Snake, SDL_Rect& apple, int& score);

//



int main(int argc,char**argv){
    int score=0;
    SDL_Window* window = NULL;
    SDL_Renderer* renderer = NULL;
    TTF_Font* font = NULL;
	SDL_Surface* surface = NULL;
	SDL_Texture* texture = NULL;
    initSDL(window,renderer,SCREEN_WIDTH,SCREEN_HEIGHT);

    //khởi tạo ttf 
    if(TTF_Init()<0){
        printf("Error: %s \n",TTF_GetError());
        return -1;
    }
    font = TTF_OpenFont("VeraMoBd.ttf", 20);
	SDL_Color color_text = { 255, 255, 255 };


    //tạo đầu rắn
    vector<SDL_Rect> Snake;
    SDL_Rect head={PLAYGROUND_WIDTH/2+2,PLAYGROUND_HEIGHT/2+2,CELL_SIZE,CELL_SIZE};
    Snake.push_back(head);
    
    //tạo quả 
    SDL_Rect apple;
    CreateApples(renderer, apple);

    SDL_Event e;
    Direction direction=RIGHT;
    Direction newdirection=RIGHT;
    GameStatus running=GAME_RUNNING;
    while(running==GAME_RUNNING){
        while(SDL_PollEvent(&e)){
            switch(e.type){
                case SDL_QUIT:
                    running=GAME_STOP;
                    break;
                case SDL_KEYDOWN:
                    switch (e.key.keysym.sym)
                    {
                    case SDLK_UP:
                        newdirection=UP;
                        break;
                    case SDLK_DOWN:
                        newdirection=DOWN;
                        break;
                    case SDLK_RIGHT:
                        newdirection=RIGHT;
                        break;
                    case SDLK_LEFT:
                        newdirection=LEFT;
                        break;
                    }
                    
                    changedirection(newdirection,direction);
            }
        }

        playground(renderer,PLAYGROUND_WIDTH,PLAYGROUND_HEIGHT);

        //tạo text để in điểm số
        string text="Your Score: "+to_string(score)+"                High score: "+to_string(Highscore(score));
        surface = TTF_RenderText_Solid(font, text.c_str(), color_text);
        texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_FreeSurface(surface);

        //xác định tọa độ vẽ text
        SDL_Rect srcRest;
        SDL_Rect desRect;
        TTF_SizeText(font, text.c_str(), &srcRest.w, &srcRest.h);
        srcRest.x = 0;
        srcRest.y =  0;

        desRect.x = 10;
        desRect.y = 320;

        desRect.w = srcRest.w;
        desRect.h = srcRest.h;

		SDL_RenderCopy(renderer, texture, &srcRest, &desRect);

        //vẽ táo 
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_RenderFillRect(renderer,&apple);

        SnakeEatApple(renderer,Snake,apple,score);
        
        DrawSnake(renderer, Snake, direction, running);
        
        //xử lí khi đầu rắn va chạm với thân rắn
        for(int i=1;i<Snake.size();++i){
            if(Snake[0].x==Snake[i].x&&Snake[0].y==Snake[i].y){
                running=GAME_LOSE;
                break;
            }
        }
        
        SDL_RenderPresent(renderer);
        SpeedUp(score);
    }
    GameOver(renderer, surface, texture, font,e);
    Quit(window,renderer);
    return 0;
}

//

void changedirection(Direction newdirection, Direction& direction){
    switch(newdirection){
        case UP:
            direction = direction == DOWN ? direction : UP; 
            break;
        case DOWN:
            direction = direction == UP ? direction : DOWN; 
            break;
        case LEFT:
            direction = direction == RIGHT ? direction : LEFT; 
            break;
        case RIGHT:
            direction = direction == LEFT ? direction : RIGHT; 
            break;
        }
}

void moveSnake(int direction, SDL_Rect &head){
    switch(direction){
        case UP:
            head.y-=CELL_SIZE;
            break;
        case DOWN:
            head.y+=CELL_SIZE;
            break;
        case RIGHT:
            head.x+=CELL_SIZE;
            break;
        case LEFT:
            head.x-=CELL_SIZE;
            break;
        }
}

void DrawSnake(SDL_Renderer* renderer, vector<SDL_Rect>& Snake, Direction direction, GameStatus& running){
    int x0=Snake[0].x;
    int y0=Snake[0].y;
    for(int i=0;i<Snake.size();++i)
    {
        SDL_SetRenderDrawColor(renderer,59, 66, 40, 255);
        if(i==0)
        {
            moveSnake(direction,Snake[i]);
            if((Snake[i].x<0||Snake[i].x>=PLAYGROUND_WIDTH)||(Snake[i].y<0||Snake[i].y>=PLAYGROUND_HEIGHT)) 
            //xử lí khi va chạm với khung 
            { 
                running=GAME_LOSE;
                break;
            }
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            SDL_RenderFillRect(renderer,&Snake[i]);
        }
        else
        {
            int temp_x = Snake[i].x;  int temp_y = Snake[i].y; //lưu vị trí cũ của đốt i 
            Snake[i].x = x0;  Snake[i].y = y0; //di chuyển đốt i sang vị trí cũ của đốt phía trước
            x0 = temp_x;  y0 = temp_y; //ghi nhớ vị trí cho lần lặp tiếp theo i+1
            Snake[i].w = CELL_SIZE;   Snake[i].h = CELL_SIZE;
            SDL_RenderFillRect(renderer,&Snake[i]);
        }
    }
}

void CreateApples(SDL_Renderer* renderer ,SDL_Rect &apple){
    srand(time(NULL));
    int x=rand()%(PLAYGROUND_WIDTH/CELL_SIZE)*CELL_SIZE+2;
    int y=rand()%(PLAYGROUND_HEIGHT/CELL_SIZE)*CELL_SIZE+2;
    apple={x,y,CELL_SIZE,CELL_SIZE};
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderFillRect(renderer,&apple);
}

void SnakeEatApple(SDL_Renderer* renderer, vector<SDL_Rect>& Snake, SDL_Rect& apple, int& score){
    if((Snake[0].x == apple.x) && (Snake[0].y == apple.y)){
            
            SDL_SetRenderDrawColor(renderer,196, 208, 163, 255);
            SDL_RenderFillRect(renderer,&apple);
            CreateApples(renderer, apple);
            SDL_Rect snake_body;
            Snake.push_back(snake_body);
            score++;
            
        }
}

//