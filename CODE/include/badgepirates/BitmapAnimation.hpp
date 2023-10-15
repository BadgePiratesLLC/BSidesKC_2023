#pragma once
#include "AnimationInterface.hpp"
#include "badgepirates/animations/gifs/trollDance.hpp" // Include bitmap frames here

class BitmapAnimation : public AnimationInterface
{
private:
    const unsigned char *const *frames;
    int frameCount;
    int frameWidth;
    int frameHeight;
    int frameDuration;
    int currentFrameIndex;
    unsigned long lastFrameChangeMillis;
    // other necessary variables

public:
    BitmapAnimation(const unsigned char *const *frames, int frameCount);
    virtual void setup() override;
    virtual void loop(unsigned long currentMillis) override;
    void nextFrame();
};