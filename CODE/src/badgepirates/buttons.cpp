#include "badgepirates/buttons.hpp"

#define DEBOUNCE_DELAY 250 // Debounce time in milliseconds

Buttons::Buttons() : lastUpPress(0),
                     lastDownPress(0),
                     lastLeftPress(0),
                     lastRightPress(0),
                     lastCenterPress(0),
                     lastButtonPressTime(0)
{
}

void Buttons::setup()
{
    pinMode(BUTTON_RIGHT, INPUT_PULLUP);
    pinMode(BUTTON_CENTER, INPUT_PULLUP);
    pinMode(BUTTON_UP, INPUT_PULLUP);
    pinMode(BUTTON_DOWN, INPUT_PULLUP);
    pinMode(BUTTON_LEFT, INPUT_PULLUP);
}

bool Buttons::isPressedUp()
{
    unsigned long now = millis();
    if (digitalRead(BUTTON_UP) == LOW && (now - lastUpPress > DEBOUNCE_DELAY))
    {
        lastUpPress = now;
        return true;
    }
    return false;
}

bool Buttons::isPressedDown()
{
    unsigned long now = millis();
    if (digitalRead(BUTTON_DOWN) == LOW && (now - lastDownPress > DEBOUNCE_DELAY))
    {
        lastDownPress = now;
        return true;
    }
    return false;
}

bool Buttons::isPressedLeft()
{
    unsigned long now = millis();
    if (digitalRead(BUTTON_LEFT) == LOW && (now - lastLeftPress > DEBOUNCE_DELAY))
    {
        lastLeftPress = now;
        return true;
    }
    return false;
}

bool Buttons::isPressedRight()
{
    unsigned long now = millis();
    if (digitalRead(BUTTON_RIGHT) == LOW && (now - lastRightPress > DEBOUNCE_DELAY))
    {
        lastRightPress = now;
        return true;
    }
    return false;
}

bool Buttons::isPressedCenter()
{
    unsigned long now = millis();
    if (digitalRead(BUTTON_CENTER) == LOW && (now - lastCenterPress > DEBOUNCE_DELAY))
    {
        lastCenterPress = now;
        return true;
    }
    return false;
}

bool Buttons::isAnyButtonPressed()
{
    unsigned long now = millis();
    if ((digitalRead(BUTTON_UP) == LOW || digitalRead(BUTTON_DOWN) == LOW || digitalRead(BUTTON_LEFT) == LOW || digitalRead(BUTTON_RIGHT) == LOW || digitalRead(BUTTON_CENTER) == LOW) && (now - lastButtonPressTime > DEBOUNCE_DELAY))
    {
        lastButtonPressTime = now;
        return true;
    }
    return false; // Add this return for the case when the condition is false.
}
