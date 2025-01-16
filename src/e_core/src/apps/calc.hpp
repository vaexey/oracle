#pragma once
#include "app.hpp"
#include "WString.h"

namespace apps
{
    class calc : public app
    {
    public:
        void setup();
        void loop();
        void setdown();
        void on_key_down(int key);

    private:
        void on_number(int number);
        void on_char(char chr);
        void on_calc();

        String expr;
    };

} // namespace apps
