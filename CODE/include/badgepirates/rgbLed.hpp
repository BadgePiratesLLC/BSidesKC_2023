#pragma once

#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include "FastLedInclude.hpp"

class RGBLed
{
private:
    unsigned long flashStartTime;
    unsigned long lastUpdate;
    const int flashDuration = 10000; // flash for 10 seconds
    bool isFlashing = false;
    bool isBlue = false;
    enum Animation
    {
        BLUE_FLASH,
        RAINBOW_FLASH
    };
    Animation currentAnimation = Animation::BLUE_FLASH;

public:
    void setup();
    void test_ws2812();
    void startBlueFlash();
    void startRainbowFlash();
    void loop(unsigned long currentMillis);
};