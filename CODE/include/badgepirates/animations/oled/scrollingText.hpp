#pragma once
#include "badgepirates/oled.hpp"
#include <Adafruit_GFX.h>
#include "badgepirates/AnimationInterface.hpp"

class ScrollingTextAnimation : public AnimationInterface
{
public:
    ScrollingTextAnimation();
    void setup();
    void loop(unsigned long currentMillis);
    void addLine(const char *text, const uint8_t *image = nullptr);

private:
    struct Line
    {
        const char *text;
        const uint8_t *image; // Assuming you're using bitmap images, like in your bouncing animation.
    };

    Line lines[18]; // Assuming max 10 lines. Adjust as necessary.
    int numLines;
    int currentY;
    unsigned long previousMillis;
    const unsigned long interval = 50; // Adjust scrolling speed
};
