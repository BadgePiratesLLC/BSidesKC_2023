// badgepirates/animations/oled/bouncingAnimation.cpp

#include "badgepirates/animations/oled/bouncingAnimation.hpp"
#include "badgepirates/images/bp-logo.h"
#include "badgepirates/images/wifi-logo.h"
#include "badgepirates/WifiManager.hpp"

extern Oled oled;
extern WifiManager wifiManager;

BouncingAnimation::BouncingAnimation()
    : xPos(0), yPos(10), xSpeed(2), ySpeed(2) {}

void BouncingAnimation::setup()
{
    previousMillis = 0;
}

void BouncingAnimation::loop(unsigned long currentMillis)
{
    if (currentMillis - previousMillis > interval)
    {
        previousMillis = currentMillis;

        oled.display->clearDisplay();
        oled.display->drawBitmap(xPos, yPos, epd_bitmap_bp_small_pixel, 16, 16, SSD1306_WHITE);

        if (wifiManager.isConnected())
        {
            oled.display->drawBitmap(120, 0, epd_bitmap_wifi_logo, 8, 8, SSD1306_WHITE);
        }

        oled.display->display();

        // Update positions
        xPos += xSpeed;
        yPos += ySpeed;

        // Check for collisions with edges and bounce if necessary
        if (xPos <= 0 || xPos + 16 >= oled.display->width())
        {
            xSpeed = -xSpeed; // Reverse the direction on the X axis
        }
        if (yPos <= 10 || yPos + 16 >= oled.display->height())
        {
            ySpeed = -ySpeed; // Reverse the direction on the Y axis
        }
    }
}
