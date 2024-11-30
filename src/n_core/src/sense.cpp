#include "sense.hpp"

namespace sense
{
    // Pins:
    const uint8_t PIN_BAT = A5;
    const uint8_t PIN_V = A4;
    const uint8_t PIN_A = A3;

    const uint8_t PIN_PULLUP = A0;

    void setup()
    {
        pinMode(PIN_BAT, INPUT);
        pinMode(PIN_V, INPUT);
        pinMode(PIN_A, INPUT);
        analogReference(INTERNAL);

        pinMode(PIN_PULLUP, OUTPUT);

        digitalWrite(PIN_PULLUP, HIGH);
    }

    int measure_multiple(uint8_t pin, uint8_t iterations)
    {
        int total = 0; 

        for(uint8_t i = 0; i < iterations; i++)
        {
            total += analogRead(pin);
        }

        return total;
    }

    int measure_v()
    {
        // U - real voltage
        // Ud - voltage after voltage divider
        // Ref - analog reference
        // A - analog read result
        // A = (1024*Ud/Ref)
        // A = (1024*0.164*U/Ref)
        // A = (65.5*U)
        // U = A/65.5
        // Transpose to 1000x
        // U = A*15.267

        int avg = measure_multiple(PIN_V, 10);

        int mapped = 152*avg/1000;

        return mapped;
    }

    int measure_a()
    {
        return measure_multiple(PIN_A, 10);
    }

    int measure_bat()
    {
        return measure_multiple(PIN_BAT, 10);
    }
} // namespace sense
