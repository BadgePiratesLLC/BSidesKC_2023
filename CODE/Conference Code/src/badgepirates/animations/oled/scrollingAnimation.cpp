#include "badgepirates/animations/oled/scrollingAnimation.hpp"
#include "badgepirates/images/bp-logo.h"
extern Oled oled; // Declare access to the global instance

ScrollingAnimation::ScrollingAnimation() {}

void ScrollingAnimation::setup()
{
    previousMillis = 0;
}

void ScrollingAnimation::loop(unsigned long currentMillis)
{
    if (currentMillis - previousMillis > interval)
    {
        previousMillis = currentMillis;
        oled.display->clearDisplay();
        oled.display->drawBitmap(xPos, 0, epd_bitmap_bp_small_pixel, 16, 16, SSD1306_WHITE);
        oled.display->display();

        xPos += 5;
        if (xPos > oled.display->width())
        {
            xPos = -16; // Reset the position to make the bitmap fly again
        }
    }
}
