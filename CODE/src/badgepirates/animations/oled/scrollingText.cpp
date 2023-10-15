#include "badgepirates/animations/oled/ScrollingText.hpp"
#include "badgepirates/WifiManager.hpp"

extern Oled oled;

ScrollingTextAnimation::ScrollingTextAnimation() : numLines(0), currentY(oled.display->height())
{
}

void ScrollingTextAnimation::setup()
{
    previousMillis = 0;

    // addLine("Producer: John Doe", epd_bitmap_bp_small_pixel);
    addLine("BSides KC 2023!");
    addLine("The Badge Pirates:");
    addLine("@fg");
    addLine("@networkgeek");
    addLine("@jigglebilly");
    addLine("@rixon");
    addLine("@reid");
    addLine("@Malmoset");
    addLine("@MrARM");
    addLine("@pecord");
    addLine("@Icona_p0p");
    addLine("@sysaaron");
    addLine("@CodexMafia");
    addLine("@LPender");
    addLine("@soup");
    addLine("Call 218-4BADGES");
    addLine("{bskc23}-EndCredz!");
}

void ScrollingTextAnimation::loop(unsigned long currentMillis)
{
    if (currentMillis - previousMillis > interval)
    {
        previousMillis = currentMillis;
        oled.display->clearDisplay();

        // Draw each line of text with its optional image
        for (int i = 0; i < numLines; i++)
        {
            if (lines[i].image)
            {
                oled.display->drawBitmap(0, currentY + i * 15, lines[i].image, 8, 8, SSD1306_WHITE); // Assumes image dimensions of 8x8. Adjust as necessary.
            }
            oled.display->setCursor(10, currentY + i * 15); // Starting X at 10 to leave space for image
            oled.display->print(lines[i].text);
        }

        oled.display->display();

        // Scroll
        currentY--;

        // Reset position after all lines have scrolled out of the screen
        if (currentY <= -(numLines * 15)) // Assuming 10 pixels height per line
        {
            currentY = oled.display->height();
        }
    }
}

void ScrollingTextAnimation::addLine(const char *text, const uint8_t *image)
{
    if (numLines < 18) // Make sure we don't exceed our array size
    {
        lines[numLines].text = text;
        lines[numLines].image = image;
        numLines++;
    }
}
