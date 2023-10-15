#pragma once

#include <Arduino.h>
#include "badgepirates/MenuItem.hpp"
#include "badgepirates/eeprom.hpp"
#include "badgepirates/oled.hpp"
#include "badgepirates/buttons.hpp"
#include "badgepirates/qrCodeDisplay.hpp"
#include "badgepirates/OledAnimationManager.hpp"
#include "badgepirates/WifiManager.hpp"

class Menu
{
public:
    Menu();
    void setup();
    void loop();
    void drawMenu();
    void setLedAnimation(String animation);
    void setOledAnimation(String animation);
    int getMenuDepth() const;
    void setMenuDepth(int depth);
    int getSelectedOption() const;
    void setSelectedOption(int option);
    int selectedOption = 0;
    int currentMenuDepth = 0;
    bool isMenuAwake = false;
    void setMainMenu();

private:
    int currentMenuSize = 0;
    int firstDisplayedOption = 0;
    unsigned long previousMillis;
    const long interval = 75;
    int xPos;
    void scanForAps();
    std::vector<String> scannedAPs;
    void connectToAP(String ssid);
};