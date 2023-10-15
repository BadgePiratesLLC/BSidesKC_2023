#ifndef BOUNCING_ANIMATION_HPP
#define BOUNCING_ANIMATION_HPP
#include "badgepirates/oled.hpp"
#include <Adafruit_GFX.h>
#include "badgepirates/AnimationInterface.hpp"

#include "badgepirates/oled.hpp" // Include the header for the Oled class. This might change based on your actual project structure.

class BouncingAnimation : public AnimationInterface
{
public:
    BouncingAnimation();
    void setup();
    void loop(unsigned long currentMillis);

private:
    int xPos, yPos;     // Position of the bitmap
    int xSpeed, ySpeed; // Speed in X and Y directions
    unsigned long previousMillis;
    const unsigned long interval = 50; // Change this to adjust the speed of the animation
};

#endif // BOUNCING_ANIMATION_HPP
