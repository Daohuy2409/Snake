all:
	g++ -Isrc/include -Lsrc/lib -o main main.cpp SDL_utils.cpp -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer