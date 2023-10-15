#include "esp_log.h"
#include "config.hpp"
#include "badgepirates/webSocket.hpp"
#include "badgepirates/eeprom.hpp"
#include "badgepirates/developer_websocket_server.h"
#include <esp_websocket_client.h>
#include "ArduinoJson.h"

#ifdef WS_URI
static const char *ws_uri = WS_URI;
#else
static const char *ws_uri = "wss://api.badgepirates.com/ws";
#endif

static esp_websocket_client_handle_t client = nullptr;
static esp_websocket_client_config_t ws_cfg;
static std::mutex dataMutex;
static bool newData = false;
static String currentData = "";
static bool startedUp = false;
extern OledAnimationManager oledAnimationManager;
extern WifiManager wifiManager;
extern BMSEEPROM bmsData;

#define NO_DATA_TIMEOUT_SEC 60

static void websocket_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data)
{
    esp_websocket_event_data_t *data = static_cast<esp_websocket_event_data_t *>(event_data);
    WebSocketManager *webSocket = (WebSocketManager *)handler_args;
    switch (event_id)
    {
    case WEBSOCKET_EVENT_CONNECTED:
        log_i("WEBSOCKET_EVENT_CONNECTED");
        webSocket->sendRegistration();
        break;
    case WEBSOCKET_EVENT_DISCONNECTED:
        log_i("WEBSOCKET_EVENT_DISCONNECTED");
        break;
    case WEBSOCKET_EVENT_DATA:
        // We also get ping frames and could get others, but we ignore them
        // Only care about text (opcode 1)
        if (data->op_code == 1)
        {
            log_i("WEBSOCKET_EVENT_DATA");
            log_i("Received opcode=%d", data->op_code);
            // log_i("Received=%.*s", data->data_len, data->data_ptr);
            // Message debugging - prints the whole thing in Hex
            // char hexOutput[1024];
            // int len = 0;
            // for (int i = 0; i < data->data_len; i++)
            // {
            //     char hexdigit = data->data_ptr[i];
            //     len += snprintf(hexOutput + len, sizeof(hexOutput) - len, "%x,", hexdigit);
            // }
            // log_i("Hex: %s", hexOutput);
            log_i("Total payload length=%d, data_len=%d, current payload offset=%d\r\n", data->payload_len, data->data_len, data->payload_offset);

            if (data->data_len > 0)
            {
                // Don't let anybody read while we're updating it
                const std::lock_guard<std::mutex> lock(dataMutex);
                currentData = String(data->data_ptr, data->data_len);
                newData = true;
            }
        }
        break;
    case WEBSOCKET_EVENT_ERROR:
        log_i("WEBSOCKET_EVENT_ERROR");
        break;
    }
}

static const char letsEncryptPem[] = "-----BEGIN CERTIFICATE-----\nMIIFFjCCAv6gAwIBAgIRAJErCErPDBinU/bWLiWnX1owDQYJKoZIhvcNAQELBQAw\nTzELMAkGA1UEBhMCVVMxKTAnBgNVBAoTIEludGVybmV0IFNlY3VyaXR5IFJlc2Vh\ncmNoIEdyb3VwMRUwEwYDVQQDEwxJU1JHIFJvb3QgWDEwHhcNMjAwOTA0MDAwMDAw\nWhcNMjUwOTE1MTYwMDAwWjAyMQswCQYDVQQGEwJVUzEWMBQGA1UEChMNTGV0J3Mg\nRW5jcnlwdDELMAkGA1UEAxMCUjMwggEiMA0GCSqGSIb3DQEBAQUAA4IBDwAwggEK\nAoIBAQC7AhUozPaglNMPEuyNVZLD+ILxmaZ6QoinXSaqtSu5xUyxr45r+XXIo9cP\nR5QUVTVXjJ6oojkZ9YI8QqlObvU7wy7bjcCwXPNZOOftz2nwWgsbvsCUJCWH+jdx\nsxPnHKzhm+/b5DtFUkWWqcFTzjTIUu61ru2P3mBw4qVUq7ZtDpelQDRrK9O8Zutm\nNHz6a4uPVymZ+DAXXbpyb/uBxa3Shlg9F8fnCbvxK/eG3MHacV3URuPMrSXBiLxg\nZ3Vms/EY96Jc5lP/Ooi2R6X/ExjqmAl3P51T+c8B5fWmcBcUr2Ok/5mzk53cU6cG\n/kiFHaFpriV1uxPMUgP17VGhi9sVAgMBAAGjggEIMIIBBDAOBgNVHQ8BAf8EBAMC\nAYYwHQYDVR0lBBYwFAYIKwYBBQUHAwIGCCsGAQUFBwMBMBIGA1UdEwEB/wQIMAYB\nAf8CAQAwHQYDVR0OBBYEFBQusxe3WFbLrlAJQOYfr52LFMLGMB8GA1UdIwQYMBaA\nFHm0WeZ7tuXkAXOACIjIGlj26ZtuMDIGCCsGAQUFBwEBBCYwJDAiBggrBgEFBQcw\nAoYWaHR0cDovL3gxLmkubGVuY3Iub3JnLzAnBgNVHR8EIDAeMBygGqAYhhZodHRw\nOi8veDEuYy5sZW5jci5vcmcvMCIGA1UdIAQbMBkwCAYGZ4EMAQIBMA0GCysGAQQB\ngt8TAQEBMA0GCSqGSIb3DQEBCwUAA4ICAQCFyk5HPqP3hUSFvNVneLKYY611TR6W\nPTNlclQtgaDqw+34IL9fzLdwALduO/ZelN7kIJ+m74uyA+eitRY8kc607TkC53wl\nikfmZW4/RvTZ8M6UK+5UzhK8jCdLuMGYL6KvzXGRSgi3yLgjewQtCPkIVz6D2QQz\nCkcheAmCJ8MqyJu5zlzyZMjAvnnAT45tRAxekrsu94sQ4egdRCnbWSDtY7kh+BIm\nlJNXoB1lBMEKIq4QDUOXoRgffuDghje1WrG9ML+Hbisq/yFOGwXD9RiX8F6sw6W4\navAuvDszue5L3sz85K+EC4Y/wFVDNvZo4TYXao6Z0f+lQKc0t8DQYzk1OXVu8rp2\nyJMC6alLbBfODALZvYH7n7do1AZls4I9d1P4jnkDrQoxB3UqQ9hVl3LEKQ73xF1O\nyK5GhDDX8oVfGKF5u+decIsH4YaTw7mP3GFxJSqv3+0lUFJoi5Lc5da149p90Ids\nhCExroL1+7mryIkXPeFM5TgO9r0rvZaBFOvV2z0gp35Z0+L4WPlbuEjN/lxPFin+\nHlUjr8gRsI3qfJOQFy/9rKIJR0Y/8Omwt/8oTWgy1mdeHmmjk7j1nYsvC9JSQ6Zv\nMldlTTKB3zhThV1+XWYp6rjd5JW1zbVWEkLNxE7GJThEUG3szgBVGP7pSWTUTsqX\nnLRbwHOoq7hHwg==\n-----END CERTIFICATE-----\n";

WebSocketManager::WebSocketManager()
{
}

void WebSocketManager::setup()
{
    if (startedUp || !wifiManager.isConnected())
    {
        return;
    }

    startedUp = true;
    // This blindly fires off the websocket client, whether we have a wifi connection or not
    if (client == nullptr)
    {
        // Load our certificate

        ws_cfg.uri = ws_uri;
        if (strncmp(ws_uri, "wss", 3) == 0)
        {
            ws_cfg.cert_pem = letsEncryptPem;
        }

        log_i("Connecting to %s...", ws_uri);

        client = esp_websocket_client_init(&ws_cfg);
        esp_websocket_register_events(client, WEBSOCKET_EVENT_ANY, websocket_event_handler, static_cast<void *>(this));

        esp_websocket_client_start(client);
    }
    else
    {
        log_i("Client already created.");
    }
}

String WebSocketManager::getCurrentData()
{
    // Lock while we do this and return a copy (since the original might get munged)
    const std::lock_guard<std::mutex> lock(dataMutex);
    String retData = currentData;
    newData = false;
    return retData;
}

bool WebSocketManager::sendData(String data)
{
    if (client == nullptr)
    {
        return false;
    }
    int sentLen = esp_websocket_client_send_text(client, data.c_str(), data.length(), 500 / portTICK_RATE_MS);
    if (sentLen == data.length())
    {
        return true;
    }
    return false;
}

bool WebSocketManager::isOnline()
{
    // I assume the client is thread safe. Lock here if it turns out not to be
    if (client != nullptr && esp_websocket_client_is_connected(client))
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool WebSocketManager::newDataAvailable()
{
    // Lock while we do this
    const std::lock_guard<std::mutex> lock(dataMutex);
    return newData;
}

void WebSocketManager::sendRegistration()
{
    log_d("Sending registration to WS\n");
    DynamicJsonDocument reg(256);
    reg["type"] = "registration";
    reg["data"]["id"] = bmsData.get("badge");
    reg["data"]["version"] = BADGE_VERSION;
    String output = "";
    serializeJson(reg, output);
    sendData(output);
}