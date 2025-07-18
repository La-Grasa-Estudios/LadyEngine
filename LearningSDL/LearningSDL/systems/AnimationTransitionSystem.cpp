#include "AnimationTransitionSystem.hpp"

#include "..\components\AnimationRequestComponent.hpp"
#include "..\components\AnimationStateComponent.hpp"
#include "..\components\AnimationDataComponent.hpp"

#include "..\core\Coordinator.hpp"

extern Coordinator gCoordinator;

void AnimationTransitionSystem::Init()
{

}


void AnimationTransitionSystem::Update()
{
	for (Entity entity : mEntities)
	{
		auto& animationRequest = gCoordinator.GetComponent<AnimationRequest>(entity);
		auto& animationState = gCoordinator.GetComponent<AnimationState>(entity);
		auto& animationData = gCoordinator.GetComponent<AnimationData>(entity);

		if (!animationRequest.AnimationQueue.empty())
		{
			states nextState = animationRequest.AnimationQueue.front();
			animationRequest.AnimationQueue.pop();

			Animation& currentAnimation = animationData.Animations[animationState.CurrentState];
			Animation& nextAnimation = animationData.Animations[nextState];

			bool pressed;
			float animationDuration = currentAnimation.frameRate * currentAnimation.frames.size();

			if (animationState.CurrentTime >= animationDuration)
			{
				pressed = true;
				std::cout << "LA TECLA SE MANTIENE PRESIONADA" << "\n";
				std::cout << "CurrentTime: " << animationState.CurrentTime << "\n";
			}
			else
			{
				pressed = false;
				std::cout << "LA TECLA NO SE EST� PRESIONANDO" << "\n";
				std::cout << "CurrentTime: " << animationState.CurrentTime << "\n";
			}

			if (nextState != animationState.CurrentState)
			{

				if (currentAnimation.isInterruptible)
				{
					animationState.CurrentTime = 0.0f;
					animationState.CurrentState = nextState;

				}
				else
				{
					if (pressed)
					{
						animationState.CurrentTime = 0.0f;
						animationState.CurrentState = nextState;
					}
				}
			}
			else
			{
				if (currentAnimation.isInterruptible)
				{
					return;
				}
				else
				{
					if (pressed)
					{
						if (currentAnimation.hasToLock)
						{
							return;
						}
						else
						{
							animationState.CurrentTime = 0.0f;
						}
					}
				}
			}
		}
	}
}