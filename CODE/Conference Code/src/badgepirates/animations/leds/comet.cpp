#include "badgepirates/animations/leds/comet.hpp"
#include <Arduino.h>

CometAnimation::CometAnimation(const int *all_leds, int num_leds)
    : leds(all_leds), numLeds(num_leds), previousMillis(0), interval(110), cometPosition(0), direction(1), cometSize(3)
{
    // Constructor initialization done
}

void CometAnimation::setup()
{
    for (int i = 0; i < numLeds; i++)
    {
        pinMode(leds[i], OUTPUT);
        digitalWrite(leds[i], LOW);
    }
}

void CometAnimation::loop(unsigned long currentMillis)
{
    if (currentMillis - previousMillis >= interval)
    {
        // save the last time the animation updated
        previousMillis = currentMillis;

        // Move the comet to the next position
        cometPosition += direction;

        // Reverse direction of comet at the ends
        if (cometPosition == (numLeds - 1) || cometPosition == 0)
        {
            direction = -direction;
        }

        // Clear all LEDs
        for (int i = 0; i < numLeds; i++)
        {
            digitalWrite(leds[i], 0);
        }

        // Draw the comet at the current position with a fading tail
        for (int i = 0; i < cometSize; i++)
        {
            if ((cometPosition - i >= 0) && (cometPosition - i < numLeds))
            {
                digitalWrite(leds[cometPosition - i], HIGH);
            }
        }
    }
}
