#define SDL_MAIN_HANDLED
#include <SDL.h> 
#include <SDL_image.h>
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
#include "imgui/imgui.h"
#include "imgui/imgui_impl_sdl2.h"
#include "imgui/imgui_impl_sdlrenderer2.h"

Coordinator gCoordinator;

void CreateEntity(std::unordered_map<states, Animation> animations)
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

int main()
{

    uint16_t x = GetSystemMetrics(SM_CXSCREEN);
    uint16_t y = GetSystemMetrics(SM_CYSCREEN);

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

    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER);

    SDL_WindowFlags window_flags = (SDL_WindowFlags)(SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
    SDL_Window* window = SDL_CreateWindow("ECS test", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1600, 900, window_flags);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    IMG_Init(IMG_INIT_PNG);

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

    bool run = true;
    SDL_Event event;
    Uint32 previous_time = SDL_GetTicks();
    float deltaTime = 0.0f;

    int ENTTQUANTITY = 1;
    std::cout << "\nPuedes hacer click para crear una entidad\n";
    std::cout << "Selecciona la cantidad de Entidades que deseas renderizar (cap 99999): ";
    //std::cin >> ENTTQUANTITY;

    for (int i = 0; i < ENTTQUANTITY; ++i)
    {
        CreateEntity(Animations);
    }

    const Uint8* state = SDL_GetKeyboardState(NULL);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;


    ImGui::StyleColorsDark();

    ImGui_ImplSDL2_InitForSDLRenderer(window, renderer);
    ImGui_ImplSDLRenderer2_Init(renderer);

    bool ImguiWindow = true;
    bool ImguiOtherWindow = false;
    bool CheckBox = false;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    bool TestW = true;

    int s_int = 0;
    float s_float = 0;
    char buffer[128] = {};
    while (run)
    {

        Uint32 current_time = SDL_GetTicks();
        deltaTime = (current_time - previous_time) / 1000.0f;
        previous_time = current_time;

        bool KeyIsPressed = false;

        while (SDL_PollEvent(&event))
        {

            ImGui_ImplSDL2_ProcessEvent(&event);

            if (event.type == SDL_QUIT)
            {
                run = false;
            }
            if (event.type == SDL_KEYDOWN)
            {
                if (event.key.keysym.sym == SDLK_ESCAPE)
                {
                    run = false;

                }
            }
            if (event.button.button == SDL_BUTTON_LEFT)
            {
                if (event.type == SDL_MOUSEBUTTONDOWN)
                {
                    //CreateEntity(Animations);
                }
            }
        }

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

        ImGui_ImplSDLRenderer2_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();

        if (ImguiWindow)
            ImGui::ShowDemoWindow(&ImguiWindow);
        {
            static float f = 0.0f;
            static int counter = 0;

            ImGui::Begin("LadyEngine");

            ImGui::Text("Achieved with LadyEngine");
            ImGui::Checkbox("ECS", &ImguiWindow);
            ImGui::Checkbox("System", &ImguiOtherWindow);

            ImGui::SliderFloat("float", &f, 0.0f, 1.0f);
            ImGui::ColorEdit3("Clear Color", (float*)&clear_color);

            if (ImGui::Button("Button"))
                counter++;
            ImGui::SameLine();
            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
            ImGui::End();

        }

        if (ImguiOtherWindow)
        {
            ImGui::Begin("Another LadyEngine Window", &ImguiOtherWindow);
            ImGui::Text("LadyEngine Hello");
            if (ImGui::Button("Close Me"))
                ImguiOtherWindow = false;
            ImGui::End();
            
        }

        {
            ImGui::SetNextWindowSize(ImVec2(500,500));
            //ImGui::SetNextWindowPos(ImVec2(200,100));
            ImGui::Begin("Test Window", &TestW, ImGuiWindowFlags_MenuBar);
            ImGui::SetCursorPosX(ImGui::GetWindowWidth() / 2 - ImGui::CalcTextSize("Me gustan las trans").x / 2);
            ImGui::Text("Me gustan las trans");
            if (ImGui::Button("CLOSE", ImVec2(50, 30)))
            {
                run = false;
            }
            ImGui::Checkbox("Eres trans", &CheckBox);
            ImGui::End();
            if (CheckBox)
            {
                
                ImGui::SetNextWindowSize(ImVec2(500, 500));
                ImGui::Begin("##trans");
                ImGuiStyle& style = ImGui::GetStyle();
                style.WindowRounding = 10.0f;
                style.Colors[ImGuiCol_WindowBg] = ImVec4(56 / 255.0f, 23 / 255.0f, 42 / 255.0f, 0.5f);
                ImGui::SetCursorPosX(ImGui::GetWindowWidth() / 2 - ImGui::CalcTextSize("Me gustan las trans").x / 2);
                ImGui::Text("Eres muy bonita");
                ImGui::SliderInt("Dick Size", &s_int, 0, 20);
                ImGui::SliderFloat("Balls Size", &s_float, 0, 100);
                ImGui::InputInt("Penis Input", &s_int);
                ImGui::InputFloat("Penis Input2", &s_float);
                ImGui::InputText("Password", buffer, IM_ARRAYSIZE(buffer), ImGuiInputTextFlags_Password);
                ImGui::End();

            }
        }


        SDL_RenderClear(renderer);

        ImGui::Render();
        SDL_RenderSetScale(renderer, io.DisplayFramebufferScale.x, io.DisplayFramebufferScale.y);
        SDL_SetRenderDrawColor(renderer, (uint8_t)(clear_color.x * 255), (uint8_t)(clear_color.y * 255), (uint8_t)(clear_color.z * 255), (uint8_t)(clear_color.w * 255));
        ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData(), renderer);


        //SDL_SetRenderDrawColor(renderer, 0x1E, 0x1E, 0x1E, 0xFF);
        //SDL_RenderClear(renderer);
        animationPlaybackSystem->Update(deltaTime);
        animationTransitionSystem->Update();
        renderSystem->Update(renderer);

        SDL_RenderPresent(renderer);
    }

    ImGui_ImplSDLRenderer2_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();

    return 0;
}