#pragma once
#include <vector>

class Tui
{
private:
    unsigned long oledInitializationTime;
    std::vector<String> parseCmd(String cmd);
    void processCmd(std::vector<String> cmd);
    String RemoveQuotes(String input);

public:
    void setup();
    void internalLoop();
};
