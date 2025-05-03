#include "AnimationPlaybackSystem.hpp"
#include "..\components\AnimationRequestComponent.hpp"
#include "..\components\AnimationStateComponent.hpp"
#include "..\components\AnimationDataComponent.hpp"
#include "..\components\RenderableComponent.hpp"
#include "..\core\Coordinator.hpp"

#include <algorithm>
#include <cmath>

extern Coordinator gCoordinator;

void AnimationPlaybackSystem::Init() 
{
	
}

void AnimationPlaybackSystem::Update(float deltaTime)
{
	for (Entity entity : mEntities)
	{
		auto& animationRequest = gCoordinator.GetComponent<AnimationRequest>(entity);
		auto& animationState = gCoordinator.GetComponent<AnimationState>(entity);
		auto& animationData = gCoordinator.GetComponent<AnimationData>(entity);
		auto& renderable = gCoordinator.GetComponent<Renderable>(entity);

		Animation& currentAnimation = animationData.Animations[animationState.CurrentState];

		animationState.CurrentTime += deltaTime;
		
		float animationDuration = currentAnimation.frameRate * currentAnimation.frames.size();

		if (animationState.CurrentTime >= animationDuration)
		{
			if (!animationState.InLoop) 
			{
				animationState.CurrentTime = animationDuration;
			}
			else 
			{
				animationState.CurrentTime = 0.0f;
			}
		}

		currentAnimation.currentFrame = (int)std::floor(animationState.CurrentTime / currentAnimation.frameRate);
		
		if (!animationState.InLoop && animationState.CurrentTime >= animationDuration) 
		{
			currentAnimation.currentFrame = std::min(currentAnimation.currentFrame, (int)currentAnimation.frames.size() - 1);
		}
		else 
		{
			currentAnimation.currentFrame = currentAnimation.currentFrame % currentAnimation.frames.size();
		}

		renderable.srcRect = currentAnimation.frames[currentAnimation.currentFrame];
		renderable.texture_path = currentAnimation.TexturePath;

	}
}
