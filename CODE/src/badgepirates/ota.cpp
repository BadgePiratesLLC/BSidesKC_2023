#include "badgepirates/ota.hpp"
#include "badgepirates/WifiManager.hpp"
/*
 * BadgePirates OTA updater
 * Handles WiFi updates
 */

// The code here will change badge-to-badge
#pragma region Indication Code
// Progess indicators, should be LED/Screen only.
extern Oled oled;
extern WifiManager wifiManager;

// Print the status of the OTA operation to the screen
void OTA::oled_status(String msg)
{
    oled.display->clearDisplay();
    oled.display->setCursor(21, 0);
    oled.display->print("- OTA UPDATE -");
    oled.display->setCursor(12, 15);
    oled.display->print(msg);
    oled.display->display();
}

void OTA::indicate_start()
{
    oled_status("Wi-Fi connecting");
    for (int dot = 0; dot < NUM_LEDS; dot++)
    {
        leds[dot] = CRGB::Green;
        FastLED.show();

        delay(30);
    }
}

void OTA::indicate_wifi_connected()
{
    FastLED.setBrightness(30);
    oled_status("Retriving manifest");
    for (int dot = 0; dot < NUM_LEDS; dot++)
    {
        leds[dot] = CRGB::Blue;
        FastLED.show();

        delay(30);
    }
}

void OTA::indicate_manifest_load()
{
    FastLED.setBrightness(60);
    oled_status("Parsing manifest");
    for (int dot = 0; dot < NUM_LEDS; dot++)
    {
        leds[dot] = CRGB::Black;
        FastLED.show();

        delay(30);
    }
    for (int dot = 0; dot < NUM_LEDS; dot++)
    {
        leds[dot] = CRGB::Blue;
        FastLED.show();
        delay(30);
    }
}

void OTA::indicate_updating()
{
    oled_status("Installing...");
    for (int dot = 0; dot < NUM_LEDS; dot++)
    {
        leds[dot] = CRGB::Purple;
        FastLED.show();

        delay(30);
    }
}

// Error indicators
void OTA::indicate_error_wifi_unavailable()
{
    oled_status("Wi-Fi unavailable");
    for (int dot = 0; dot < NUM_LEDS; dot++)
    {
        leds[dot] = CRGB::Red;
        FastLED.show();
        delay(30);
    }
    for (int dot = 0; dot < NUM_LEDS; dot++)
    {
        leds[dot] = CRGB::Blue;
        FastLED.show();
        delay(30);
    }
    for (int dot = 0; dot < NUM_LEDS; dot++)
    {
        leds[dot] = CRGB::Red;
        FastLED.show();
        delay(30);
    }
}

void OTA::indicate_error_wifi_rejected()
{
    oled_status("Wi-Fi failed");
    for (int dot = 0; dot < NUM_LEDS; dot++)
    {
        leds[dot] = CRGB::Red;
        FastLED.show();
        delay(30);
    }
    for (int dot = 0; dot < NUM_LEDS; dot++)
    {
        leds[dot] = CRGB::White;
        FastLED.show();
        delay(30);
    }
    for (int dot = 0; dot < NUM_LEDS; dot++)
    {
        leds[dot] = CRGB::Red;
        FastLED.show();
        delay(30);
    }
}

void OTA::indicate_error_download()
{
    oled_status("Server error");
    for (int dot = 0; dot < NUM_LEDS; dot++)
    {
        leds[dot] = CRGB::Red;
        FastLED.show();
        delay(30);
    }
    for (int dot = 0; dot < NUM_LEDS; dot++)
    {
        leds[dot] = CRGB::Orange;
        FastLED.show();
        delay(30);
    }
    for (int dot = 0; dot < NUM_LEDS; dot++)
    {
        leds[dot] = CRGB::Red;
        FastLED.show();
        delay(30);
    }
}

void OTA::indicate_error_latest_version()
{
    oled_status("Running Latest!");
    for (int dot = 0; dot < NUM_LEDS; dot++)
    {
        leds[dot] = CRGB::Green;
        FastLED.show();
        delay(30);
    }
    for (int dot = 0; dot < NUM_LEDS; dot++)
    {
        leds[dot] = CRGB::Blue;
        FastLED.show();
        delay(30);
    }
    for (int dot = 0; dot < NUM_LEDS; dot++)
    {
        leds[dot] = CRGB::White;
        FastLED.show();
        delay(30);
    }
}
#pragma endregion

#pragma region OTA Code
// Checks whether or not our wifi can be seen
bool OTA::isWifiAvailable()
{
    FastLED.setBrightness(75);

    wifiManager.setMode(WIFI_MGR_STA);

    // Wait for scanning
    // scanComplete will time out eventually on its own and return true, so we don't need to do it here
    while (!wifiManager.scanComplete())
    {
        delay(200);
    }

#ifdef DEBUG
    log_i("[OTA] %d networks found", wifiManager.wifiNetworksFound());
#endif

    if (wifiManager.wifiNetworksFound() == 0)
    {
        log_d("[OTA] found no wifi networks, please check board and/or networks.");
    }
    else
    {
        // If we found networks, go ahead and move forward. Not sure yet if we can connect to one, but we'll sort that out later
        return true;
    }

    return false;
}

void OTA::checkOTASync()
{

    // Start
    indicate_start();
    if (!wifiManager.isConnected())
    {

        // Check wifi
        if (!isWifiAvailable())
        {
            indicate_error_wifi_unavailable();
            log_i("[OTA] Wifi wasn't found.");
            delay(10000);
            ESP.restart();
        }

        // Wait up to 10 seconds for a connection to a useable network
        int count = 0;
        while (!wifiManager.isConnected() && count < 100)
        {
            delay(100);
            count++;
        }

        // Check if wifi failed to connect
        if (!wifiManager.isConnected())
        {
            Serial.print("[OTA] WiFi failed to connect.");
            indicate_error_wifi_rejected();

            ESP.restart();
            return;
        }
    }

    Serial.print("[OTA] WiFi connected.");

    // S1 - WiFi Connected
    indicate_wifi_connected();

    // I - Manifest downloaded
    indicate_manifest_load();
    int newVersion = getServerVersion();

    if (newVersion > 0)
    {
        // Check
        log_i("[OTA] Current firmware version: %d", BADGE_VERSION);
        log_i("[OTA] Available firmware version: %d", newVersion);

        if (newVersion > BADGE_VERSION)
        {
            // D - Downloading update
            indicate_updating();

            log_i("[OTA] Update is downloading/installing...");
            log_i("[OTA] Do not power off the badge");
            log_i("%s", OTA_DOWNLOAD_URL);
            t_httpUpdate_return ret = ESPhttpUpdate.update(OTA_DOWNLOAD_URL);

            // I don't think this code actually is triggered unless HTTP fails.
            switch (ret)
            {
            case HTTP_UPDATE_FAILED:
                Serial.printf("[OTA] Download failed, Error: (%d): %s", ESPhttpUpdate.getLastError(), ESPhttpUpdate.getLastErrorString().c_str());

                indicate_error_download();
                for (;;)
                {
                    delay(200);
                }
                break;

            case HTTP_UPDATE_NO_UPDATES:
                log_i("[OTA] Your badge is running the latest firmware");
                // Latest version
                indicate_error_latest_version();
                delay(10000);
                ESP.restart();
                break;

            case HTTP_UPDATE_OK:
                log_i("[OTA] Your badge is updated!");
                ESP.restart();
                break;
            }
        }
        else
        {
            // Latest version
            indicate_error_latest_version();
            delay(10000);
            ESP.restart();
        }
        WiFi.disconnect();
    }
    else
    {
        log_i("[OTA] Download server error. Please try again later.");
        indicate_error_download();
        delay(10000);
        ESP.restart();
    }
}

int OTA::getServerVersion()
{
    // This function is seperate so it will release it's memory when done
    // We don't have enough to run normal code, HTTPClient and the updater client
    HTTPClient httpClient;

    // Check update server
    // Try to download the manifest(pull file version, containing an int)
    httpClient.begin(OTA_MANIFEST_URL);
    log_i("[OTA] Downloading manifest from: %s", OTA_MANIFEST_URL);
    int httpCode = httpClient.GET();

    if (httpCode == 200)
    {
        // I - Manifest downloaded
        indicate_manifest_load();

        // Check
        String newFWVersion = httpClient.getString();
        return newFWVersion.toInt();
    }
    else
    {
        log_d("HTTP didn't throw a 200 :(");
        log_d("Code: %d", httpCode);
    }
    return 0;
}
#pragma endregion