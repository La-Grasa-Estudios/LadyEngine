#include "AnimationPlaybackSystem.hpp"
#include "..\components\AnimationRequestComponent.hpp"
#include "..\components\AnimationStateComponent.hpp"
#include "..\components\AnimationDataComponent.hpp"
#include "..\components\RenderableComponent.hpp"

#include "..\core\Coordinator.hpp"

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
		animationState.CurrentFrameTime += deltaTime;

		//Calculate the frame to renderize

		while (animationState.CurrentFrameTime >= currentAnimation.frameRate) 
		{
			animationState.CurrentFrameTime -= currentAnimation.frameRate;
			animationState.CurrentFrame++;
		}

		if (!currentAnimation.loop)
		{
			animationState.CurrentFrame = std::min(animationState.CurrentFrame, (int)currentAnimation.frames.size() - 1);
		}
		else
		{
			animationState.CurrentFrame = animationState.CurrentFrame % currentAnimation.frames.size();
		}

		renderable.srcRect = currentAnimation.frames[animationState.CurrentFrame];
		renderable.texture_path = currentAnimation.TexturePath;
	}

}
