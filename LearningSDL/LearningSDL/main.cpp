#define SDL_MAIN_HANDLED
#include <SDL3/SDL.h> 
#include <SDL3/SDL_image.h> 
#include "core\Coordinator.hpp"
#include "systems\TransformSystem.hpp"
#include "systems\RenderSystem.hpp"
#include "systems\AnimationTransitionSystem.hpp"
#include "systems\AnimationPlaybackSystem.hpp"
#include "systems\RequestAnimationSystem.hpp"
#include "components\TransformComponent.hpp"
#include "components\RenderableComponent.hpp"
#include "components\AnimationStateComponent.hpp"
#include "components\AnimationDataComponent.hpp"
#include "components\AnimationRequestComponent.hpp"
#include "tools\generic_tools.hpp"

#include <vector>

Coordinator gCoordinator;

//CORRECT ANIMATIONSYSTEM

Entity CreateEntity(std::array<Animation, MAX_ANIMATIONS> animations)
{
    Entity entity = gCoordinator.CreateEntity();

    gCoordinator.AddComponent<Transform>(entity,
        { .position = Vec2{static_cast<float>(generic_tools::random_number(0,  1500)), static_cast<float>(generic_tools::random_number(0, 800)) }, .scale = Vec2{1.0f,1.0f} });

    gCoordinator.AddComponent<AnimationRequest>(entity,
        {
			.AnimationQueue = std::queue<states>(),
        });

    gCoordinator.AddComponent<AnimationData>(entity,
        {
            .Animations = animations
        });

    gCoordinator.AddComponent<AnimationState>(entity,
        {
			.CurrentState = states::IDLE,
			.CurrentTime = 0.0f,
			.CurrentFrameTime = 0.0f,
			.CurrentFrame = 0,
        });


    gCoordinator.AddComponent<Renderable>
        (entity,
            {
            .texture_path = "",
            .srcRect = animations[0].frames[0],
            .color_mod = { 255, 255, 255, 255 },
            .angle = 0.0f,
            .flip = SDL_FLIP_NONE,
            .origin = { 0, 0 },
            .renderLayer = 1,
            .isVisible = true
            }
        );

    return entity;
}

int main()
{
    int ENTTQUANTITY;
    std::cout << "\nPuedes hacer click para crear una entidad\n";
    std::cout << "Selecciona la cantidad de Entidades que deseas renderizar (cap 99999): ";
    std::cin >> ENTTQUANTITY;

    srand(time(0));

    gCoordinator.RegisterComponent<Transform>();
    gCoordinator.RegisterComponent<Renderable>();
    gCoordinator.RegisterComponent<AnimationRequest>();
    gCoordinator.RegisterComponent<AnimationData>();
    gCoordinator.RegisterComponent<AnimationState>();

    auto TranformSystem = gCoordinator.RegisterSystem<TransformSystem>();
    {
        Signature transformSignature;
        transformSignature.set(gCoordinator.GetComponentType<Transform>());
        gCoordinator.SetSystemSignature<TransformSystem>(transformSignature);
    }

	auto animationTransitionSystem = gCoordinator.RegisterSystem<AnimationTransitionSystem>();
	{
		Signature animationTransitionSignature;
		animationTransitionSignature.set(gCoordinator.GetComponentType<AnimationRequest>());
		animationTransitionSignature.set(gCoordinator.GetComponentType<AnimationState>());
		animationTransitionSignature.set(gCoordinator.GetComponentType<AnimationData>());
		gCoordinator.SetSystemSignature<AnimationTransitionSystem>(animationTransitionSignature);
	}

    auto animationPlaybackSystem = gCoordinator.RegisterSystem<AnimationPlaybackSystem>();
    {
        Signature animationPlaybackSignature;
        animationPlaybackSignature.set(gCoordinator.GetComponentType<AnimationRequest>());
        animationPlaybackSignature.set(gCoordinator.GetComponentType<AnimationState>());
        animationPlaybackSignature.set(gCoordinator.GetComponentType<AnimationData>());
        animationPlaybackSignature.set(gCoordinator.GetComponentType<Renderable>()); 

        gCoordinator.SetSystemSignature<AnimationPlaybackSystem>(animationPlaybackSignature);
    }

    auto transitionSystem = gCoordinator.RegisterSystem<RequestAnimationSystem>();
    {
        Signature transitionSignature;
		transitionSignature.set(gCoordinator.GetComponentType<AnimationRequest>());
        gCoordinator.SetSystemSignature<RequestAnimationSystem>(transitionSignature);

    }

    auto renderSystem = gCoordinator.RegisterSystem<RenderSystem>();
    {
        Signature renderSignature;
        renderSignature.set(gCoordinator.GetComponentType<Transform>());
        renderSignature.set(gCoordinator.GetComponentType<Renderable>());
        gCoordinator.SetSystemSignature<RenderSystem>(renderSignature);
    }

    SDL_Init(SDL_INIT_VIDEO);
	SDL_Window* window = SDL_CreateWindow("ECS test", 1600, 900, 0);

	SDL_Renderer* renderer = SDL_CreateRenderer(window, 0);
    
    std::vector<SDL_Rect> WalkAnimation;
    std::vector<SDL_Rect> AttackAnimation;
    std::vector<SDL_Rect> TalkAnimation;
    std::vector<SDL_Rect> IdleAnimation;
    std::vector<SDL_Rect> ProtectionAnimation;

    generic_tools::CreateRect(WalkAnimation, 12);
    generic_tools::CreateRect(AttackAnimation, 8);
    generic_tools::CreateRect(TalkAnimation, 11);
    generic_tools::CreateRect(IdleAnimation, 9 );
    generic_tools::CreateRect(ProtectionAnimation, 4);


	
    std::array<Animation, MAX_ANIMATIONS> Animations = 
    {  
       Animation{"images/Girl_1/Idle.png", IdleAnimation, 1.0f / 9.0f, 1.2f, true, true},
       Animation{"images/Girl_1/Walk.png", WalkAnimation, 1.0f / 12.0f, 1.0f, true, true, IDLE},  
       Animation{"images/Girl_1/Attack.png", AttackAnimation, 1.0f / 8.0f, 1.2f, false, false, IDLE}, 
       Animation{"images/Girl_1/Protection.png", ProtectionAnimation, 1.0f / 4.0f, 1.2f, false, true, NONE},
       Animation{"images/Girl_1/Talk.png", TalkAnimation, 1.0f / 11.0f, 1.2f, false, true, IDLE} 
    };
		
    bool run = true;
    SDL_Event event;
    Uint32 previous_time = SDL_GetTicks();
    float deltaTime = 0.0f;

    Entity entity = CreateEntity(Animations);;

	const bool* state = SDL_GetKeyboardState(NULL);

    while (run)
    {
        Uint32 current_time = SDL_GetTicks();
        deltaTime = (current_time - previous_time) / 1000.0f;
        previous_time = current_time;

        bool KeyIsPressed = false;
        
        for (int i = 0; i < 256; ++i) 
        {
            if(state[i] == 1)
            {
				KeyIsPressed = true;
			}

        }

        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_EVENT_QUIT)
            {
                run = false;
            }
        }

        auto& renderable = gCoordinator.GetComponent<Renderable>(entity);
        
        std::cout << "Is key Pressed? : " << KeyIsPressed << "\n";
        if (state[SDL_SCANCODE_D])
        {
            transitionSystem->Update(WALK);
            renderable.flip = SDL_FLIP_NONE;
        }
        if (state[SDL_SCANCODE_A])
        {
            transitionSystem->Update(WALK);
            renderable.flip = SDL_FLIP_HORIZONTAL;
        }
        if (state[SDL_SCANCODE_T])
        {
            transitionSystem->Update(ATTACK);
        }
        if (state[SDL_SCANCODE_Y])
        {
            transitionSystem->Update(TALK);
        }
        if (state[SDL_SCANCODE_F])
        {
            transitionSystem->Update(COVER);
        }
        if (!KeyIsPressed)
        {
            transitionSystem->Update(IDLE);
        }

        SDL_RenderClear(renderer);
		animationTransitionSystem->Update();
		animationPlaybackSystem->Update(deltaTime);
        //TranformSystem->Update(deltaTime);
        renderSystem->Update(renderer);
        SDL_RenderPresent(renderer);
        
        
    }

}