#ifndef GAME_H
#define GAME_H
#include <sdl/SDL.h>
#include "../ECS/ECS.h"
#include <memory>
#include "../AssetStore/AssetStore.h"

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
	std::unique_ptr<Registry> registry;
	std::unique_ptr<AssetStore> assetStore;
	bool isRunning;
	int windowWidth;
	int windowHeight;
	const int FPS = 500;
	const int millisecondsPerFrame = 1000 / FPS;
	int millisecondsPreviousFrame;
};

#endif