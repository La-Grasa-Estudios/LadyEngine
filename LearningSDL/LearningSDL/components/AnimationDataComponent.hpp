#pragma once
#include <array>
#include <unordered_map>
#include "..\Animation\AnimationData.hpp" 

struct AnimationData
{
    //std::array<Animation, MAX_ANIMATIONS> Animations;
    std::unordered_map<states, Animation> Animations;
};