#include "badgepirates/RickRoller.hpp"

extern WifiManager wifiManager;

const int update_interval = 20000;

RickRoller::RickRoller()
{
    rickRolling = false;
}

void RickRoller::startRickRolling()
{
    if (rickRolling)
        return;

    wifiManager.disconnect();
    WiFi.mode(WIFI_AP);
    rickRolling = true;

    changeSSID(); // Call this initially to setup the first SSID
}

void RickRoller::stopRickRolling()
{
    rickRolling = false;
    wifiManager.disconnect();
}

void RickRoller::loop()
{
    if (rickRolling && millis() - lastChangeMillis > interval)
    {
        changeSSID();
        lastChangeMillis = millis();
    }
}
void RickRoller::changeSSID()
{
    Serial.print("Updating SSID to ");
    Serial.println(lyricLines[currentLyricIndex]);
    WiFi.softAP(lyricLines[currentLyricIndex].c_str());
    currentLyricIndex = (currentLyricIndex + 1) % (sizeof(lyricLines) / sizeof(String));
}