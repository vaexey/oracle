#pragma once
#include "app.hpp"
#include "WString.h"

namespace apps
{
    class wifi : public app
    {
    public:
        void setup();
        void loop();
        void setdown();
        void on_key_down(int key);

    private:
        int found;
        int position;
        String ssid;
    };

} // namespace apps
