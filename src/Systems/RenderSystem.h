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
		std::vector<Entity> zSortedEntities;
		for (auto entity : GetSystemEntities())
		{

			zSortedEntities.push_back(entity);
		}

		std::sort(
			zSortedEntities.begin(),
			zSortedEntities.end(),
			[]( Entity& a, Entity& b)
			{
				return a.GetComponent<SpriteComponent>().zIndex < b.GetComponent<SpriteComponent>().zIndex;
			}
		);
		for (auto entity : zSortedEntities)
		{
			auto transform = entity.GetComponent<TransformComponent>();
			auto sprite = entity.GetComponent<SpriteComponent>();

			
			SDL_Rect sourceRect = sprite.sourceRect;
			SDL_Rect destRect = {
				static_cast<int>(transform.position.x),
				static_cast<int>(transform.position.y),
				static_cast<int>(sprite.width * transform.scale.x),
				static_cast<int>(sprite.height * transform.scale.y)
			};

			SDL_Texture* texure = assetStore->GetTexture(sprite.assetTextureID);
			SDL_RenderCopyEx(renderer, texure, &sourceRect, &destRect,transform.rotation, NULL, SDL_FLIP_NONE);
		}
	}

};

#endif // !RENDERSYSTEM_H
