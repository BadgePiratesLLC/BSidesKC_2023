#include "badgepirates/AnimationManager.hpp"
#include "badgepirates/eeprom.hpp"
#include "badgepirates/animations/leds/breath.hpp"
#include "badgepirates/animations/leds/chase.hpp"
#include "badgepirates/animations/leds/comet.hpp"
#include "badgepirates/buttons.hpp"
#include "config.h"
#include <Arduino.h>

// GPIO_NUM_10 - Camera
// GPIO_NUM_11 - Flashlight
// GPIO_NUM_12 - Magnifying glass
// GPIO_NUM_13 - Radio
// GPIO_NUM_14 - Notepad
// GPIO_NUM_15 - lock

const int lockLED = GPIO_NUM_15;
const int flashlightLED = GPIO_NUM_11;
const int magnifyingGlassLED = GPIO_NUM_12;
const int cameraLED = GPIO_NUM_10;
const int notepadLED = GPIO_NUM_14;
const int radioLED = GPIO_NUM_13;

extern BMSEEPROM bmsData;
extern Buttons buttons;

const int NUM_LEDS = 6;
#if CONFIG_IDF_TARGET_ESP32S3
const int all_leds[6] = {
    GPIO_NUM_15,
    GPIO_NUM_11,
    GPIO_NUM_12,
    GPIO_NUM_13,
    GPIO_NUM_14,
    GPIO_NUM_10};
#else
const int all_leds[6] = {
    GPIO_NUM_16,
    GPIO_NUM_16,
    GPIO_NUM_16,
    GPIO_NUM_17,
    GPIO_NUM_17,
    GPIO_NUM_17};
#endif

bool isLockPickLEDOn = 0;
bool isFlashlightLEDOn = 0;
bool isMagnifyingGlassLEDOn = 0;
bool isCameraLEDOn = 0;
bool isNotepadLEDOn = 0;
bool isRadioLEDOn = 0;

AnimationManager::AnimationManager()
    : animations{/*new BreathAnimation(all_leds, NUM_LEDS),*/ new ChaseAnimation(all_leds, NUM_LEDS), new CometAnimation(all_leds, NUM_LEDS)},
      currentAnimationIndex(0)
{
    currentAnimation = animations[currentAnimationIndex];

    isLockPickLEDOn = bmsData.get("lock") ? HIGH : LOW;
    isFlashlightLEDOn = bmsData.get("flashlight") ? HIGH : LOW;
    isMagnifyingGlassLEDOn = bmsData.get("magnifyingGlass") ? HIGH : LOW;
    isCameraLEDOn = bmsData.get("camera") ? HIGH : LOW;
    isNotepadLEDOn = bmsData.get("notepad") ? HIGH : LOW;
    isRadioLEDOn = bmsData.get("radio") ? HIGH : LOW;
    for (int i = 0; i < 6; i++)
    {
        pinMode(all_leds[i], OUTPUT);
        digitalWrite(all_leds[i], LOW);
    }
}

void AnimationManager::setAnimation(AnimationInterface *animation)
{
    currentAnimation = animation;
    if (currentAnimation)
    {
        currentAnimation->setup();
    }
}

void AnimationManager::setup()
{
    if (currentAnimation)
        currentAnimation->setup();
}

void AnimationManager::loop(unsigned long currentMillis)
{
    // Run the current animation loop
    if (currentAnimation)
    {
        currentAnimation->loop(currentMillis);
    }

    digitalWrite(lockLED, bmsData.get("lock") == "1" ? HIGH : LOW);
    digitalWrite(flashlightLED, bmsData.get("flashlight") == "1" ? HIGH : LOW);
    digitalWrite(magnifyingGlassLED, bmsData.get("magnifying") == "1" ? HIGH : LOW);
    digitalWrite(cameraLED, bmsData.get("camera") == "1" ? HIGH : LOW);
    digitalWrite(notepadLED, bmsData.get("notepad") == "1" ? HIGH : LOW);
    digitalWrite(radioLED, bmsData.get("radio") == "1" ? HIGH : LOW);
}

bool AnimationManager::selectAnimationHack(int animation)
{
    Serial.printf("Selected animation %d\n", animation);
    if (animation == -1)
    {
        currentAnimationIndex = -1;
        currentAnimation = nullptr;
        return true;
    }

    if (animation >= 0 && animation < (sizeof(animations) / sizeof(void *)))
    {
        currentAnimationIndex = animation;
        setAnimation(animations[currentAnimationIndex]);
        return true;
    }
    return false;
}