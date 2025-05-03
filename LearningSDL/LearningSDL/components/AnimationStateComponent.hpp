#pragma once
#include <array>
#include <string>
#include <queue>
#include <SDL3/SDL.h>
#include "..\Data\States.hpp"

struct AnimationState 
{
    states CurrentState;
    float CurrentTime;
    float CurrentFrameTime;
    int CurrentFrame = 0;
}; 



