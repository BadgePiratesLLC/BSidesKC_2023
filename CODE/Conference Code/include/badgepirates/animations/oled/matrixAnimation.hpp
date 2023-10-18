#pragma once
#include "badgepirates/oled.hpp"
#include <Adafruit_GFX.h>
#include "badgepirates/AnimationInterface.hpp"

class MatrixAnimation : public AnimationInterface
{
public:
    MatrixAnimation();
    void setup() override;
    void loop(unsigned long currentMillis) override;

private:
    struct FallingBitmap
    {
        int x;     // X position
        int y;     // Y position
        int speed; // Falling speed
    };
    int OLED_WIDTH = 128;
    int OLED_HEIGHT = 32;

    void
    initializeBitmaps();
    void drawBitmapWithTrail(FallingBitmap &bitmap);
    void drawDitheredBitmap(int x, int y, const uint8_t *bitmap, int width, int height, float brightness);

    static const int TRAIL_LENGTH = 3; // Adjust this value as necessary
    static const int NUM_BITMAPS = 8;  // Adjust this value as necessary
    FallingBitmap bitmaps[NUM_BITMAPS];
};
