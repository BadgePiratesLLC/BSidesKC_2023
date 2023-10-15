// comet.hpp
#pragma once
#include "badgepirates/AnimationInterface.hpp"

class CometAnimation : public AnimationInterface
{
private:
    const int *leds;
    int numLeds;
    unsigned long previousMillis;
    int interval;
    int cometPosition;
    int direction;
    int cometSize; // Determines how long the tail of the comet is.

public:
    CometAnimation(const int *all_leds, int num_leds);
    virtual void setup() override;
    virtual void loop(unsigned long currentMillis) override;
};
