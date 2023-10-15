#pragma once
#define AUTOMATIC_UPDATES           // Comment out to disable, Does not require boot to be pressed to initialize an update.
#define AUTOMATIC_UPDATE_DELAY 5000 // Wait X second before beginning the update process

#include <Arduino.h>
#include <Adafruit_GFX.h>
#include "config.hpp"

// Buttons
const int red_pin = 4;   // Red LED, connected to digital pin 3
const int green_pin = 3; // Green LED, connected to digital pin 4
const int blue_pin = 5;  // Blue LED, connected to digital pin 5

class OperatorLED
{
    TaskHandle_t RedPinTaskHandle;

private:
    unsigned long previousMillis = 0;
    const long interval = 10; // How often we update the LED colors (10ms in this case)
    float hue = 0.0;

    void operator_led_setup();
    void set_operator_color(int r, int g, int b);
    void hsvToRgb(float h, float s, float v, int &r, int &g, int &b);
    void rotate_colors();

public:
    void setup();
    void loop();
};