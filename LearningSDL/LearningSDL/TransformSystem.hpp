#pragma once
#include "System.hpp"
class MovementSystem : public System
{
public:
	void Init();
	void Update(float deltaTime);
};