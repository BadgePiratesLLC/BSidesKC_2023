// BreathAnimation.cpp
#include "badgepirates/animations/leds/breath.hpp"
#include <Arduino.h>

BreathAnimation::BreathAnimation(const int *all_leds, int num_leds)
    : leds(all_leds), numLeds(num_leds), previousMillis(0), interval(10), brightness(0), increasing(true)
{
    // Constructor initialization done
}

void BreathAnimation::setup()
{
    for (int i = 0; i < numLeds; i++)
    {
        pinMode(leds[i], OUTPUT);
        digitalWrite(leds[i], LOW);
    }
}

void BreathAnimation::loop(unsigned long currentMillis)
{
    if (currentMillis - previousMillis >= interval)
    {
        // save the last time you blinked the LED
        previousMillis = currentMillis;

        // change the brightness for next time through the loop:
        brightness = increasing ? brightness + 1 : brightness - 1;

        // reverse the direction of the fading at the ends of the fade:
        if (brightness <= 0 || brightness >= 255)
        {
            increasing = !increasing;
        }

        // Set the brightness on all LEDs
        for (int i = 0; i < numLeds; i++)
        {
            analogWrite(leds[i], brightness);
        }
    }
}
