#pragma once
#pragma once
#include "..\core\System.hpp"
class AnimationPlaybackSystem : public System
{
public:
	void Init();
	void Update(float deltaTime);
};