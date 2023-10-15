#pragma once
#include "badgepirates/AnimationInterface.hpp"

class ChaseAnimation : public AnimationInterface
{
public:
    ChaseAnimation(const int *all_leds, int num_leds);

    virtual void setup() override;
    virtual void loop(unsigned long currentMillis) override;

private:
    const int *leds;
    int numLeds;
    unsigned long previousMillis;
    unsigned int interval;
    int currentLed;
};
