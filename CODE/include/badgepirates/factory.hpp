#pragma once
#define AUTOMATIC_UPDATES           // Comment out to disable, Does not require boot to be pressed to initialize an update.
#define AUTOMATIC_UPDATE_DELAY 5000 // Wait X second before beginning the update process

#include <Arduino.h>

class Factory
{
private:
public:
    void setup();
    void loop();
};