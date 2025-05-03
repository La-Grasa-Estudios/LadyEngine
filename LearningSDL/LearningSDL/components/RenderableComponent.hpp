#pragma once
#include <string>
#include <SDL3/SDL.h>
struct Renderable
{
	std::string texture_path;
	SDL_Rect srcRect{ 0,0,0,0 };
	SDL_Color color_mod = { 255,255,255,255 };
	double angle;
	SDL_FlipMode flip = SDL_FLIP_NONE;
	SDL_Point origin = { 0,0 };
	int renderLayer = 0;
	bool isVisible = true;
};