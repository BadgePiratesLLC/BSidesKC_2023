#pragma once

#include <map>

#include <Arduino.h>
#include <WiFi.h>

#include "badgepirates/eeprom.hpp"
#include "badgepirates/RickRoller.hpp"

#define WIFI_CONNECT_TIMEOUT_MS 5000

typedef enum
{
    WIFI_MGR_DISABLED,
    WIFI_MGR_AP,
    WIFI_MGR_RICK,
    WIFI_MGR_STA
} WifiModes;

typedef struct
{
    String ssid;
    String password;
    int priority;
    int RSSI;
    bool visible;
} wifiNetwork;

class WifiManager
{
private:
    WifiModes wifiMode = WIFI_MGR_DISABLED;
    WifiModes newWifiMode = WIFI_MGR_DISABLED;
    void SetupAP();
    void ScanNetworks();
    std::map<String, wifiNetwork> networks;
    void LoadNetworks();
    void SaveNetworks();

public:
    WifiManager();
    void setup();
    void internalLoop();
    bool isConnected();
    bool addNetwork(String ssid, String password, int priority);
    void disconnect();
    WifiModes getMode();
    void setMode(WifiModes newMode);
    int wifiNetworksFound();
    bool scanComplete();
};
