#pragma once
#ifndef RENDERSYSTEM_HPP
#define RENDERSYSTEM_HPP
#include "core\System.hpp"

class RenderSystem : public System
{
public:
	void Init();
	void Update(float deltaTime);
};


#endif //!RENDERSYSTEM_HPP