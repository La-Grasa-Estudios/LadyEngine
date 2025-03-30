#pragma once
#include "..\core\System.hpp"
class MovementSystem : public System
{
public:
	void Init();
	void Update(float deltaTime);
};