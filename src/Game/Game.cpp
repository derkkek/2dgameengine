#include "Game.h"
#include <iostream>
#include <sdl/sdl_image.h>
#include <glm/glm.hpp>
#include <fstream>
#include <sstream>
#include <vector>
#define FMT_UNICODE 0
#include <spdlog/spdlog.h>
#include "../Components/TransformComponent.h"
#include "../Components/RigidbodyComponent.h"
#include "../Systems/MovementSystem.h"
#include "../Systems/RenderSystem.h"
#include "../Components/SpriteComponent.h"
#include "../Components/AnimationComponent.h"
#include "../Systems/AnimationSystem.h"

Game::Game()
{
	registry = std::make_unique<Registry>();
	assetStore = std::make_unique<AssetStore>();
	isRunning = false;
}

Game::~Game()
{
}

void Game::Initialize()
{
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		spdlog::critical("Error initilaizing sdl");
		return;
	}
	SDL_DisplayMode displayMode;
	SDL_GetCurrentDisplayMode(0, &displayMode);

	windowWidth = 800;//displayMode.w;
	windowHeight = 600;//displayMode.h;
	window = SDL_CreateWindow(NULL, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, windowWidth, windowHeight, SDL_WINDOW_BORDERLESS);
	if (!window)
	{
		spdlog::critical("Error initializing window.");
		return;
	}
	
	renderer = SDL_CreateRenderer(window, -1, 0);
	if (!renderer)
	{
		spdlog::critical("Error initializing renderer.");
	}
	//SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);
	isRunning = true;
	spdlog::info("game is initialized");
}
glm::vec2 playerPos;
glm::vec2 playerVel;
SDL_Rect player;


void Game::LoadLevel(int level) {
	// Add the sytems that need to be processed in our game
	registry->AddSystem<MovementSystem>();
	registry->AddSystem<RenderSystem>();
	registry->AddSystem<AnimationSystem>();

	// Adding assets to the asset store
	assetStore->AddTexture(renderer, "tank-image", "./assets/images/tank-panther-right.png");
	assetStore->AddTexture(renderer, "truck-image", "./assets/images/truck-ford-right.png");
	assetStore->AddTexture(renderer, "tilemap-image", "./assets/tilemaps/jungle.png");
	assetStore->AddTexture(renderer, "chopper-image", "./assets/images/chopper.png");
	assetStore->AddTexture(renderer, "radar-image", "./assets/images/radar.png");

	// Load the tilemap
	int tileSize = 32;
	double tileScale = 2.0;
	int mapNumCols = 25;
	int mapNumRows = 20;

	std::fstream mapFile;
	mapFile.open("./assets/tilemaps/jungle.map");

	for (int y = 0; y < mapNumRows; y++) {
		for (int x = 0; x < mapNumCols; x++) {
			char ch;
			mapFile.get(ch);
			int srcRectY = std::atoi(&ch) * tileSize;
			mapFile.get(ch);
			int srcRectX = std::atoi(&ch) * tileSize;
			mapFile.ignore();

			Entity tile = registry->CreateEntity();
			tile.AddComponent<TransformComponent>(glm::vec2(x * (tileScale * tileSize), y * (tileScale * tileSize)), glm::vec2(tileScale, tileScale), 0.0);
			tile.AddComponent<SpriteComponent>("tilemap-image", tileSize, tileSize, 0, srcRectX, srcRectY);
		}
	}
	mapFile.close();

	// Create an entity
	Entity tank = registry->CreateEntity();
	tank.AddComponent<TransformComponent>(glm::vec2(10.0, 10.0), glm::vec2(1.0, 1.0), 0.0);
	tank.AddComponent<RigidbodyComponent>(glm::vec2(40.0, 0.0));
	tank.AddComponent<SpriteComponent>("tank-image", 32, 32, 2);

	Entity truck = registry->CreateEntity();
	truck.AddComponent<TransformComponent>(glm::vec2(50.0, 100.0), glm::vec2(1.0, 1.0), 0.0);
	truck.AddComponent<RigidbodyComponent>(glm::vec2(20.0, 0.0));
	truck.AddComponent<SpriteComponent>("truck-image", 32, 32, 1);

	Entity chopper = registry->CreateEntity();
	chopper.AddComponent<TransformComponent>(glm::vec2(100.0, 100.0), glm::vec2(1.0, 1.0), 0.0);
	chopper.AddComponent<RigidbodyComponent>(glm::vec2(0.0, 0.0));
	chopper.AddComponent<SpriteComponent>("chopper-image", 32, 32, 1);
	chopper.AddComponent<AnimationComponent>(2, 10,true);

	Entity radar = registry->CreateEntity();
	radar.AddComponent<TransformComponent>(glm::vec2(windowWidth - 74, 10.0), glm::vec2(1.0, 1.0), 0.0);
	radar.AddComponent<SpriteComponent>("radar-image", 64, 64, 2);
	radar.AddComponent<AnimationComponent>(8, 5, true);
}


void Game::Run()
{
	Initialize();

	LoadLevel(1);

	while (isRunning)
	{
		ProcessInput();
		Update();
		Render();
	}

}

void Game::ProcessInput()
{
	SDL_Event sdlEvent;
	
	while (SDL_PollEvent(&sdlEvent))
	{
		switch (sdlEvent.type)
		{
		case SDL_QUIT:
			isRunning = false;
		case SDL_KEYDOWN:
			if (sdlEvent.key.keysym.sym == SDLK_ESCAPE)
			{
				isRunning = false;
			}
		default:
			break;
		}
	}
}

void Game::Update()
{
	double deltaTime = (SDL_GetTicks() - millisecondsPreviousFrame) / 1000.0f;

	millisecondsPreviousFrame = SDL_GetTicks();
	
	registry->GetSystem<MovementSystem>().Update(deltaTime);
	registry->GetSystem<AnimationSystem>().Update();

	//update the registry to process the entitites to be deleted or created.
	registry->Update();
}


void Game::Render()
{
	SDL_SetRenderDrawColor(renderer, 21, 21, 21, 255);
	SDL_RenderClear(renderer);


	registry->GetSystem<RenderSystem>().Update(renderer, assetStore);

	SDL_RenderPresent(renderer);

}


void Game::Destroy()
{
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}
