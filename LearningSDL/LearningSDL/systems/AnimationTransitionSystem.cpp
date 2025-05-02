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

			if (nextState != animationState.CurrentState) 
			{
				Animation& currentAnimation = animationData.Animations[animationState.CurrentState];
				Animation& nextAnimation = animationData.Animations[nextState];

				if (animationState.CurrentTime >= currentAnimation.duration || animationState.IsInterruptible) 
				{
					animationState.CurrentTime = 0.0f;
					animationState.InLoop = nextAnimation.loop;
					animationState.IsInterruptible = nextAnimation.isInterruptible;
					animationState.CurrentState = nextState;
				}
				else 
				{
					return;
				}

			}
		}

	}
}

