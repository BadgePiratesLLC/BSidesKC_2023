#pragma once

#include <Adafruit_SSD1306.h>

class Oled
{
private:
    unsigned long oledInitializationTime;

public:
    Oled(); // Constructor
    Adafruit_SSD1306 *display;
    void setup();
    void loop();
    bool oledInitialized = false;
};
