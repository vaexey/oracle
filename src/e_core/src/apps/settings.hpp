#pragma once
#include "app.hpp"

namespace apps
{
    class settings : public app
    {
    public:
        void setup();
        void loop();
        void setdown();
        void on_key_down(int key);

    private:
        bool pullup;
    };

} // namespace apps