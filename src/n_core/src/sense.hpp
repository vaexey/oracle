#ifndef N_CORE_SENSE
#define N_CORE_SENSE

#include <Arduino.h>

namespace sense
{
    void setup();
    int measure_bat();
    int measure_v();
    int measure_a();

    // void measure();
} // namespace sense

#endif