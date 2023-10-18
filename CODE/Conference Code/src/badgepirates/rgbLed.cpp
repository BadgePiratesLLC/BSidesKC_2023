#include "badgepirates/rgbLed.hpp"
#include "config.hpp"

struct CRGB leds[NUM_LEDS];
// Define the colors (assumes CRGB supports these color names)
CRGB colors[] = {CRGB::Red, CRGB::Orange, CRGB::Yellow, CRGB::Green, CRGB::Blue, CRGB::Indigo, CRGB::Violet};
int currentColorIndex = 0;

void RGBLed::test_ws2812()
{
    // Insert anything that needs to be tested here.
    FastLED.setBrightness(100);
    for (int dot = 0; dot < NUM_LEDS; dot++)
    {
        leds[dot] = CRGB::White;
        FastLED.show();
        delay(30);
    }
    delay(100);
    for (int dot = 0; dot < NUM_LEDS; dot++)
    {
        leds[dot] = CRGB::Black;
        FastLED.show();
        delay(30);
    }
}

void RGBLed::setup()
{
    FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS);
    test_ws2812();
}

void RGBLed::loop(unsigned long currentMillis)
{
    if (currentAnimation == Animation::BLUE_FLASH)
    {
        if (isFlashing)
        {
            if (currentMillis - lastUpdate >= 250)
            {
                lastUpdate = currentMillis;
                isBlue = !isBlue;
                for (int dot = 0; dot < NUM_LEDS; dot++)
                {
                    leds[dot] = isBlue ? CRGB::Blue : CRGB::Black;
                }
                FastLED.show();
                if (currentMillis - flashStartTime >= flashDuration)
                {
                    isFlashing = false;
                }
            }
        }
    }
    else if (currentAnimation == Animation::RAINBOW_FLASH)
    {
        if (isFlashing)
        {
            if (currentMillis - lastUpdate >= 250) // Adjust this value to change the speed of color change
            {
                lastUpdate = currentMillis;

                // Cycle through colors array
                currentColorIndex = (currentColorIndex + 1) % (sizeof(colors) / sizeof(colors[0]));

                for (int dot = 0; dot < NUM_LEDS; dot++)
                {
                    leds[dot] = colors[currentColorIndex];
                }
                FastLED.show();

                // Stop flashing after a certain duration
                if (currentMillis - flashStartTime >= flashDuration)
                {
                    isFlashing = false;
                    for (int dot = 0; dot < NUM_LEDS; dot++)
                    {
                        leds[dot] = CRGB::Black;
                    }
                    FastLED.show();
                }
            }
        }
    }
}

void RGBLed::startBlueFlash()
{
    isFlashing = true;
    flashStartTime = millis();
    lastUpdate = millis();
    currentAnimation = Animation::BLUE_FLASH;
    lastUpdate = millis();
}

void RGBLed::startRainbowFlash()
{
    isFlashing = true;
    flashStartTime = millis();
    lastUpdate = millis();
    currentAnimation = Animation::RAINBOW_FLASH;
    lastUpdate = millis();
}