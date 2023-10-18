#include "badgepirates/operatorLed.hpp"
#include "badgepirates/webSocket.hpp"

extern WebSocketManager webSocket;

void OperatorLED::set_operator_color(int red, int green, int blue)
{
    analogWrite(red_pin, red);
    analogWrite(green_pin, green);
    analogWrite(blue_pin, blue);
}

void OperatorLED::operator_led_setup()
{
    pinMode(red_pin, OUTPUT);
    pinMode(green_pin, OUTPUT);
    pinMode(blue_pin, OUTPUT);
}

void OperatorLED::setup()
{
}
// Convert HSV to RGB
// h is from 0-360
// s,v values are 0-1
// r,g,b values are from 0-255
void OperatorLED::hsvToRgb(float h, float s, float v, int &r, int &g, int &b)
{
    int i;
    float f, p, q, t;

    if (s == 0)
    {
        // Achromatic (gray)
        r = g = b = v * 255.0;
        return;
    }

    h /= 60.0; // Sector 0 to 5
    i = floor(h);
    f = h - i; // Factorial part of h
    p = v * (1.0 - s);
    q = v * (1.0 - s * f);
    t = v * (1.0 - s * (1.0 - f));

    switch (i)
    {
    case 0:
        r = v * 255.0;
        g = t * 255.0;
        b = p * 255.0;
        break;
    case 1:
        r = q * 255.0;
        g = v * 255.0;
        b = p * 255.0;
        break;
    case 2:
        r = p * 255.0;
        g = v * 255.0;
        b = t * 255.0;
        break;
    case 3:
        r = p * 255.0;
        g = q * 255.0;
        b = v * 255.0;
        break;
    case 4:
        r = t * 255.0;
        g = p * 255.0;
        b = v * 255.0;
        break;
    default: // case 5:
        r = v * 255.0;
        g = p * 255.0;
        b = q * 255.0;
        break;
    }
}

void OperatorLED::rotate_colors()
{

    unsigned long currentMillis = millis();

    if (currentMillis - previousMillis >= interval)
    {
        previousMillis = currentMillis;

        // Convert the Hue to RGB
        int red, green, blue;
        hsvToRgb(hue, 1.0, 1.0, red, green, blue); // saturation and value are set to 1 for full color

        analogWrite(red_pin, red);
        analogWrite(green_pin, green);
        analogWrite(blue_pin, blue);

        hue += 0.5; // Adjust this value to change the speed of the hue change.
        if (hue >= 360.0)
            hue = 0; // Loop back to the beginning
    }
}

void OperatorLED::loop()
{
    if (webSocket.isOnline())
    {
        // If the badge is connected to websockets, show a green light
        set_operator_color(0, 255, 0);
    }
    else
    {
        rotate_colors();
    }
}