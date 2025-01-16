#include "keys.hpp"
#include "main.hpp"
#include "oracle.h"

namespace apps
{
    namespace main
    {
        menu app_menu;
        meter app_meter;
        calc app_calc;
        wifi app_wifi;
        settings app_settings;
        charging app_charging;

        app* current = nullptr;

        bool last_charging = false;

        void setup()
        {
            set_app(&app_menu);
        }

        void loop()
        {
            if(last_charging != nc::charging_bat)
            {
                last_charging = nc::charging_bat;

                if(last_charging)
                {
                    // Started charging
                    
                    set_app(&app_charging);
                } else {
                    if(current == &app_charging)
                    {
                        set_app(&app_menu);
                    }
                }
            }

            current->loop();
        }
        
        void set_app(app* app)
        {
            if(current != nullptr)
            {
                current->setdown();
            }

            current = app;

            current->setup();
        }

        void on_key_down(int key)
        {
            if(key == KEY_HOME)
            {
                nc::beep(2);
                set_app(&app_menu);

                return;
            }

            if(key == KEY_SETTINGS)
            {
                nc::beep(2);
                set_app(&app_settings);

                return;
            }

            current->on_key_down(key);
        }

    } // namespace main
    
} // namespace apps