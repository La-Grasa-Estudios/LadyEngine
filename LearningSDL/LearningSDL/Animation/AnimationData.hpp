#pragma once
#include <string>
#include <vector>   
#include <SDL3/SDL.h>

const char MAX_ANIMATIONS = 8;

struct Animation
{
    std::string TexturePath;
    std::vector<SDL_Rect> frames;
    float frameRate = 0.0f;
    float duration = 0.0f; //Animation duration
    bool loop = true;
    bool isInterruptible;
    states nextState = IDLE;
};
