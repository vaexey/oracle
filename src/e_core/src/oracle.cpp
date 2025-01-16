#include "oracle.h"
#include "macros.hpp"
#include <math.h>

#define SHIFT_MEASURE_V(value) (0.0015352697f * (float)(value))
#define SHIFT_MEASURE_A(value) (0.000268125f * (float)(value))
// #define SHIFT_MEASURE_A(value) (1.0f * (float)(value))
#define SHIFT_MEASURE_BAT(value) (0.00274647887f * (float)(value))

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


    bool overload_v = true;
    float measured_v = 0;

    bool overload_a = true;
    float measured_a = 0;

    float percentage_bat = 0;
    float measured_bat = 0;
    bool charging_bat = false;

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

    void send_measure()
    {
        NC_PRINTLN("M");
    }

    void set_measure_pullup(bool pullup)
    {
        if(pullup)
        {
            NC_PRINTLN("Pb");
        } else {
            NC_PRINTLN("Pa");
        }
    }

    float bat_to_percentage(float voltage)
    {
        if(voltage > 4.2f)
        {
            return 100.0f;
        } else if(voltage > 3.8f)
        {
            return (-200.0f * voltage + 1745.0f) * voltage - 3701.0f;
        } else if(voltage > 3.6f)
        {
            return (1000.0f * voltage - 7200.0f) * voltage + 12962.0f;
        } else if(voltage > 3.5f)
        {
            return 20.0f * voltage - 70.0f;
        } else {
            return 0.0f;
        }
    }

    void query_events()
    {
        capture_serial();

        while(nc_available)
        {
            int e_type, e_arg, val;

            switch(nc_buffer[0])
            {
                case 'E':
                    e_type = sltoi(nc_buffer[1]);
                    e_arg = sltoi(nc_buffer[2]);

                    emit(e_type, e_arg);

                    break;
                case 'M':
                    val = sltoi16(nc_buffer, 1);
                    overload_v = (val < 0) || (val > 10240);
                    measured_v = SHIFT_MEASURE_V(val);

                    val = sltoi16(nc_buffer, 5);
                    overload_a = (val < 0) || (val > 10240);
                    measured_a = SHIFT_MEASURE_A(val);

                    measured_bat = SHIFT_MEASURE_BAT(sltoi16(nc_buffer, 9));
                    charging_bat = measured_bat > 4.2;
                    percentage_bat = roundf(bat_to_percentage(measured_bat) * 10.0f) / 10.0f;

                    break;
            }
            
            nc_available = false;
            nc_buffer_len = 0;

            capture_serial();
        }
    }

    void attach(event_hook hook)
    {
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

        void blink(bool blink)
        {
            if(blink)
            {
                NC_PRINTLN("Nb");
            } else {
                NC_PRINTLN("Na");
            }
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