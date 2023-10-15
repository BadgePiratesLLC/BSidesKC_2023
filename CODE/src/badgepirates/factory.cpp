#include "badgepirates/factory.hpp"

void Factory::setup()
{
#if VERSION > 0
    Serial.println("[FACTORY] WARNING: Factory code is enabled on production versions");
#endif

#ifdef AUTOMATIC_UPDATES
    delay(AUTOMATIC_UPDATE_DELAY);
    // OTA::checkOTASync();
#else
    pinMode(GPIO_NUM_0, INPUT_PULLUP);
    Serial.println("Hold BOOT to initialize an update.");
#endif
}

void Factory::loop()
{

#ifndef AUTOMATIC_UPDATES
    auto boot_buttonState = digitalRead(GPIO_NUM_0);
    auto down_buttonState = digitalRead(down_btn_pin);
    auto up_buttonState = digitalRead(up_btn_pin);
    auto center_buttonState = digitalRead(center_btn_pin);
    auto right_buttonState = digitalRead(right_btn_pin);
    auto left_buttonState = digitalRead(left_btn_pin);

    if (boot_buttonState == LOW)
    { // Pin state ref: https://www.esp32.com/viewtopic.php?t=869, Change if board is different.
        leds_off();
        // OTA::checkOTASync();
    }

    if (down_buttonState == LOW)
    {
        leds[0] = CRGB::Blue;
        FastLED.show();
    }
    else if (up_buttonState == LOW)
    {
        leds[0] = CRGB::Green;
        FastLED.show();
    }
    else if (center_buttonState == LOW)
    {
        leds[0] = CRGB::White;
        FastLED.show();
    }
    else if (right_buttonState == LOW)
    {
        leds[0] = CRGB::Purple;
        FastLED.show();
    }
    else if (left_buttonState == LOW)
    {
        leds[0] = CRGB::Red;
        FastLED.show();
    }
    else
    {
        leds[0] = CRGB::Black;
        FastLED.show();
    }

    delay(20);
#else
#endif
}