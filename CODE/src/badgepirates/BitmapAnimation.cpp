#include "badgepirates/oled.hpp"
#include "badgepirates/WifiManager.hpp"
#include "badgepirates/BitmapAnimation.hpp"
#include "badgepirates/images/wifi-logo.h"
#include <Arduino.h>

extern Oled oled;
extern WifiManager wifiManager;

BitmapAnimation::BitmapAnimation(const unsigned char *const *frames, int frameCount)
    : frames(frames), frameCount(frameCount), frameWidth(128), frameHeight(32), frameDuration(60), currentFrameIndex(0), lastFrameChangeMillis(0) {}

void BitmapAnimation::setup()
{
    // Any setup code here, for example:
    lastFrameChangeMillis = millis();
}

void BitmapAnimation::loop(unsigned long currentMillis)
{
    if (currentMillis - lastFrameChangeMillis >= frameDuration)
    {
        nextFrame();
        lastFrameChangeMillis = currentMillis;
        oled.display->clearDisplay();
        if (wifiManager.isConnected())
        {
            oled.display->drawBitmap(120, 0, epd_bitmap_wifi_logo, 8, 8, SSD1306_WHITE);
        }

        oled.display->drawBitmap(0, 0, frames[currentFrameIndex], frameWidth, frameHeight, SSD1306_WHITE);
        oled.display->display();
    }
}

void BitmapAnimation::nextFrame()
{
    currentFrameIndex = (currentFrameIndex + 1) % frameCount;
}