#pragma once

#include "badgepirates/oledAnimationManager.hpp"
#include "WifiManager.hpp"
#include <mutex>

class WebSocketManager
{
private:
    
public:
    WebSocketManager();

    void setup();

    String getCurrentData();
    bool sendData(String data);
    bool isOnline();
    bool newDataAvailable();
    void sendRegistration();
};
