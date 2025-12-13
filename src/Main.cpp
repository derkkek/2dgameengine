// Fix for SDL_ttf deprecated attribute syntax issue with MSVC
#define SDL_DEPRECATED

#include "Game.h"

int main(int argc, char* argv[])
{
	Game game;
	game.Run();
	return 0;
}