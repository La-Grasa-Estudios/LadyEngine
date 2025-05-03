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
    int CurrentFrame;
    states NextState;
}; 



