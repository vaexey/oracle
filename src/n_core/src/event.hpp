#ifndef N_CORE_EVENT
#define N_CORE_EVENT

#include <Arduino.h>

namespace event
{
    struct evdat
    {
        uint8_t type;
        uint8_t arg;
        uint8_t valid;
    };
    
    
    void push(uint8_t event);
    uint8_t pop();
    uint8_t peek();
    bool available();
    void unpack(uint8_t event, evdat* data);
    evdat popEvent();

} // namespace event

#endif