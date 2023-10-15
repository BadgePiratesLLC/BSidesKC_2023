// AnimationManager.hpp

#pragma once

#include "AnimationInterface.hpp" // Assuming you have an AnimationInterface header
#include "badgepirates/buttons.hpp"

class AnimationManager
{
public:
    AnimationManager();

    void setup();
    void loop(unsigned long currentMillis);
    bool selectAnimationHack(int animation);

private:
    void setAnimation(AnimationInterface *animation);

    AnimationInterface *animations[3]; // Assuming you have 2 animations for now, you can adjust this number
    int currentAnimationIndex;

    AnimationInterface *currentAnimation;

    Buttons buttons; // The instance of your Buttons class
};
