#pragma once
#include "badgepirates/AnimationInterface.hpp"

class BreathAnimation : public AnimationInterface
{
private:
    const int *leds; // Make this a pointer to const int
    int numLeds;
    unsigned long previousMillis;
    int interval;
    int brightness;
    bool increasing;

public:
    BreathAnimation(const int *all_leds, int num_leds); // Change the parameter here as well
    void setup() override;
    void loop(unsigned long currentMillis) override;
};
