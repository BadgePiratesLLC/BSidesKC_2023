#include "badgepirates/WifiManager.hpp"
#include "badgepirates/animations/oled/matrixAnimation.hpp"
#include "badgepirates/images/bp-logo.h"
#include "badgepirates/images/wifi-logo.h"

extern WifiManager wifiManager;
extern Oled oled; // Declare access to the global instance

MatrixAnimation::MatrixAnimation()
{
    // Constructor code if any
}

void MatrixAnimation::setup()
{
    initializeBitmaps();
}

void MatrixAnimation::initializeBitmaps()
{
    int divisionWidth = OLED_WIDTH / NUM_BITMAPS;

    for (int i = 0; i < NUM_BITMAPS; i++)
    {
        bitmaps[i].x = random(i * divisionWidth, (i + 1) * divisionWidth - 16);
        bitmaps[i].y = -22; // Start off the screen at the top, adjusted for 10 pixel UI space
        bitmaps[i].speed = random(2, 5);
    }
}

void MatrixAnimation::drawBitmapWithTrail(FallingBitmap &bitmap)
{
    for (int i = 0; i < 3; i++)
    {
        int newY = bitmap.y + 10 - (i * 16); // Adjust "16" with the actual bitmap's height and add 10 for UI space
        if (newY < OLED_HEIGHT + 16 * 6)
        {
            // Adjust bitmap brightness based on its position in the trail
            if (i == 0)
            {
                oled.display->drawBitmap(bitmap.x, newY, epd_bitmap_bp_small_pixel, 16, 16, SSD1306_WHITE); // Leading bitmap
            }
            else if (i == 1)
            {                                                                               // Middle bitmap
                drawDitheredBitmap(bitmap.x, newY, epd_bitmap_bp_small_pixel, 16, 16, 0.5); // 50% brightness
            }
            else
            {                                                                                // Last bitmap
                drawDitheredBitmap(bitmap.x, newY, epd_bitmap_bp_small_pixel, 16, 16, 0.26); // 25% brightness
            }
        }
    }
}

void MatrixAnimation::drawDitheredBitmap(int x, int y, const uint8_t *bitmap, int width, int height, float brightness)
{
    int bayerMatrix[2][2] = {
        {1, 2},
        {3, 0}};
    int threshold = brightness * 4;

    for (int j = 0; j < height; j++)
    {
        for (int i = 0; i < width; i++)
        {
            uint8_t byteVal = pgm_read_byte(bitmap + (j * (width / 8) + i / 8));
            if ((byteVal >> (7 - (i % 8))) & 0x01)
            { // Check if the specific bit for the pixel is set
                if (bayerMatrix[i % 2][j % 2] < threshold)
                {
                    oled.display->drawPixel(x + i, y + j, SSD1306_WHITE);
                }
            }
        }
    }
}

void MatrixAnimation::loop(unsigned long currentMillis)
{
    oled.display->clearDisplay();

    if (wifiManager.isConnected())
    {
        oled.display->drawBitmap(120, 0, epd_bitmap_wifi_logo, 8, 8, SSD1306_WHITE);
    }

    int divisionWidth = OLED_WIDTH / NUM_BITMAPS;

    for (int i = 0; i < NUM_BITMAPS; i++)
    {
        bitmaps[i].y += bitmaps[i].speed;

        if (bitmaps[i].y > OLED_HEIGHT + 38) // Adjusted for 10 pixel UI space
        {
            bitmaps[i].x = random(i * divisionWidth, (i + 1) * divisionWidth - 16);
            bitmaps[i].y = -22; // Reset to top, adjusted for 10 pixel UI space
            bitmaps[i].speed = random(2, 5);
        }

        drawBitmapWithTrail(bitmaps[i]);
    }

    oled.display->display();
}