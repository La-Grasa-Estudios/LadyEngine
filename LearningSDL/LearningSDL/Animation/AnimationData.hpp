#pragma once
#include <string>
#include <vector>   
#include <SDL.h>

const char MAX_ANIMATIONS = 8;

struct Animation
{
    std::string TexturePath;
    std::vector<SDL_Rect> frames;
    float frameRate = 0.0f;
    float duration = 0.0f; //Animation duration
    int currentFrame = 0;
    bool loop = true;
    bool isInterruptible;
};
