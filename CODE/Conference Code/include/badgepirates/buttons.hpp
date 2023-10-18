#pragma once
#include <Arduino.h>

class Buttons
{
public:
    Buttons();
    void setup();
    bool isPressedUp();
    bool isPressedDown();
    bool isPressedLeft();
    bool isPressedRight();
    bool isPressedCenter();
    bool isAnyButtonPressed();
    unsigned long lastButtonPressTime = 0;

#if CONFIG_IDF_TARGET_ESP32S3
    const int BUTTON_RIGHT = 35;
    const int BUTTON_CENTER = 36;
    const int BUTTON_UP = 37;
    const int BUTTON_DOWN = 38;
    const int BUTTON_LEFT = 46;
#else
    // Sean's dev board
    const int BUTTON_RIGHT = GPIO_NUM_39;
    const int BUTTON_LEFT = GPIO_NUM_33;
    const int BUTTON_CENTER = GPIO_NUM_36;
    const int BUTTON_UP = GPIO_NUM_34;
    const int BUTTON_DOWN = GPIO_NUM_35;

#endif
private:
    unsigned long lastUpPress;
    unsigned long lastDownPress;
    unsigned long lastLeftPress;
    unsigned long lastRightPress;
    unsigned long lastCenterPress;
};
