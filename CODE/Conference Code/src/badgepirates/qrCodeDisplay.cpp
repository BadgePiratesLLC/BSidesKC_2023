#include "badgepirates/qrCodeDisplay.hpp"
#include "badgepirates/oled.hpp"
#include <Adafruit_GFX.h>

extern Oled oled;

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32

QRDisplay::QRDisplay()
{
    bufferSize = qrcode_getBufferSize(3);
    qrcodeData = new uint8_t[bufferSize];
}

QRDisplay::~QRDisplay()
{
    delete[] qrcodeData;
}

void QRDisplay::setup()
{
    // setup related code here
}

void QRDisplay::generateQRCode(const String &data)
{
    qrcode_initText(&qrcode, qrcodeData, 3, ECC_LOW, data.c_str());
}

void QRDisplay::drawQROled(QRCode &)
{
    oled.display->clearDisplay();

    int totalWidth = qrcode.size;  // Each module is 1x1 pixel, so total width = qrcode.size
    int totalHeight = qrcode.size; // Total height = qrcode.size

    // Calculate horizontal margin
    int xMargin = (SCREEN_WIDTH - totalWidth) / 2;

    // For a 32 pixel screen height with a 29x29 QR code, set yMargin to 1 or 2 for a tiny top margin.
    int yMargin = (SCREEN_HEIGHT - totalHeight) / 2; // Align center of the QR code with the center of the screen

    for (uint8_t y = 0; y < qrcode.size; y++)
    {
        for (uint8_t x = 0; x < qrcode.size; x++)
        {
            if (qrcode_getModule(&qrcode, x, y))
            {
                // Add the margins to the coordinates
                oled.display->drawPixel(x + xMargin, y + yMargin, SSD1306_WHITE);
            }
        }
    }
    oled.display->display();
}

void QRDisplay::drawQRSerial(QRCode &qrcode)
{
    // Top quiet zone
    Serial.print("\n\n\n\n");

    for (uint8_t y = 0; y < qrcode.size; y++)
    {

        // Left quiet zone
        Serial.print("        ");

        // Each horizontal module
        for (uint8_t x = 0; x < qrcode.size; x++)
        {

            // Print each module (UTF-8 \u2588 is a solid block)
            Serial.print(qrcode_getModule(&qrcode, x, y) ? "\u2588\u2588" : "  ");
        }

        Serial.print("\n");
    }

    // Bottom quiet zone
    Serial.print("\n\n\n\n");
}
