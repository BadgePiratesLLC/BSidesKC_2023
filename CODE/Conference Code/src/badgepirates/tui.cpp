#include <Arduino.h>
#include "config.hpp"

#include "badgepirates/tui.hpp"
#include "badgepirates/WifiManager.hpp"
#include "badgepirates/AnimationManager.hpp"

static bool tuiLaunched = false;

extern WifiManager wifiManager;
extern AnimationManager animationManager;

static void tuiLoop(void *param)
{
    Tui *tui = (Tui *)param;
    tui->internalLoop();
}

String Tui::RemoveQuotes(String input)
{
    String output = "";
    for (auto c : input)
    {
        if (c != '"')
        {
            output += c;
        }
    }
    return output;
}

std::vector<String> Tui::parseCmd(String command)
{
    std::vector<String> v;
    size_t i = 0, j = 0;

    while (i < command.length())
    {
        if (command[i] == ' ' || i == 0)
        {
            // eat the space
            if (command[i] == ' ')
            {
                i++;
            }
            // Capture quoted elements
            if (i < command.length() && command[i] == '\"')
            {
                j = i + 1;
                while (j < command.length() && command[j++] != '\"')
                    ;

                auto output = String(&command[i], j - i);
                v.push_back(output);
                i = j;
                continue;
            }

            // Non quoted elements
            j = i + 1;
            while (j < command.length() && command[j++] != ' ')
                ;
            auto output = String(&command[i], j - i - (i ? 1 : 0));
            v.push_back(output);
        }
        i++;
    }
    return v;
}

void Tui::processCmd(std::vector<String> cmd)
{
    if (cmd.size() == 0)
    {
        Serial.println("Bad command or filename");
        return;
    }

    String command = cmd[0];
    // wait for data available
    command.trim();
    command.toUpperCase();
    if (command == "HELP" || command == "?")
    {
        Serial.println("Available commands:");
        Serial.println("help - View this help");
        Serial.println("wifi <SSID> <Password> - SSID and password can be quoted if they have spaces");
        // Serial.println("animate <SSID> <Password> - SSID and password can be quoted if they have spaces");
    }
    else if (command == "VER")
    {
        Serial.printf("BadgePirateOS Version %d\n", BADGE_VERSION);
    }
    else if (command == "ANIMATE" || command == "ANIMATE.EXE")
    {
        if (cmd.size() < 2)
        {
            Serial.println("Usage: animate <animation> - Choose an animation");
            return;
        }
        String animation = RemoveQuotes(cmd[1]);
        if (animationManager.selectAnimationHack(animation.toInt()))
        {
            Serial.println("Animation selected.");
        }
        else
        {
            Serial.printf("Invalid animation selected: %s\n", animation.c_str());
        }
    }
    else if (command == "WIFI")
    {
        if (cmd.size() < 2)
        {
            Serial.println("Usage: wifi <SSID> <Password> - SSID and password can be quoted if they have spaces");
            return;
        }
        String SSID = RemoveQuotes(cmd[1]);
        String Password = "";
        if (cmd.size() >= 3)
        {
            Password = RemoveQuotes(cmd[2]);
        }
        Serial.printf("Adding wifi network: %s with password %s\n", SSID.c_str(), Password.c_str());
        wifiManager.addNetwork(SSID, Password, 60);
    }
    else if (command == "HAX" || command == "HAX.EXE")
    {
        Serial.println("Hax? Nice try. {bskc23}-TuiHax!");
    }
    else if (command == "LS")
    {
        Serial.println("LS? What? Do you think this is Linux?");
    }
    else if (command == "DIR")
    {
        String arg = "";
        if (cmd.size() >= 2)
        {
            arg = cmd[1];
            arg.trim();
        }
        Serial.println(" Volume in drive F is PIRATES.");
        Serial.println(" Volume Serial Number is 0C31-1BB2");
        Serial.println("");
        Serial.println(" Directory of F:\\");
        Serial.println("");
        Serial.println("07/16/2023  04:27 PM    <DIR>          .");
        Serial.println("07/16/2023  04:27 PM    <DIR>          ..");
        Serial.println("03/03/2023  09:50 PM           503,055 WIFI.EXE");
        if (arg == "/a:h")
        {
            Serial.println("03/03/2023  06:03 PM         2,356,703 ANIMATE.EXE");
        }
        Serial.println("07/22/2023  06:03 PM         1,825,295 HAX.EXE");
        Serial.println("11/16/2022  06:38 PM             3,706 NOTICE");
        Serial.println("               4 File(s)     4,688,759 bytes");
        Serial.println("               0 Dir(s)  100,416,262,144 bytes free");
    }
    else
    {
        Serial.println("Bad command or filename");
    }
}

void Tui::setup()
{
    if (tuiLaunched)
    {
        return;
    }

    tuiLaunched = true;
    // Start the background manager task
    xTaskCreate(tuiLoop, "Tui Task", 8192, this, 1, NULL);
}

void Tui::internalLoop()
{
    Serial.print("\nBadgePiratesOS F:\\>");
    String cmd = "";

    while (true)
    {
        while (Serial.available() == 0)
        {
            vTaskDelay(10 / portTICK_PERIOD_MS);
        }
        int result = 0;
        while (result != -1)
        {
            result = Serial.read();
            if (result != -1)
            {
                char input = (char)result;
                if (input > 0 && input <= 127)
                {
                    Serial.write(input);
                    cmd += (char)input;
                }
            }
        }

        char endChar = cmd.charAt(cmd.length() - 1);
        // Serial.printf("Cmd is: %s, end is %c\n", cmd.c_str(), endChar);

        if (endChar == '\n' || endChar == '\r')
        {
            auto output = parseCmd(cmd);
            processCmd(output);
            cmd = "";

            Serial.printf("\nBadgePiratesOS F:\\>");
        }
    }
}