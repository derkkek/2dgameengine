#include "Game.h"
#include <iostream>
#include <sdl/sdl_image.h>
#include <glm/glm.hpp>

Game::Game()
{
	isRunning = false;
}

Game::~Game()
{
}

void Game::Initialize()
{
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		std::cerr << "Error initilaizing sdl" << "\n";
		return;
	}
	SDL_DisplayMode displayMode;
	SDL_GetCurrentDisplayMode(0, &displayMode);

	windowWidth = 800;//displayMode.w;
	windowHeight = 600;//displayMode.h;
	window = SDL_CreateWindow(NULL, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, windowWidth, windowHeight, SDL_WINDOW_BORDERLESS);
	if (!window)
	{
		std::cerr << "Error initializing window." << "\n";
		return;
	}
	
	renderer = SDL_CreateRenderer(window, -1, 0);
	if (!renderer)
	{
		std::cerr << "Error initializing renderer." << "\n";
	}
	//SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);
	isRunning = true;
}
glm::vec2 playerPos;
glm::vec2 playerVel;
SDL_Rect player;
void Game::Setup()
{
	playerPos = glm::vec2(10, 10);
	playerVel = glm::vec2(0.5, 0);
	player = { (int)playerPos.x, (int)playerPos.y, 20,20 };
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
	int timeToWait = millisecondsPerFrame - (SDL_GetTicks() - millisecondsPreviousFrame);
	if (timeToWait > 0 && timeToWait <= millisecondsPerFrame) {
		SDL_Delay(timeToWait);
	}

	millisecondsPreviousFrame = SDL_GetTicks();
	playerPos += playerVel;
	player = { (int)playerPos.x, (int)playerPos.y, 20,20 };
}


void Game::Render()
{
	SDL_SetRenderDrawColor(renderer, 21, 21, 21, 255);
	SDL_RenderClear(renderer);



	// Use forward slashes or raw string literal to avoid escape sequence issues
	SDL_Surface* surface = IMG_Load("assets/images/tank-tiger-right.png");

	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

	SDL_FreeSurface(surface);
	SDL_RenderCopy(renderer, texture, NULL, &player);

	SDL_RenderPresent(renderer);
	SDL_DestroyTexture(texture);
}


void Game::Destroy()
{
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}
