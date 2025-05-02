#pragma once
#include "..\Data\States.hpp"
#include <queue>

struct AnimationRequest
{
	std::queue<states> AnimationQueue;
};
