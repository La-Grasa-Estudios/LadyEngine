#pragma once
#include "..\core\System.hpp"
#include "..\Data\States.hpp" 

class RequestAnimationSystem : public System
{
public:
	void Init();
	void Update(states STATE);
};