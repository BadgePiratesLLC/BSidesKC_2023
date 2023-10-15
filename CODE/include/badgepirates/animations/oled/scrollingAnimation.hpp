#pragma once
#include "badgepirates/AnimationInterface.hpp"
#include "badgepirates/oled.hpp"

class ScrollingAnimation : public AnimationInterface
{
public:
    ScrollingAnimation();
    void setup() override;
    void loop(unsigned long currentMillis) override;

private:
    unsigned long previousMillis;
    const long interval = 75;
    int xPos;
};
