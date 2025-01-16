#include "oracle.h"
#include "menu.hpp"
#include "keys.hpp"
#include "settings.hpp"

namespace apps
{
    void settings::setup()
    {

    }

    void settings::loop()
    {
        nc::lcd::setCursor(0,0);
        nc::lcd::print("Battery:        ");

        nc::lcd::setCursor(0,1);

        auto vstring = String(nc::percentage_bat);
        auto mstring = String(nc::measured_bat);

        nc::lcd::print(vstring);
        nc::lcd::print("% ");
        nc::lcd::print(mstring);
        nc::lcd::print("V          ");
    }

    void settings::setdown()
    {

    }

    void settings::on_key_down(int key)
    {
        // switch(key)
        // {
        // }

        nc::beep(3);
    }
} // namespace apps
