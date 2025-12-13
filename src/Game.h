#ifndef GAME_H
#define GAME_H
#include <sdl/SDL.h>
class Game
{
public:
	Game();
	~Game();
	void Initialize();
	void Run();
	void ProcessInput();
	void Update();
	void Render();
	void Destroy();
	void Setup();

private:
	SDL_Window* window;
	SDL_Renderer* renderer;
	bool isRunning;
	int windowWidth;
	int windowHeight;
};

#endif