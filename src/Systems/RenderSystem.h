#ifndef RENDERSYSTEM_H
#define RENDERSYSTEM_H
#include "../ECS/ECS.h"
#include "../Components/TransformComponent.h"
#include "../Components/SpriteComponent.h"
#include <spdlog/spdlog.h>
#include "../AssetStore/AssetStore.h"
struct RenderSystem :public System
{
	RenderSystem()
	{
		RequireComponent<TransformComponent>();
		RequireComponent<SpriteComponent>();
	}

	void Update(SDL_Renderer* renderer, std::unique_ptr<AssetStore>& assetStore)
	{
		for (auto entity : GetSystemEntities())
		{
			auto transform = entity.GetComponent<TransformComponent>();
			auto sprite = entity.GetComponent<SpriteComponent>();

			
			SDL_Rect sourceRect = sprite.sourceRect;
			SDL_Rect destRect = { transform.position.x, transform.position.y, sprite.width, sprite.height };

			SDL_Texture* texure = assetStore->GetTexture(sprite.assetTextureID);
			SDL_RenderCopyEx(renderer, texure, &sourceRect, &destRect,transform.rotation, NULL, SDL_FLIP_NONE);
		}
	}

};

#endif // !RENDERSYSTEM_H
