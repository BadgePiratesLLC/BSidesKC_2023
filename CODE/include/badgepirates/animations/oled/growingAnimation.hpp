#pragma once
#include "badgepirates/oled.hpp"
#include <Adafruit_GFX.h>
#include "badgepirates/AnimationInterface.hpp"

class GrowingAnimation : public AnimationInterface
{
public:
    GrowingAnimation();
    void setup() override;
    void loop(unsigned long currentMillis) override;

private:
    void nearestNeighborScale(const uint8_t *srcBitmap, int srcWidth, int srcHeight, uint8_t *destBitmap, int destWidth, int destHeight);
    unsigned long previousMillis;
    const long interval = 75;
    int xPos;
};
