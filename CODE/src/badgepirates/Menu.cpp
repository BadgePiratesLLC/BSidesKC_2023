#include <functional>
#include "badgepirates/Menu.hpp"
#include "badgepirates/images/bp-logo-tiny.h"
#include "badgepirates/fonts/31khz_futuristic141216pt7b.h"
#include "badgepirates/ota.hpp"
#include "badgepirates/images/wifi-logo.h"

extern Oled oled;
extern Buttons buttons;
extern BMSEEPROM bmsData;
extern QRDisplay qrDisplay;
extern WifiManager wifiManager;
extern OledAnimationManager oledAnimationManager;
extern OTA updater;

// Maximum number of menu options
constexpr int MAX_OPTIONS = 7;

int currentWifiSubmenuSize = 0; // Initialize with a default size

// Define a maximum depth for submenus (how many levels deep can we go)
constexpr int MAX_MENU_DEPTH = 10;

// Our menu stack will be an array of MenuItem pointers
MenuItem *menuStack[MAX_MENU_DEPTH];

void setOledAnimation(String animation)
{
    if (animation != "6")
    {
        bmsData.set("oledAnimation", animation);
    }
    oledAnimationManager.isPlayingAnimation = true;
    oledAnimationManager.selectedOledAnimation = animation;
}

void startOTA()
{
    updater.checkOTASync();
}

MenuItem animationsSubmenu[MAX_OPTIONS] = {
    {"Bounce", nullptr, [&]()
     { setOledAnimation("1"); }},
    {"Grow", nullptr, [&]()
     { setOledAnimation("2"); }},
    {"Matrix", nullptr, [&]()
     { setOledAnimation("3"); }},
    {"Troll", nullptr, [&]()
     { setOledAnimation("4"); }},
    {"3D Dots", nullptr, [&]()
     { setOledAnimation("5"); }},
};

MenuItem mainMenu[MAX_OPTIONS] = {
    {"Animations", animationsSubmenu, nullptr},
    {"About", nullptr, [&]()
     { setOledAnimation("6"); }},
    {"Update Firmware", nullptr, [&]()
     { startOTA(); }},
};

Menu::Menu()
{
    menuStack[0] = mainMenu;
}

void Menu::drawMenu()
{
    // Retrieve the current menu from the stack for drawing
    MenuItem *currentMenu = menuStack[currentMenuDepth];

    oled.display->clearDisplay();
    if (wifiManager.isConnected())
    {
        oled.display->drawBitmap(120, 0, epd_bitmap_wifi_logo, 8, 8, SSD1306_WHITE);
    }

    for (int i = 0; i < 3 && (i + firstDisplayedOption) < MAX_OPTIONS; i++)
    {
        oled.display->setCursor(0, i * 10); // Assuming 10 pixels height per line

        if ((i + firstDisplayedOption) == selectedOption)
        {
            oled.display->drawBitmap(xPos, i * 10, epd_bitmap_bp_tiny_pixel, 8, 8, SSD1306_WHITE);
        }

        oled.display->setCursor(10, i * 10); // Assuming 10 pixels height per line
        oled.display->print(currentMenu[i + firstDisplayedOption].label);
        oled.display->print("\n");
    }

    oled.display->display();
}

int getMenuItemCount(MenuItem *menu)
{
    int count = 0;
    while (menu && menu[count].label != nullptr && count < MAX_OPTIONS)
    {
        count++;
    }
    return count;
}
void Menu::setup()
{
    previousMillis = 0;
    oled.display->setFont(&khz31_futuristic141216pt7b);
    drawMenu();
}

void Menu::loop()
{
    // Retrieve the current menu from the stack
    MenuItem *currentMenu = menuStack[currentMenuDepth]; // Retrieve the current menu from the stack

    if (currentMenu) // check if currentMenu is not nullptr
    {
        currentMenuSize = getMenuItemCount(currentMenu);
    }

    if (buttons.isPressedLeft())
    {
        currentMenuDepth = 0;
        selectedOption = 0;
        firstDisplayedOption = 0;
        isMenuAwake = true; // Wake up the menu
        if (oledAnimationManager.selectedOledAnimation == "6")
        {
            oledAnimationManager.selectedOledAnimation = oledAnimationManager.lastSelectedOledAnimation;
        }
    }
    else if (buttons.isPressedUp())
    {
        if (!isMenuAwake)
        {
            isMenuAwake = true;
            if (oledAnimationManager.selectedOledAnimation == "6")
            {
                oledAnimationManager.selectedOledAnimation = oledAnimationManager.lastSelectedOledAnimation;
            }
        }
        else if (selectedOption > 0)
        {
            selectedOption--;

            if (selectedOption < firstDisplayedOption)
            {
                firstDisplayedOption--;
            }
        }
    }
    else if (buttons.isPressedDown())
    {
        if (oledAnimationManager.selectedOledAnimation == "6")
        {
            oledAnimationManager.selectedOledAnimation = oledAnimationManager.lastSelectedOledAnimation;
        }
        if (!isMenuAwake)
        {
            isMenuAwake = true;
        }
        else if (selectedOption < (currentMenuSize - 1))
        {
            selectedOption++;

            if ((selectedOption - firstDisplayedOption) > 2)
            {
                firstDisplayedOption++;
            }
        }
    }
    else if (buttons.isPressedCenter() || buttons.isPressedRight())
    {
        if (!isMenuAwake)
        {
            isMenuAwake = true;
            if (oledAnimationManager.selectedOledAnimation == "6")
            {
                oledAnimationManager.selectedOledAnimation = oledAnimationManager.lastSelectedOledAnimation;
            }
        }
        else if (currentMenu[selectedOption].submenu)
        {
            // Enter the submenu
            currentMenuDepth++;
            menuStack[currentMenuDepth] = currentMenu[selectedOption].submenu;
            selectedOption = 0;
            firstDisplayedOption = 0;
            if (oledAnimationManager.selectedOledAnimation == "6")
            {
                oledAnimationManager.selectedOledAnimation = oledAnimationManager.lastSelectedOledAnimation;
            }
        }
        else if (currentMenu[selectedOption].action)
        {
            // Execute the associated action
            currentMenu[selectedOption].action();
        }
    }

    drawMenu();
}

void Menu::setLedAnimation(String animation)
{
}

int Menu::getSelectedOption() const
{
    return selectedOption;
}

void Menu::setSelectedOption(int option)
{
    if (option >= 0 && option < MAX_OPTIONS)
    {
        selectedOption = option;
    }
}

int Menu::getMenuDepth() const
{
    return currentMenuDepth;
}

void Menu::setMenuDepth(int depth)
{
    if (depth >= 0 && depth < MAX_MENU_DEPTH)
    {
        currentMenuDepth = depth;
    }
}

void Menu::setMainMenu()
{
    currentMenuDepth = 0;
    selectedOption = 0;
    menuStack[0] = mainMenu;
}
