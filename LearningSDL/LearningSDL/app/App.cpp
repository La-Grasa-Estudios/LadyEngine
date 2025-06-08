#include "App.hpp"
#include "SDLWindow.hpp"
#include "..\components\TransformComponent.hpp"
#include "..\components\RenderableComponent.hpp"
#include "..\components\AnimationStateComponent.hpp"
#include "..\components\AnimationDataComponent.hpp"
#include "..\components\AnimationRequestComponent.hpp"
#include "..\tools\generic_tools.hpp"

Coordinator gCoordinator;

void App::Run()
{
    Init();

    SDLWindow Window("LadyEngine");
    SDLWindow::s_Instance->p_Width = 1600;
    SDLWindow::s_Instance->p_Height = 900;
    SDLWindow::s_Instance->Init();

    std::vector<SDL_Rect> WalkAnimation;
    std::vector<SDL_Rect> AttackAnimation;
    std::vector<SDL_Rect> TalkAnimation;
    std::vector<SDL_Rect> IdleAnimation;
    std::vector<SDL_Rect> ProtectionAnimation;

    generic_tools::CreateRect(WalkAnimation, 12);
    generic_tools::CreateRect(AttackAnimation, 8);
    generic_tools::CreateRect(TalkAnimation, 11);
    generic_tools::CreateRect(IdleAnimation, 9);
    generic_tools::CreateRect(ProtectionAnimation, 4);

    std::unordered_map<states, Animation> Animations;

    Animations[IDLE] = Animation{ "images/Girl_1/Idle.png", IdleAnimation, 1.0f / 9.0f, true, true, false };
    Animations[WALK] = Animation{ "images/Girl_1/Walk.png", WalkAnimation, 1.0f / 12.0f, true, true, false };
    Animations[ATTACK] = Animation{ "images/Girl_1/Attack.png", AttackAnimation, 1.0f / 8.0f, false, false, false };
    Animations[COVER] = Animation{ "images/Girl_1/Protection.png", ProtectionAnimation, 1.0f / 4.0f, false, false, true };
    Animations[TALK] = Animation{ "images/Girl_1/Talk.png", TalkAnimation, 1.0f / 11.0f, true, true, false };

    int ENTTQUANTITY = 1;
    std::cout << "\nPuedes hacer click para crear una entidad\n";
    std::cout << "Selecciona la cantidad de Entidades que deseas renderizar (cap 99999): ";
    //std::cin >> ENTTQUANTITY;

    for (int i = 0; i < ENTTQUANTITY; ++i)
    {
        CreateEntity(Animations);
    }

    Uint32 previous_time = SDL_GetTicks();
    float deltaTime = 0.0f;
    const Uint8* state = SDL_GetKeyboardState(NULL);
    SDL_Renderer* renderer = SDLWindow::s_Instance->GetRenderer();

    while (!SDLWindow::s_Instance->ShouldClose()) 
    {
        Uint32 current_time = SDL_GetTicks();
        deltaTime = (current_time - previous_time) / 1000.0f;
        previous_time = current_time;
        SDLWindow::s_Instance->Update();
        SDLWindow::s_Instance->Clear();
        UpdateSystems(renderer, deltaTime, state);
        SDLWindow::s_Instance->Render();
    }

}

void App::Init() 
{
    SetComponents();
    RegisterSystems();
}

void App::SetComponents()
{
    gCoordinator.RegisterComponent<Transform>();
    gCoordinator.RegisterComponent<Renderable>();
    gCoordinator.RegisterComponent<AnimationRequest>();
    gCoordinator.RegisterComponent<AnimationData>();
    gCoordinator.RegisterComponent<AnimationState>();
}

void App::UpdateSystems(SDL_Renderer* renderer, float deltaTime, const Uint8* state)
{
    bool KeyIsPressed = false;

    for (int i = 0; i < 256; ++i)
    {
        if (state[i] == 1)
        {
            KeyIsPressed = true;
        }
    }

    if (state[SDL_SCANCODE_D])
    {
        transitionSystem->Update(WALK);
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

    animationPlaybackSystem->Update(deltaTime);
    animationTransitionSystem->Update();
    renderSystem->Update(renderer);


}

void App::RegisterSystems()
{
    transformSystem = gCoordinator.RegisterSystem<TransformSystem>();
    {
        Signature transformSignature;
        transformSignature.set(gCoordinator.GetComponentType<Transform>());
        gCoordinator.SetSystemSignature<TransformSystem>(transformSignature);
    }

    animationTransitionSystem = gCoordinator.RegisterSystem<AnimationTransitionSystem>();
    {
        Signature animationTransitionSignature;
        animationTransitionSignature.set(gCoordinator.GetComponentType<AnimationRequest>());
        animationTransitionSignature.set(gCoordinator.GetComponentType<AnimationState>());
        animationTransitionSignature.set(gCoordinator.GetComponentType<AnimationData>());
        gCoordinator.SetSystemSignature<AnimationTransitionSystem>(animationTransitionSignature);
    }

    animationPlaybackSystem = gCoordinator.RegisterSystem<AnimationPlaybackSystem>();
    {
        Signature animationPlaybackSignature;
        animationPlaybackSignature.set(gCoordinator.GetComponentType<AnimationRequest>());
        animationPlaybackSignature.set(gCoordinator.GetComponentType<AnimationState>());
        animationPlaybackSignature.set(gCoordinator.GetComponentType<AnimationData>());
        animationPlaybackSignature.set(gCoordinator.GetComponentType<Renderable>());

        gCoordinator.SetSystemSignature<AnimationPlaybackSystem>(animationPlaybackSignature);
    }

    transitionSystem = gCoordinator.RegisterSystem<RequestAnimationSystem>();
    {
        Signature transitionSignature;
        transitionSignature.set(gCoordinator.GetComponentType<AnimationRequest>());
        gCoordinator.SetSystemSignature<RequestAnimationSystem>(transitionSignature);

    }

    renderSystem = gCoordinator.RegisterSystem<RenderSystem>();
    {
        Signature renderSignature;
        renderSignature.set(gCoordinator.GetComponentType<Transform>());
        renderSignature.set(gCoordinator.GetComponentType<Renderable>());
        gCoordinator.SetSystemSignature<RenderSystem>(renderSignature);
    }
}

void App::CreateEntity(std::unordered_map<states, Animation> animations) 
{
    Entity entity = gCoordinator.CreateEntity();
    gCoordinator.AddComponent<Transform>(entity,

        { .position = Vec2{static_cast<float>(generic_tools::random_number(0,1400)), static_cast<float>(generic_tools::random_number(0, 800)) }, .scale = Vec2{1.0f,1.0f} });
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
            .CurrentTime = 0.0f,
        });
    gCoordinator.AddComponent<Renderable>
        (entity,
            {
            .texture_path = "",
            .color_mod = { 255, 255, 255, 255 },
            .angle = 0.0f,
            .flip = SDL_FLIP_NONE,
            .origin = { 0, 0 },
            .renderLayer = 1,
            .isVisible = true
            }
        );
}