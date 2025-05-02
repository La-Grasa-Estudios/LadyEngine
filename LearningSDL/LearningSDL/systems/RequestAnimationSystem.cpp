#include "RequestAnimationSystem.hpp"
#include "..\components\AnimationRequestComponent.hpp"
#include "..\core\Coordinator.hpp"
#include <iostream>

extern Coordinator gCoordinator;

void RequestAnimationSystem::Init()
{

}
void RequestAnimationSystem::Update(states STATE)
{
	for (Entity entity : mEntities)
	{
		auto& animationRequest = gCoordinator.GetComponent<AnimationRequest>(entity);

		animationRequest.AnimationQueue.push(STATE); 
		std::cout << "\nAnimation request: " << static_cast<int>(STATE) << "\n";
	}
}