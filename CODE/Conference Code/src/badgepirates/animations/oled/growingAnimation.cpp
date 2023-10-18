#include "badgepirates/WifiManager.hpp"
#include "badgepirates/animations/oled/growingAnimation.hpp"
#include "badgepirates/images/bp-logo.h"
#include "badgepirates/images/wifi-logo.h"
extern Oled oled; // Declare access to the global instance

const long interval = 75; // Interval to move the bitmap
int xPos = 0;
int yPos = 0;
float scale = .5;          // Initial scale
float scaleIncrement = .5; // How much the bitmap grows each frame

extern WifiManager wifiManager;
GrowingAnimation::GrowingAnimation() {}

void GrowingAnimation::setup()
{
    previousMillis = 0;
}

void GrowingAnimation::nearestNeighborScale(const uint8_t *srcBitmap, int srcWidth, int srcHeight, uint8_t *destBitmap, int destWidth, int destHeight)
{
    for (int y = 0; y < destHeight; y++)
    {
        for (int x = 0; x < destWidth; x++)
        {
            int srcX = x * srcWidth / destWidth;
            int srcY = y * srcHeight / destHeight;

            // Extract the source pixel
            uint8_t srcPixel = (srcBitmap[(srcY * srcWidth + srcX) / 8] >> (7 - (srcX % 8))) & 0x01;

            // Set the destination pixel
            if (srcPixel)
            {
                destBitmap[(y * destWidth + x) / 8] |= (0x01 << (7 - (x % 8)));
            }
            else
            {
                destBitmap[(y * destWidth + x) / 8] &= ~(0x01 << (7 - (x % 8)));
            }
        }
    }
}

void GrowingAnimation::loop(unsigned long currentMillis)
{
    if (currentMillis - previousMillis > interval)
    {
        previousMillis = currentMillis;

        int newWidth = 16 * scale;
        int newHeight = 16 * scale;
        uint8_t scaledBitmap[newWidth * newHeight];

        int xPos = (oled.display->width() - newWidth) / 2;
        int yPos = (oled.display->height() - newHeight) / 2;

        oled.display->clearDisplay();

        if (wifiManager.isConnected())
        {
            oled.display->drawBitmap(120, 0, epd_bitmap_wifi_logo, 8, 8, SSD1306_WHITE);
        }

        memset(scaledBitmap, 0, sizeof(scaledBitmap)); // Clear the buffer before scaling
        // scale the bitmap
        nearestNeighborScale(epd_bitmap_bp_small_pixel, 16, 16, scaledBitmap, newWidth, newHeight);
        // draw the bitmap
        oled.display->drawBitmap(xPos, yPos, scaledBitmap, newWidth, newHeight, SSD1306_WHITE);
        // write the buffer to the display
        oled.display->display();

        scale += scaleIncrement;

        if (newWidth > oled.display->width() || newHeight > oled.display->height())
        {
            scale = 1.0; // Reset scale when bitmap overflows the display
        }
    }
}
