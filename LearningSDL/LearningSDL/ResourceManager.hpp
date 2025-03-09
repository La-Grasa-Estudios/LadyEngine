#pragma once
#ifndef RESOURCEMANAGER_HPP
#define RESOURCEMANAGER_HPP
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <unordered_map>
#include <string>
class ResourceManager
{
private:
	ResourceManager();
	~ResourceManager();
	std::unordered_map<std::string, SDL_Surface*> surfaces;
public:
	static ResourceManager& get_resource_manager();
	SDL_Surface* get_surface(std::string path);
	
};

#endif //RESOURCEMANAGER_HPP