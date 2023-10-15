#ifndef QRDISPLAY_HPP
#define QRDISPLAY_HPP

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <qrcode.h>

class QRDisplay
{
public:
    QRDisplay();
    ~QRDisplay(); // Add destructor declaration
    void setup();
    void generateQRCode(const String &data);
    void drawQROled(QRCode &qrcode);
    void drawQRSerial(QRCode &qrcode);

    QRCode qrcode; // Made this public for direct access

private:
    void drawQR(QRCode &qrcode);
    const float MARGIN_FACTOR = .9; // 90% of the calculated size

    uint8_t *qrcodeData; // Dynamically allocated array for QR code data
    int bufferSize;
};

#endif // QRDISPLAY_HPP
