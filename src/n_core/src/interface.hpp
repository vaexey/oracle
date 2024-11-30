#ifndef N_CORE_INTERFACE
#define N_CORE_INTERFACE

#include <LiquidCrystal_I2C.h>

namespace interface
{
    extern LiquidCrystal_I2C lcd;

    void setup();
    void loop();

    void keypad_roll();
    void pwm_roll();
    void beep_roll();

    void enable_PCINT();
    void disable_PCINT();

    void on_keypad();

    void beep(int type);

    void set_rgb(uint8_t r, uint8_t g, uint8_t b);

    void discard_keypad_events();
}

#endif