#include "oracle.h"
#include "keys.hpp"
#include "wifi.hpp"
#include "ESP8266WiFi.h"

namespace apps
{
    void wifi::setup()
    {
        nc::lcd::clear();
        nc::lcd::print("   Scanning...  ");

        WiFi.mode(WIFI_STA);
        WiFi.disconnect();

        found = WiFi.scanNetworks();
        position = 0;

        if(found == 0)
        {
            ssid = "<NONE>";
        } else {
            ssid = String(position + 1) + ":" + WiFi.SSID(position);
        }
    }

    void wifi::loop()
    {
        nc::lcd::setCursor(0,0);
        nc::lcd::print("Found ");
        nc::lcd::print(String(found));
        nc::lcd::print(" AP(s):  ");

        nc::lcd::setCursor(0,1);
        nc::lcd::print(ssid);

        for(int i = ssid.length(); i < 16; i++)
        {
            nc::lcd::print(".");
        }
    }

    void wifi::setdown()
    {
        WiFi.mode(WIFI_OFF);
    }

    void wifi::on_key_down(int key)
    {
        switch(key)
        {
            case KEY_RIGHT:
            case KEY_DOWN:
                position += 1;
                
                if(position == found)
                    position = 0;

                nc::beep(1);

                ssid = String(position + 1) + ":" + WiFi.SSID(position);
                return;
            case KEY_LEFT:
            case KEY_UP:
                position -= 1;
                
                if(position == -1)
                    position = found - 1;

                nc::beep(1);

                ssid = String(position + 1) + ":" + WiFi.SSID(position);
                return;
        }

        nc::beep(3);
    }
} // namespace apps
