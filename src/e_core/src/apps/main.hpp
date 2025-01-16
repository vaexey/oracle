#include "menu.hpp"
#include "meter.hpp"
#include "wifi.hpp"
#include "settings.hpp"
#include "calc.hpp"
#include "charging.hpp"

namespace apps
{
    namespace main
    {
        extern menu app_menu;
        extern meter app_meter;
        extern calc app_calc;
        extern wifi app_wifi;
        extern settings app_settings;
        extern charging app_charging;

        extern app* current;

        void setup();
        void loop();
        
        void set_app(app* app);

        void on_key_down(int key);

    } // namespace main
    
} // namespace apps
