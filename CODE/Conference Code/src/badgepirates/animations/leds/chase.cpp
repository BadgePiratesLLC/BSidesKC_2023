#include "badgepirates/animations/leds/chase.hpp"
#include <Arduino.h>

ChaseAnimation::ChaseAnimation(const int *all_leds, int num_leds)
    : leds(all_leds), numLeds(num_leds), previousMillis(0), interval(100), currentLed(0) // Change interval to your desired LED change speed.
{
    // Constructor initialization done
}

void ChaseAnimation::setup()
{
    for (int i = 0; i < numLeds; i++)
    {
        pinMode(leds[i], OUTPUT);
        digitalWrite(leds[i], LOW);
    }
}

void ChaseAnimation::loop(unsigned long currentMillis)
{
    if (currentMillis - previousMillis >= interval)
    {
        // Turn off the current LED
        digitalWrite(leds[currentLed], LOW);

        // Move to the next LED
        currentLed++;
        if (currentLed >= numLeds)
        {
            currentLed = 0;
        }

        // Turn on the new current LED
        digitalWrite(leds[currentLed], HIGH);

        // Save the last time you updated the LED
        previousMillis = currentMillis;
    }
}
