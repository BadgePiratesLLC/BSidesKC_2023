#pragma once
#include "ArduinoJson.h"

class BMSEEPROM
{
private:
    void loadConfig();
    void saveConfig();
    String readFile(String filename);
    void writeFile(String filename, String contents);

public:
    BMSEEPROM();
    void init();
    void set(String key, String value);
    void set(String key, JsonVariant value);
    String get(String key);
    JsonVariant getJSON(String key);
    String badge, event, version;
    String getCert();
};