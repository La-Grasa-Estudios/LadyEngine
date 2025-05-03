#include "RenderSystem.hpp"
#include "..\core\Coordinator.hpp"
#include "..\components\RenderableComponent.hpp"
#include "..\components\TransformComponent.hpp"
#include "..\ResourceManager.hpp"

#include<iostream>

extern Coordinator gCoordinator;

void RenderSystem::Init()
{

}
//fix
void RenderSystem::Update(SDL_Renderer* renderer)
{
	for (Entity entity : mEntities)
	{
		auto renderable = gCoordinator.GetComponent<Renderable>(entity);
		auto transform = gCoordinator.GetComponent<Transform>(entity);

		SDL_FRect dstRect;

		dstRect.x = static_cast<float>(transform.position.x);
		dstRect.y = static_cast<float>(transform.position.y);
		dstRect.w = static_cast<float>(transform.scale.x * renderable.srcRect.w);
		dstRect.h = static_cast<float>(transform.scale.y * renderable.srcRect.h);

		SDL_Texture* texture = ResourceManager::get_resource_manager().get_texture(renderer, renderable.texture_path);
		
		SDL_SetTextureColorMod(texture, renderable.color_mod.r, renderable.color_mod.g, renderable.color_mod.b);

		SDL_FRect srcRect;

		srcRect.x = renderable.srcRect.x;
		srcRect.y = renderable.srcRect.y;
		srcRect.w = renderable.srcRect.w;
		srcRect.h = renderable.srcRect.h;

		SDL_FPoint origin;
		origin.x = renderable.origin.x;
		origin.y = renderable.origin.y;

		SDL_RenderTextureRotated(renderer, texture, &srcRect, &dstRect, renderable.angle, &origin, renderable.flip);

	}
}