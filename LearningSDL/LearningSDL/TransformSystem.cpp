#include "TransformSystem.hpp"
#include "Coordinator.hpp"
#include "TransformComponent.hpp"
extern Coordinator gCoordinator;

void MovementSystem::Init()
{

}

void MovementSystem::Update(float deltaTime)
{
	for (auto const& entity : mEntities) 
	{
		auto transform = gCoordinator.GetComponent<Transform>(entity);

		transform.position.x += (180.0f * deltaTime);
	}
}