#include "TexturedRectangle.hpp"
#include "ResourceManager.hpp"
#include <iostream>
TexturedRectangle::TexturedRectangle(SDL_Renderer* renderer, std::string file)
{
	render = renderer;
	path = file;

}

TexturedRectangle::~TexturedRectangle()
{
	render = nullptr;
	SDL_DestroyTexture(texture);
	texture = nullptr;
}

void TexturedRectangle::set_attributes(int X, int Y, int W, int H)
{
	rectangle.x = X;
	rectangle.y = Y;
	rectangle.w = W;
	rectangle.h = H;

}

void TexturedRectangle::set_texture()
{
	texture = SDL_CreateTextureFromSurface(render, ResourceManager::get_resource_manager().get_surface(path));
}

void TexturedRectangle::set_render_copy()
{
	SDL_FRect rect;
	rect.x = rectangle.x;
	rect.y = rectangle.y;
	rect.w = rectangle.w;
	rect.h = rectangle.h;
	SDL_RenderTexture(render, texture, NULL, &rect);
}

bool TexturedRectangle::is_coliding_with(TexturedRectangle& obj)
{
	return SDL_HasRectIntersection(&rectangle, obj.get_rect());
}

SDL_Rect* TexturedRectangle::get_rect()
{
	return &rectangle;
}