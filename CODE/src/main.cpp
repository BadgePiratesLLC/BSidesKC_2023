#include <Arduino.h>

#include "badgepirates/eeprom.hpp"
#include "badgepirates/oled.hpp"
#include "badgepirates/operatorLed.hpp"
#include "badgepirates/rgbLed.hpp"
#include "badgepirates/qrCodeDisplay.hpp"
#include "badgepirates/FastLedInclude.hpp"
#include "badgepirates/WiFiManager.hpp"
#include "badgepirates/AnimationManager.hpp"
#include "badgepirates/OledAnimationManager.hpp"
#include "badgepirates/webSocket.hpp"
#include "badgepirates/ota.hpp"
#include "badgepirates/tui.hpp"
#include "config.hpp"

const int all_leds[6] = {GPIO_NUM_10, GPIO_NUM_11, GPIO_NUM_12, GPIO_NUM_13, GPIO_NUM_14, GPIO_NUM_15};

Oled oled;
RGBLed rgbLed;
BMSEEPROM bmsData;
QRDisplay qrDisplay;
WifiManager wifiManager;
AnimationManager animationManager;
OledAnimationManager oledAnimationManager;
WebSocketManager webSocket;
OperatorLED operatorLed;
Buttons buttons;
Tui tui;

extern OTA updater;

static void processEvent(String eventData);
static int startupSplashTime;

void setup()
{
    Serial.begin(115200);
    bmsData.init();

    String lockValue = bmsData.get("lock");
    String flashlightValue = bmsData.get("flashlight");
    String magnifyingValue = bmsData.get("magnifying");
    String cameraValue = bmsData.get("camera");
    String notepadValue = bmsData.get("notepad");
    String radioValue = bmsData.get("radio");

    wifiManager.setup();
    wifiManager.setMode(WIFI_MGR_STA);

    tui.setup();

    // Serial.printf("Badge: %s \r\n Event: %s \r\n Version: %d \r\n Badge User: %s  \r\n LockLED: %s \r\n FlashlightLED: %s \r\n Magnifying Glass: %s \r\n CameraLED: %s \r\n NotepadLED: %s \r\n RadioLED: %s \r\n", bmsData.get("badge").c_str(), bmsData.get("event").c_str(), BADGE_VERSION, bmsData.get("badgeuser").c_str(), bmsData.get("lock").c_str(), bmsData.get("flashlight").c_str(), bmsData.get("magnifying").c_str(), bmsData.get("camera").c_str(), bmsData.get("notepad").c_str(), bmsData.get("radio").c_str());

    String url = "https://bms.badgepirates.com/register/" + bmsData.get("badge");
    Serial.print("URL: ");
    Serial.println(url);
    qrDisplay.generateQRCode(url);
    qrDisplay.drawQRSerial(qrDisplay.qrcode);

    // Show the splash for 3 seconds
    startupSplashTime = millis() + 3000;

    buttons.setup();
    oled.setup();
    oledAnimationManager.setup();
    animationManager.setup();
    rgbLed.setup();
    operatorLed.setup();
    Serial.println("{bskc23}-g1mm3flag");
}

void loop()
{

    operatorLed.loop();

    if (millis() > startupSplashTime)
    {
        animationManager.loop(millis());
        oledAnimationManager.loop(millis());
    }

    if (wifiManager.isConnected())
    {
        webSocket.setup();
    }

    if (webSocket.newDataAvailable())
    {
        processEvent(webSocket.getCurrentData());
    }
}

static void processEvent(String eventData)
{
    DynamicJsonDocument msg(2048);
    DeserializationError error = deserializeJson(msg, eventData);

    if (error)
    {
        log_i("Error parsing event: %s", error.f_str());
        return;
    }

    const String type = msg["type"].as<String>();
    if (type == "message")
    {
        String payload = msg["data"].as<String>();
        oledAnimationManager.displayText(payload.c_str());
    }
    else if (type == "badgeuser")
    {
        JsonObject payload = msg["data"].as<JsonObject>();
        if (payload.containsKey("badgeuser"))
        {
            // log_i("Setting badgeuser to %s", payload.c_str());
            bmsData.set("badgeuser", payload);
        }
        if (payload.containsKey("flags"))
        {
            String allFlags[] = {"lock", "flashlight", "magnifying", "camera", "notepad", "radio"};

            for (auto flag : allFlags)
            {
                bmsData.set(flag, "0");
            }
            JsonArray flags = payload["flags"].as<JsonArray>();
            for (auto flag : flags)
            {
                String foundFlag = flag.as<String>();
                log_i("Setting flag %s", foundFlag.c_str());
                bmsData.set(foundFlag, "1");
            }
        }
    }
    else if (type == "flag")
    {
        String payload = msg["data"].as<String>();
        String existingValue = bmsData.get(payload);

        // Explicitly check the value of existingValue
        bmsData.set(payload, existingValue == "1" ? "0" : "1");
    }
    else if (type == "command")
    {
    }
    else if (type == "upgrade")
    {
        updater.checkOTASync();
    }
}
