#pragma once
#include <array>
#include <string>
#include <queue>
#include <SDL.h>
#include "..\Data\States.hpp"

struct AnimationState 
{
    states CurrentState;
    float CurrentTime;
    float CurrentFrameTime;
    bool InLoop;
    bool IsInterruptible;
    states NextState;
}; 



