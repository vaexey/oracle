#include "oracle.h"
#include "menu.hpp"
#include "keys.hpp"
#include "meter.hpp"

namespace apps
{
    void meter::setup()
    {
        pullup = false;

        nc::set_measure_pullup(false);
    }

    unsigned long next_ding = 0;

    void meter::loop()
    {
        if(pullup)
        {
            if(nc::measured_v < 0.1)
            {
                // Ding mode
                nc::lcd::rgb(255, 0, 0);

                if(millis() > next_ding)
                {
                    next_ding = millis() + 100;

                    nc::beep(1);
                }
            } else {
                nc::lcd::rgb(255, 255, 255);

                next_ding = 0;
            }
        }

        nc::lcd::setCursor(0,0);

        if(nc::overload_v)
        {
            nc::lcd::print("V: OVERLOAD    ");
        } else {
            nc::lcd::print("V: ");

            auto vstring = String(nc::measured_v);

            if(vstring.length() > 12)
            {
                vstring = vstring.substring(0,12);
            }

            nc::lcd::print(vstring);

            for(int i = vstring.length(); i < 12; i++)
            {
                nc::lcd::print(".");
            }
        }

        if(pullup)
        {
            nc::lcd::print("P");
        } else {
            nc::lcd::print(".");
        }

        nc::lcd::setCursor(0,1);

        if(nc::overload_a)
        {
            nc::lcd::print("A: OVERLOAD     ");
        } else {
            nc::lcd::print("A: ");

            auto vstring = String(nc::measured_a);

            if(vstring.length() > 5)
            {
                vstring = vstring.substring(0,5);
            }

            nc::lcd::print(vstring);

            for(int i = vstring.length(); i < 5; i++)
            {
                nc::lcd::print(".");
            }
        }

        if(nc::overload_v || nc::overload_a)
        {
            nc::lcd::print("W: ?    ");
        } else {
            nc::lcd::print("W: ");

            auto vstring = String(nc::measured_a * nc::measured_v);

            if(vstring.length() > 5)
            {
                vstring = vstring.substring(0,5);
            }

            nc::lcd::print(vstring);

            for(int i = vstring.length(); i < 5; i++)
            {
                nc::lcd::print(".");
            }
        }
    }

    void meter::setdown()
    {
        nc::set_measure_pullup(false);

        nc::lcd::rgb(255, 255, 255);
    }

    void meter::on_key_down(int key)
    {
        switch(key)
        {
            case KEY_ENTER:
                pullup = !pullup;

                nc::set_measure_pullup(pullup);
                nc::beep(1);

                if(!pullup)
                {
                    nc::lcd::rgb(255, 255, 255);
                }

                return;
        }

        nc::beep(3);
    }
} // namespace apps
