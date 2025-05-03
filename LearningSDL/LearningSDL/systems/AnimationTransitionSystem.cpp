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

		Animation& currentAnimation = animationData.Animations[animationState.CurrentState];
		float animationDuration = currentAnimation.frameRate * currentAnimation.frames.size();

		if (animationState.CurrentTime >= animationDuration && !currentAnimation.loop && currentAnimation.nextState != NONE)
		{
			Animation& nextAnimation = animationData.Animations[currentAnimation.nextState];
			animationState.CurrentTime = 0.0f;
			animationState.CurrentState = currentAnimation.nextState;
			animationState.CurrentFrame = 0;
		}

		while (!animationRequest.AnimationQueue.empty())
		{
			states nextState = animationRequest.AnimationQueue.front();
			animationRequest.AnimationQueue.pop();

			if (nextState != animationState.CurrentState) 
			{
				Animation& nextAnimation = animationData.Animations[nextState];

				if (currentAnimation.isInterruptible)
				{
					animationState.CurrentTime = 0.0f;
					animationState.CurrentState = nextState;
					animationState.CurrentFrame = 0;
				}
				else 
				{
					return;
				}

			}
		}

	}
}

