#ifndef N_CORE_SENSE
#define N_CORE_SENSE

#include <Arduino.h>

namespace sense
{
    void setup();
    void set_pullup(bool pullup);
    uint16_t measure_bat();
    uint16_t measure_v();
    uint16_t measure_a();

    // void measure();
} // namespace sense

#endif