#pragma once
#include <SDL.h>
#include <SDL_image.h>
class SDLWindow
{
public:
	unsigned int p_Width = {};
	unsigned int p_Height = {};
	SDL_WindowFlags p_WindowFlags = {};
	static inline SDLWindow* s_Instance = nullptr;

	SDLWindow(const char* title);
	~SDLWindow();

	void Init();
	void Update();
	void Clear();
	void Render();
	inline bool ShouldClose() { return m_ShouldClose; };

	SDL_Renderer* SetRenderer();
	SDL_Window* SetWindow();
	SDL_Window* GetWindow();
	SDL_Renderer* GetRenderer();

private:
	SDL_Renderer* m_Renderer = nullptr;
	SDL_Window* m_Window = nullptr;
	bool m_ShouldClose = false;
	SDL_Event m_Event = {};
	const char* m_Title = nullptr;
	
	
};