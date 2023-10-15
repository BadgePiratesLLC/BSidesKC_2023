#include <Arduino.h>
#include "badgepirates/eeprom.hpp"
#include "badgepirates/rgbLed.hpp"
#include "badgepirates/OledAnimationManager.hpp"
#include "badgepirates/BitmapAnimation.hpp"
#include "badgepirates/animations/oled/scrollingAnimation.hpp"
#include "badgepirates/animations/oled/growingAnimation.hpp"
#include "badgepirates/animations/oled/matrixAnimation.hpp"
#include "badgepirates/animations/oled/bouncingAnimation.hpp"
#include "badgepirates/animations/gifs/trollDance.hpp"
#include "badgepirates/animations/gifs/animatedDots.hpp"
#include "badgepirates/animations/oled/scrollingText.hpp"

Menu menu;
extern BMSEEPROM bmsData;
extern RGBLed rgbLed;
extern Oled oled;
extern Buttons buttons;
extern BMSEEPROM bmsData;

OledAnimationManager::OledAnimationManager()
    : animations{new BouncingAnimation(), new GrowingAnimation(), new MatrixAnimation(), new BitmapAnimation(trollDanceFrames, 10), new BitmapAnimation(animatedDotsFrames, 120), new ScrollingTextAnimation()},
      currentAnimationIndex(0)
{
    currentAnimation = animations[currentAnimationIndex];
    lastSwitchTime = millis();
}

void OledAnimationManager::setAnimation(AnimationInterface *animation)
{
    currentAnimation = animation;
    if (currentAnimation)
    {
        currentAnimation->setup();
    }
    lastAnimationChangeTime = millis();
}

void OledAnimationManager::displayText(const char *text)
{
    if (!oled.oledInitialized)
    {
        return;
    }

    // isPlayingAnimation = false;
    isDisplayingMessage = true;
    rgbLed.startRainbowFlash(); // start the LED animation
    oled.display->clearDisplay();
    oled.display->setTextSize(1);
    oled.display->setTextColor(SSD1306_WHITE);
    oled.display->setCursor(0, 0);
    oled.display->print(text);
    oled.display->display();
    messageDisplayTime = millis();
}

void OledAnimationManager::setup()
{
    selectedOledAnimation = bmsData.get("oledAnimation");
    lastSelectedOledAnimation = selectedOledAnimation;
    buttons.setup();
    if (currentAnimation)
    {
        currentAnimation->setup();
    }
}

void OledAnimationManager::loop(unsigned long currentMillis)
{
    rgbLed.loop(currentMillis);

    if (!oled.oledInitialized)
    {
        return;
    }
    // Call this functions so we can update the last time a button was pressed
    buttons.isAnyButtonPressed();

    if (currentMillis - messageDisplayTime >= TEXT_DISPLAY_DURATION)
    {
        isDisplayingMessage = false;
    }

    if (isDisplayingMessage)
    {
        return;
    }
    else
    {

        // Set the animation based on the value from EEPROM
        if (selectedOledAnimation == "1" && currentAnimation != animations[0])
        {
            lastSelectedOledAnimation = selectedOledAnimation;
            setAnimation(animations[0]); // Assuming animations[0] is "Bounce"
        }
        else if (selectedOledAnimation == "2" && currentAnimation != animations[1])
        {
            lastSelectedOledAnimation = selectedOledAnimation;
            setAnimation(animations[1]); // Assuming animations[1] is "Grow"
        }
        else if (selectedOledAnimation == "3" && currentAnimation != animations[2])
        {
            lastSelectedOledAnimation = selectedOledAnimation;
            setAnimation(animations[2]); // Assuming animations[2] is "Matrix"
        }
        else if (selectedOledAnimation == "4" && currentAnimation != animations[3])
        {
            lastSelectedOledAnimation = selectedOledAnimation;
            setAnimation(animations[3]); // Assuming animations[2] is "Troll"
        }
        else if (selectedOledAnimation == "5" && currentAnimation != animations[4])
        {
            lastSelectedOledAnimation = selectedOledAnimation;
            setAnimation(animations[4]); // Assuming animations[2] is "Matrix"
        }
        else if (selectedOledAnimation == "6" && currentAnimation != animations[5])
        {
            setAnimation(animations[5]); // Assuming animations[2] is "Matrix"
        }

        // Check for inactivity
        if (!isPlayingAnimation && (millis() - buttons.lastButtonPressTime > inactivityThreshold))
        {
            menu.isMenuAwake = false;
            isPlayingAnimation = true; // Set the flag to true
            currentAnimation->loop(currentMillis);
        }

        // If playing animation and a button is pressed, stop the animation and show the menu
        if (isPlayingAnimation && (millis() - lastAnimationChangeTime > animationChangeCooldown) && (buttons.isPressedLeft() || buttons.isPressedRight() || buttons.isPressedUp() || buttons.isPressedDown() || buttons.isPressedCenter()))
        {
            menu.setMainMenu();
            isPlayingAnimation = false; // Set the flag to false
        }

        // If playing animation, call the loop method of the current animation
        if (isPlayingAnimation)
        {
            currentAnimation->loop(currentMillis);
        }

        if (!isPlayingAnimation)
        {
            menu.loop();
        }
    }
}
