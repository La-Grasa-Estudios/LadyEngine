#include "SDLWindow.hpp"
#include <cassert>
#include "../imgui/imgui.h"
#include "../imgui/imgui_impl_sdl2.h"
#include "../imgui/imgui_impl_sdlrenderer2.h"
#include <iostream>


SDLWindow::SDLWindow(const char* title)
{
    this->m_Title = title;
    s_Instance = this;
}

SDLWindow::~SDLWindow() 
{
    SDL_DestroyRenderer(m_Renderer);
    SDL_DestroyWindow(m_Window);
    IMG_Quit();
    SDL_Quit();
}

void SDLWindow::Init()
{   
	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER);
	SetWindow();
	SetRenderer();
	IMG_Init(IMG_INIT_PNG);

}

void SDLWindow::Update() 
{

    while (SDL_PollEvent(&m_Event))
    {

        //ImGui_ImplSDL2_ProcessEvent(&m_Event); //it is necessary to do this because in other way ImGui would not handle Events... At least is what a think for now

        if (m_Event.type == SDL_QUIT)
        {
            m_ShouldClose = true;
        }
        if (m_Event.type == SDL_KEYDOWN)
        {
            if (m_Event.key.keysym.sym == SDLK_ESCAPE)
            {
                m_ShouldClose = true;

            }
        }
        if (m_Event.button.button == SDL_BUTTON_LEFT)
        {
            if (m_Event.type == SDL_MOUSEBUTTONDOWN)
            {
                //CreateEntity(Animations);
            }
        }
    }

}

void SDLWindow::Clear()
{
    SDL_RenderClear(m_Renderer);
}

void SDLWindow::Render()
{
    SDL_RenderPresent(m_Renderer);
}

SDL_Window* SDLWindow::SetWindow()
{
    m_Window = SDL_CreateWindow(m_Title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, p_Width, p_Height, p_WindowFlags);
	assert(m_Window != nullptr);
	return m_Window;
}

SDL_Renderer* SDLWindow::SetRenderer()
{
    m_Renderer = SDL_CreateRenderer(m_Window, -1, SDL_RENDERER_ACCELERATED);
	assert(m_Renderer != nullptr);
	return m_Renderer;
}

SDL_Window* SDLWindow::GetWindow()
{
	assert(m_Window != nullptr);
	return m_Window;
}

SDL_Renderer* SDLWindow::GetRenderer()
{
	assert(m_Renderer != nullptr);
	return m_Renderer;
}

