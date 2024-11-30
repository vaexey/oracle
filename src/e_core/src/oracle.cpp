#include "oracle.h"
#include "macros.hpp"

namespace nc
{
    const size_t MAX_NC_BUFFER = 256;

    uint8_t nc_buffer[MAX_NC_BUFFER+1];
    uint8_t nc_buffer_len = 0;
    bool nc_available = false;

    // std::vector<event_hook> event_hooks(64);
    // std::array<event_hook, 64> event_hooks;
    event_hook event_hooks[64];
    int event_hook_count = 0;

    void boot()
    {
        Serial.begin(19200);

        NC_PRINTLN("");
        NC_PRINTLN("");
        NC_PRINTLN("E_CORE_BOOT_V1");
    }

    void beep(int beepType)
    {
        NC_PRINT("B");
        NC_PRINTLN(itosl(beepType));
    }

    void query_events()
    {
        capture_serial();

        while(nc_available)
        {
            switch(nc_buffer[0])
            {
                case 'E':
                    int e_type = sltoi(nc_buffer[1]);
                    int e_arg = sltoi(nc_buffer[2]);

                    emit(e_type, e_arg);

                    break;
            }
            
            nc_available = false;
            nc_buffer_len = 0;

            capture_serial();
        }
    }

    void attach(event_hook hook)
    {
        // event_hooks.push_back(hook);
        event_hooks[event_hook_count++] = hook;
    }

    void attach(int type_mask, event_cb callback)
    {
        attach(event_hook {
            type_mask,
            callback
        });
    }

    void on(int event_type, event_cb callback)
    {
        attach(
            1 << event_type, 
            callback
        );
    }

    void on(int event_type1, int event_type2, event_cb callback)
    {
        attach(
            (1 << event_type1) |
            (1 << event_type2), 
            callback
        );
    }

    void emit(int type, int arg)
    {
        int typeMask = (1 << type);

        for(int i = 0; i < event_hook_count; i++)
        {
            auto eh = event_hooks[i];

            if((eh.type_mask & typeMask) != 0)
            {
                eh.callback(type, arg);
            }
        }        
    }

    void capture_serial()
    {
        if(nc_available)
        {
            return;
        }

        while(Serial.available())
        {
            auto byte = Serial.read();

            if(byte == '\n')
            {
                if(nc_buffer[nc_buffer_len-1] == '\r')
                {
                    // End of transmission
                    nc_available = true;
                    nc_buffer_len--;

                    nc_buffer[nc_buffer_len] = 0;

                    return;
                }
            }

            nc_buffer[nc_buffer_len] = byte;
            nc_buffer_len++;

            if(nc_buffer_len == MAX_NC_BUFFER)
            {
                // Discard buffer
                nc_buffer_len = 0;
            }
        }
    }

    namespace lcd
    {
        void clear()
        {
            NC_PRINTLN("C");
        }

        void setCursor(int x, int y)
        {
            NC_PRINT("X");
            NC_PRINT(itosl(x));
            NC_PRINTLN(itosl(y));
        }

        void rgb(int r, int g, int b)
        {
            NC_PRINT("R");
            NC_PRINT(itoslh(r));
            NC_PRINT(itosll(r));
            NC_PRINT(itoslh(g));
            NC_PRINT(itosll(g));
            NC_PRINT(itoslh(b));
            NC_PRINTLN(itosll(b));
        }

        void print(const char* value)
        {
            NC_PRINT("L");
            NC_PRINTLN(value);
        }
        void print(char* value)
        {
            NC_PRINT("L");
            NC_PRINTLN(value);
        }
        void print(const String value)
        {
            NC_PRINT("L");
            NC_PRINTLN(value);
        }
    }
}