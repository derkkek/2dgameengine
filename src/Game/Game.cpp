#include "Game.h"
#include <iostream>
#include <sdl/sdl_image.h>
#include <glm/glm.hpp>
#define FMT_UNICODE 0
#include <spdlog/spdlog.h>
#include "../Components/TransformComponent.h"
#include "../Components/RigidbodyComponent.h"
#include "../Systems/MovementSystem.h"
#include "../Systems/RenderSystem.h"
#include "../Components/SpriteComponent.h"

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
	spdlog::critical("this is a critical error", 42);
}
glm::vec2 playerPos;
glm::vec2 playerVel;
SDL_Rect player;

void Game::Setup()
{
	registry->AddSystem<MovementSystem>();
	registry->AddSystem<RenderSystem>();

	assetStore->AddTexture(renderer, "tank-image", "./assets/images/tank-tiger-right.png");
	assetStore->AddTexture(renderer, "truck-image", "./assets/images/truck-ford-down.png");
	Entity tank = registry->CreateEntity();

	tank.AddComponent<SpriteComponent>("tank-image", 32, 32);
	tank.AddComponent<TransformComponent>(glm::vec2(10.0, 10.0), glm::vec2(1.0, 1.0), 0.0);
	tank.AddComponent<RigidbodyComponent>(glm::vec2(1.0f, 0.0f));

	Entity truck = registry->CreateEntity();

	truck.AddComponent<SpriteComponent>("truck-image", 32, 32);
	truck.AddComponent<TransformComponent>(glm::vec2(5.0, 10.0), glm::vec2(1.0, 1.0), 0.0);
	truck.AddComponent<RigidbodyComponent>(glm::vec2(0.0f, 5.0f));
}

void Game::Run()
{
	Initialize();
	Setup();
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
