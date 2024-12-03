#ifndef N_CORE_ESP
#define N_CORE_ESP

#include <Arduino.h>

/**
 * Macro for debug mode that:
 *  - does not require BOOT message (thus no wdt timeout)
 *  - uses USB Serial rather than onboard TX/RX (ESP) Serial
 */
// #define NO_ESP

#ifndef NO_ESP
#define ESP_SERIAL Serial1
#else
#define ESP_SERIAL Serial
#endif


namespace esp
{
    void setup();
    void loop();

    void capture_serial();

} // namespace esp

#endif