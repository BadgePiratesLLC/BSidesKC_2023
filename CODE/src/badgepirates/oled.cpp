#include "badgepirates/oled.hpp"
#include "badgepirates/images/bsides-logo.h"

Oled::Oled()
{
    // Create an instance of the OLED display
    display = new Adafruit_SSD1306(128, 32, &Wire, -1);
}

void Oled::setup()
{
#if CONFIG_IDF_TARGET_ESP32S3
    Wire.begin(GPIO_NUM_6, GPIO_NUM_7);
#else
    // Sean's dev board
    Wire.begin(GPIO_NUM_4, GPIO_NUM_2);
#endif

    oledInitializationTime = millis();
    oledInitialized = false;
    if (!display->begin(SSD1306_SWITCHCAPVCC, 0x3C))
    {
        Serial.println(F("SSD1306 allocation failed"));
        return;
    }

    // Flip the display so it's easier to read
    display->ssd1306_command(0xA0);
    display->ssd1306_command(0xC0);
    display->setFont();
    display->setTextColor(SSD1306_WHITE);
    display->setTextSize(1);
    display->clearDisplay();
    display->drawBitmap(36, 0, epd_bitmap_bsides_logo_pixel, 54, 32, SSD1306_WHITE);
    display->display();

    oledInitialized = true;
}

void Oled::loop()
{
}