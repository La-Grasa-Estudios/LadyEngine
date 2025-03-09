#pragma once
#ifndef COMPONENTS_HPP
#define COMPONENTS_HPP
#include "LadyMath.hpp"

using ComponentType = std::uint8_t;

const ComponentType MAX_COMPONENTS = 255;

struct Transform
{
	Vec2 postition;
	Vec2 scale;
};

#endif //!COMPONENTS_HPP