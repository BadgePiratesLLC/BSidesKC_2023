#include <stdio.h>
#include "badgepirates/eeprom.hpp"
#include "esp_spiffs.h"
#include "sys/stat.h"
#include "ArduinoJson.h"

#define DEBUG_EEPROM 1
static const size_t EEPROM_SIZE = 512;
static DynamicJsonDocument config_data(2048);

#define SPIFFS_PART "/spiffs"
#define CONFIG_FILE "config.json"

BMSEEPROM::BMSEEPROM()
{
    // Constructor code if needed
}

void BMSEEPROM::init()
{
    // Check our FS - create it if it's not there already
    esp_vfs_spiffs_conf_t conf = {
        .base_path = SPIFFS_PART,
        .partition_label = NULL,
        .max_files = 5,
        .format_if_mount_failed = true};

    esp_err_t ret = esp_vfs_spiffs_register(&conf);

    if (ret != ESP_OK)
    {
        if (ret == ESP_FAIL)
        {
            log_i("Failed to mount or format filesystem");
        }
        else if (ret == ESP_ERR_NOT_FOUND)
        {
            log_i("Failed to find SPIFFS partition");
        }
        else
        {
            log_i("Failed to initialize SPIFFS (%s)", esp_err_to_name(ret));
        }
        log_e("Created SPIFFS filesystem\n");
        return;
    }

    loadConfig();
}

// Assume that outsiders are only setting one thing, so commit right away
void BMSEEPROM::set(String key, String value)
{
    // Once again, stupid esp and strings
    config_data[key.c_str()] = value;
    saveConfig();
}

void BMSEEPROM::set(String key, JsonVariant value)
{
    // Once again, stupid esp and strings
    config_data[key.c_str()] = value;
    saveConfig();
}

String BMSEEPROM::get(String key)
{
    if (config_data.containsKey(key))
    {
        return config_data[key];
    }
    return "";
}

JsonVariant BMSEEPROM::getJSON(String key)
{
    JsonVariant result;
    if (config_data.containsKey(key))
    {

        result = config_data[key];
    }
    else
    {
        DynamicJsonDocument empty(0);
        result = empty;
    }
    return result;
}

void BMSEEPROM::loadConfig()
{
    String config = readFile(CONFIG_FILE);

    // No config data available, so stuff in some defaults
    if (config.length() == 0)
    {
        config_data["badge"] = "";
        config_data["event"] = "64cadc29e1c00285843b4166";
        saveConfig();
    }
    else
    {
        deserializeJson(config_data, config);
    }

    // Ensure certain keys are set to "0" if they are null or nonexistent
    const char *keys[] = {"lock", "flashlight", "magnifying", "camera", "notepad", "radio", "oledAnimation"};
    for (const char *key : keys)
    {
        if (!config_data.containsKey(key) || config_data[key].isNull())
        {
            config_data[key] = "0";
        }
    }

    // Save the configuration after ensuring the keys are set
    saveConfig();
}

void BMSEEPROM::saveConfig()
{
    // esp-idf is dumb about strings, so we have to convert it twice. Lame.
    String output;
    serializeJson(config_data, output);
    writeFile(CONFIG_FILE, output);
}

String BMSEEPROM::readFile(String filename)
{
    String result = "";
    struct stat st;
    String fullPath = String(SPIFFS_PART) + "/" + filename;
    if (stat(fullPath.c_str(), &st) == 0)
    {
        FILE *f = fopen(fullPath.c_str(), "r");

        char *buffer = (char *)malloc(st.st_size);
        if (buffer)
        {
            int read = fread(buffer, st.st_size, 1, f);

            if (read)
            {
                result = String(buffer, st.st_size);
            }
            free(buffer);
        }

        fclose(f);
    }

    return result;
}

void BMSEEPROM::writeFile(String filename, String contents)
{
    struct stat st;
    String fullPath = String(SPIFFS_PART) + "/" + filename;
    FILE *f = fopen(fullPath.c_str(), "w");
    fwrite(contents.c_str(), contents.length(), 1, f);
    fclose(f);
}

String BMSEEPROM::getCert()
{
    // Todo: fix this. It's broken.
    String pemData = readFile("lets-encrypt-r3.pem");
    if (pemData.length() == 0)
    {
        return "";
    }
    return pemData;
}