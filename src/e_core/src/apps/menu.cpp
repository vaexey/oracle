#include "oracle.h"
#include "menu.hpp"
#include "keys.hpp"
#include "main.hpp"

namespace apps
{
    menu::menu()
    {
        options[0] = &main::app_meter;
        options[1] = &main::app_calc;
        options[2] = &main::app_wifi;
        options[3] = &main::app_settings;
    }

    void menu::setup()
    {
        position = 0;
    }

    void menu::loop()
    {
        switch(position)
        {
            case 0:
                nc::lcd::setCursor(0,0);
                nc::lcd::print("[X]Meter [ ]Calc");
                nc::lcd::setCursor(0,1);
                nc::lcd::print("[ ]WiFi  [ ]Set ");
                break;
            case 1:
                nc::lcd::setCursor(0,0);
                nc::lcd::print("[ ]Meter [X]Calc");
                nc::lcd::setCursor(0,1);
                nc::lcd::print("[ ]WiFi  [ ]Set ");
                break;
            case 2:
                nc::lcd::setCursor(0,0);
                nc::lcd::print("[ ]Meter [ ]Calc");
                nc::lcd::setCursor(0,1);
                nc::lcd::print("[X]WiFi  [ ]Set ");
                break;
            case 3:
                nc::lcd::setCursor(0,0);
                nc::lcd::print("[ ]Meter [ ]Calc");
                nc::lcd::setCursor(0,1);
                nc::lcd::print("[ ]WiFi  [X]Set ");
                break;
        }
    }

    void menu::setdown()
    {

    }

    void menu::on_key_down(int key)
    {
        switch(key)
        {
            case KEY_RIGHT:
            case KEY_DOWN:
                position += 1;
                
                if(position == 4)
                    position = 0;

                nc::beep(1);
                return;
            case KEY_LEFT:
            case KEY_UP:
                position -= 1;
                
                if(position == -1)
                    position = 3;

                nc::beep(1);
                return;
            
            case KEY_ENTER:
                main::set_app(options[position]);

                nc::beep(2);
                return;
        }

        nc::beep(3);
    }
} // namespace apps::menu
