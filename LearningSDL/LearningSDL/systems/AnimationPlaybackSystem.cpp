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

		if (animationState.CurrentFrameTime >= currentAnimation.frameRate) 
		{
			animationState.CurrentFrameTime = 0.0f;
			currentAnimation.currentFrame++;

			if (currentAnimation.currentFrame >= currentAnimation.frames.size()) 
			{
				currentAnimation.currentFrame = 0;
			}
			//if (currentAnimation.loop) {
				//currentAnimation.currentFrame = 0;
			//}
			//else 
			//{
				
				//currentAnimation.currentFrame = currentAnimation.frames.size() - 1;
			//}

		}
		/////

		if (animationState.CurrentTime >= currentAnimation.duration && !animationState.InLoop) 
		{
			animationState.CurrentState = animationState.NextState;
			animationState.CurrentTime = 0.0f;
			currentAnimation.currentFrame = 0;

		}
		else if (animationState.CurrentTime >= currentAnimation.duration && animationState.InLoop) 
		{
			animationState.CurrentTime = 0.0f;
			animationState.CurrentFrameTime = 0.0f;
			currentAnimation.currentFrame = 0;
		}

		renderable.srcRect = currentAnimation.frames[currentAnimation.currentFrame];
		renderable.texture_path = currentAnimation.TexturePath;
	}

}
