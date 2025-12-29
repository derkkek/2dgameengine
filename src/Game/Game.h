#ifndef GAME_H
#define GAME_H
#include <sdl/SDL.h>
#include "../ECS/ECS.h"
#include <memory>
#include "../AssetStore/AssetStore.h"
#include "../EventBus/EventBus.h"

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
	void LoadLevel(int level);

	static int windowWidth;
	static int windowHeight;
	static int mapWidth;
	static int mapHeight;

	static std::unique_ptr<Registry> registry;

private:
	SDL_Window* window;
	SDL_Renderer* renderer;
	SDL_Rect camera;
	
	std::unique_ptr<AssetStore> assetStore;
	std::unique_ptr<EventBus> eventBus;
	bool isRunning;

	const int FPS = 500;
	const int millisecondsPerFrame = 1000 / FPS;
	int millisecondsPreviousFrame;
	bool debug;
};

#endif