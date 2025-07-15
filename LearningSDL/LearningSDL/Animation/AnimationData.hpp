#pragma once
#include <string>
#include <vector>   
#include <SDL.h>

//const char MAX_ANIMATIONS = 32;

struct Animation
{
    std::string TexturePath;
    std::vector<SDL_Rect> frames;
    float frameRate;
    bool loop;
    bool isInterruptible;
    bool hasToLock = false; //Controls if the animation needs to lock in the last frame if it is not looped and the animation time has ended
    int startFrame = 0;
};