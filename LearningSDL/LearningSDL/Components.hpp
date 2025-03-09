#pragma once
#ifndef COMPONENTS_HPP
#define COMPONENTS_HPP
#include "LadyMath.hpp"
#include <bitset>

using ComponentType = std::uint8_t;

const ComponentType MAX_COMPONENTS = 255;

using Signature = std::bitset<MAX_COMPONENTS>;

struct Transform
{
	Vec2 postition;
	Vec2 scale;
};

#endif //!COMPONENTS_HPP