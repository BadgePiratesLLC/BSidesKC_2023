#pragma once

#include <WiFi.h>
#include <vector>

#include "badgepirates/WifiManager.hpp"

class RickRoller
{
private:
    bool rickRolling;

    void changeSSID();
    unsigned long lastChangeMillis = 0;  // Time of the last SSID change
    const unsigned long interval = 1000; // Interval for SSID change (1 second)
    int currentLyricIndex = 0;           // Index of the current lyric being broadcasted

    String lyricLines[11] = {
        "Were no strangers to love",
        "You know the rules and so do I",
        "A full commitments what Im thinking of",
        "You wouldnt get this from any other guy"
        "I just wanna tell you how Im feeling",
        "Gotta make you understand",
        "Never gonna give you up",
        "Never gonna let you down",
        "Never gonna run around and desert you",
        "Never gonna make you cry",
        "Never gonna say goodbye",
        "Never gonna tell a lie and hurt you"};

public:
    RickRoller();
    void setup();
    void loop();
    void startRickRolling();
    void stopRickRolling();
};
