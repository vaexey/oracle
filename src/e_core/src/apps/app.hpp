#pragma once

namespace apps
{
    class app
    {
    public:
        virtual void setup();
        virtual void loop();
        virtual void setdown();

        virtual void on_key_down(int key);
    };
    
} // namespace apps
