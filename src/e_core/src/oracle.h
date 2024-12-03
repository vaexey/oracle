#ifndef E_CORE_NCORE
#define E_CORE_NCORE

#include <Arduino.h>

#define NC_PRINTLN(command) Serial.println(command);
#define NC_PRINT(command) Serial.print(command);

namespace nc
{
    typedef void (*event_cb)(int, int);

    struct event_hook
    {
        int type_mask;
        event_cb callback;
    };

    extern float measured_v;
    extern float measured_a;
    extern float measured_bat;

    void boot();
    void beep(int beepType);
    void send_measure();
    void set_measure_pullup(bool pullup);
    void capture_serial();
    void query_events();

    void attach(event_hook hook);
    void attach(int type_mask, event_cb callback);
    void on(int event_type, event_cb callback);
    void on(int event_type1, int event_type2, event_cb callback);

    void emit(int type, int arg);

    namespace lcd
    {
        void clear();
        void setCursor(int x, int y);
        void rgb(int r, int g, int b);

        void print(const char* value);
        void print(char* value);
        void print(const String value);
    }
}

#endif