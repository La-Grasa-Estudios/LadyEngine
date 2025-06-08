#pragma once
#include <memory>
#include <SDL.h>
#include "..\core\Coordinator.hpp"
#include "..\systems\TransformSystem.hpp"
#include "..\systems\RenderSystem.hpp"
#include "..\systems\AnimationTransitionSystem.hpp"
#include "..\systems\AnimationPlaybackSystem.hpp"
#include "..\systems\RequestAnimationSystem.hpp"
#include "..\Animation\AnimationData.hpp"

class App
{
public:
	void Run();

private:
	void Init();
	void SetComponents();
	void RegisterSystems();
	void UpdateSystems(SDL_Renderer* renderer, float deltaTime, const Uint8* state);
	void CreateEntity(std::unordered_map<states, Animation> animations);

	std::shared_ptr<RenderSystem> renderSystem = nullptr;
	std::shared_ptr<RequestAnimationSystem> transitionSystem = nullptr;
	std::shared_ptr<AnimationPlaybackSystem> animationPlaybackSystem = nullptr;
	std::shared_ptr<AnimationTransitionSystem> animationTransitionSystem = nullptr;
	std::shared_ptr<TransformSystem> transformSystem = nullptr;

};