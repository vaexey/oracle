#include "oracle.h"
#include "menu.hpp"
#include "keys.hpp"
#include "charging.hpp"
#include "main.hpp"

namespace apps
{
    void charging::setup()
    {
        nc::lcd::rgb(0, 255, 0);
    }

    void charging::loop()
    {
        int animation = (millis() % 1500L) / 500L;

        nc::lcd::setCursor(0,0);
        nc::lcd::print("   Charging...  ");
        nc::lcd::setCursor(0,1);

        switch (animation)
        {
            case 0:
                nc::lcd::print("    <> [] []    ");
                break;
            case 1:
                nc::lcd::print("    [] <> []    ");
                break;
            case 2:
                nc::lcd::print("    [] [] <>    ");
                break;
        }
    }

    void charging::setdown()
    {
        nc::lcd::rgb(255, 255, 255);
    }

    void charging::on_key_down(int key)
    {
        nc::beep(3);

        main::set_app(&main::app_menu);
    }
} // namespace apps
