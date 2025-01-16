#pragma once
#include "app.hpp"

namespace apps
{
    class menu : public app
    {
    public:
        menu();

        void setup();
        void loop();
        void setdown();
        void on_key_down(int key);

    private:
        int position = 0;

        app* options[4];
    };

} // namespace apps
