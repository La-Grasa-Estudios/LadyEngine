#pragma once
#include <array>
#include "..\Animation\AnimationData.hpp" 

struct AnimationData
{
    std::array<Animation, MAX_ANIMATIONS> Animations;
};