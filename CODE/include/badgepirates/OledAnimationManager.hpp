#pragma once
#include "badgepirates/oled.hpp"
#include "badgepirates/eeprom.hpp"
#include "badgepirates/AnimationInterface.hpp"
#include "badgepirates/buttons.hpp"
#include "badgepirates/Menu.hpp"
#include "config.h"

class OledAnimationManager
{
public:
    OledAnimationManager();
    void setup();
    void loop(unsigned long currentMillis);
    bool isDisplayingMessage = false;
    unsigned long messageDisplayTime = 0;
    unsigned long lastButtonPressTime = 0;          // Time when the button was last pressed
    const unsigned long inactivityThreshold = 5000; // 5 seconds
    bool isPlayingAnimation = true;                 // Flag to check if OLED animation is playing
    unsigned long lastSwitchTime;
    String selectedOledAnimation;
    String lastSelectedOledAnimation;
    void displayText(const char *text);

private:
    void setAnimation(AnimationInterface *animation);

    int TEXT_DISPLAY_DURATION = 25000; // 25 seconds
    AnimationInterface *animations[6];
    AnimationInterface *currentAnimation;
    int currentAnimationIndex;
    unsigned long lastAnimationChangeTime = 0;
    const unsigned long animationChangeCooldown = 500; // 500 ms for example

    Buttons buttons;
};