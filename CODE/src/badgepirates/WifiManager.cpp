#include "badgepirates/WifiManager.hpp"
#include "ArduinoJson.h"

extern BMSEEPROM bmsData;
RickRoller rickRoller;

static unsigned long connectTimer = 0;
static bool wifiLaunched = false;
static int foundNetworkCount = 0;

static void wifiManagerLoop(void *param)
{
    WifiManager *manager = (WifiManager *)param;
    manager->internalLoop();
}

WifiManager::WifiManager()
{
}

void WifiManager::setup()
{
    if (wifiLaunched)
    {
        return;
    }
    wifiLaunched = true;
    String badge = bmsData.get("badge");
    String lastSix = badge.substring(badge.length() - 6);
    String hostname = "BSidesKC 2023 - " + lastSix;

    WiFi.setHostname(hostname.c_str());

    // Add a default network
    wifiNetwork main = {"steamnet", "cloudybits", 60, -255, false};
    wifiNetwork plexpod = {"PlexpodGuests", "@plexpod", 80, -255, false};
    wifiNetwork bpbackup = {"badgepirates", "APirateIsFr33", 100, -255, false};
    networks[main.ssid] = main;
    networks[plexpod.ssid] = plexpod;
    networks[bpbackup.ssid] = bpbackup;

#include "badgepirates/developer_network.h"

    LoadNetworks();

    // Start the background manager task
    xTaskCreate(wifiManagerLoop, "Wifi Manager Task", 8192, this, 1, NULL);
}

void WifiManager::internalLoop()
{
    while (true)
    {
        if (wifiMode != newWifiMode)
        {
            // Shutdown the old (if it needs it)
            switch (wifiMode)
            {
            case WIFI_MGR_RICK:
                rickRoller.stopRickRolling();
                break;
            case WIFI_MGR_STA:
                // If we're connected, disconnect gracefully
                if (isConnected())
                {
                    WiFi.softAPdisconnect(true);
                }
                break;
            case WIFI_MGR_DISABLED:
            case WIFI_MGR_AP:
            default:
                break;
            }

            // Setup the new
            switch (newWifiMode)
            {
            case WIFI_MGR_DISABLED:
                WiFi.mode(WIFI_MODE_NULL);
                // Give it a bit to shut down
                vTaskDelay(250 / portTICK_PERIOD_MS);
                break;
            case WIFI_MGR_AP:
                SetupAP();
                break;
            case WIFI_MGR_RICK:
                rickRoller.startRickRolling();
                break;
            case WIFI_MGR_STA:
                WiFi.mode(WIFI_MODE_STA);
                vTaskDelay(100 / portTICK_PERIOD_MS);
                break;
            }

            wifiMode = newWifiMode;
        }

        // Periodic tasks for each mode
        switch (wifiMode)
        {
        case WIFI_MGR_RICK:
            rickRoller.loop();
            break;
        case WIFI_MGR_DISABLED:
        case WIFI_MGR_AP:
        case WIFI_MGR_STA:
            if (!isConnected())
            {
                ScanNetworks();
            }
        }
        vTaskDelay(500 / portTICK_PERIOD_MS);
    }
}

bool WifiManager::isConnected()
{
    return WiFi.status() == WL_CONNECTED;
}

bool WifiManager::addNetwork(String ssid, String password, int priority)
{
    wifiNetwork testing = {ssid, password, priority, -255, false};
    networks[ssid] = testing;

    SaveNetworks();
    return true;
}

void WifiManager::disconnect()
{
    WiFi.disconnect();
}

WifiModes WifiManager::getMode()
{
    return wifiMode;
}

void WifiManager::setMode(WifiModes newMode)
{
    if (newMode != newWifiMode)
    {
        newWifiMode = newMode;
    }
}

void WifiManager::SetupAP()
{
    WiFi.mode(WIFI_AP_STA);
}

void WifiManager::ScanNetworks()
{
    // If we've got a connection or pending connection, give it some time to finish
    if (connectTimer > millis() || isConnected())
    {
        return;
    }

    connectTimer = 0;
    int scanResult = WiFi.scanNetworks(false);
    if (scanResult < 0)
    {
        log_i("Scan failed\n");
        WiFi.scanDelete();
    }
    foundNetworkCount = scanResult;

    // If we got here, a scan completed and we have networks to check
    // Clear visibility for everything to start off
    for (auto const &x : networks)
    {
        networks[x.first].visible = false;
        networks[x.first].RSSI = -255;
    }

    log_i("Network scan results:\n");

    for (int i = 0; i < scanResult; i++)
    {
        log_i("Found network: (%d) %s (%d) RSSI\n", i, WiFi.SSID(i), WiFi.RSSI(i));

        if (networks.count(WiFi.SSID(i)))
        {
            networks[WiFi.SSID(i)].RSSI = WiFi.RSSI(i);
            networks[WiFi.SSID(i)].visible = true;
        }
    }

    wifiNetwork nullNet = {"", "", 101, -255, false};
    wifiNetwork &bestNetwork = nullNet;

    for (auto const &x : networks)
    {
        if (x.second.visible)
        {
            // We currently prefer higher RSSI over higher priority. The logic below can be adjusted to change prioritization behavior.
            bool copyNetwork = false;
            // if (x.second.RSSI > bestNetwork.RSSI)
            // {
            //     copyNetwork = true;
            // }
            // else
            // {
            if (x.second.priority < bestNetwork.priority)
            {
                copyNetwork = true;
            }
            // }

            if (copyNetwork)
            {
                bestNetwork = x.second;
            }
        }
    }

    if (bestNetwork.ssid.length() > 0)
    {
        log_i("Connecting to network %s with priority %d and RSSI %d\n", bestNetwork.ssid, bestNetwork.priority, bestNetwork.RSSI);
        WiFi.begin(bestNetwork.ssid, bestNetwork.password);
        // Give a little delay to connect so we don't spam it
        vTaskDelay(500 / portTICK_PERIOD_MS);
        connectTimer = millis() + WIFI_CONNECT_TIMEOUT_MS;
    }

    WiFi.scanDelete();
}

void WifiManager::LoadNetworks()
{
    log_d("Loading WiFi configuration\n");
    JsonVariant jsonData = bmsData.getJSON("WiFi");

    int networkCount = 0;
    JsonArray jsonNetworks = jsonData.as<JsonArray>();
    for (JsonVariant network : jsonNetworks)
    {
        String ssid = network["ssid"].as<String>();
        String password = network["password"].as<String>();
        int priority = network["priority"].as<int>();

        if (ssid.length() > 0)
        {
            wifiNetwork newNetwork = {ssid, password, priority, -255, false};
            networks[ssid] = newNetwork;
            networkCount++;
        }
    }

    // If there's nothing in the file, save off at least our defaults
    if (networkCount == 0)
    {
        SaveNetworks();
    }
}

void WifiManager::SaveNetworks()
{
    DynamicJsonDocument jsonData(2048);
    int pos = 0;
    for (auto const &x : networks)
    {
        jsonData[pos]["ssid"] = x.second.ssid;
        jsonData[pos]["password"] = x.second.password;
        jsonData[pos]["priority"] = x.second.priority;
    }

    log_d("Saving WiFi configuration\n");
    bmsData.set("WiFi", jsonData);
}

int WifiManager::wifiNetworksFound()
{
    return foundNetworkCount;
}

bool WifiManager::scanComplete()
{
    if (WiFi.scanComplete() == WIFI_SCAN_RUNNING)
    {
        return false;
    }
    return true;
}