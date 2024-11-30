#include <Arduino.h>
#include "event.hpp"

namespace event
{
    const int MAX_EVENT_COUNT = 16;
    
    // Events stack
    uint8_t events[MAX_EVENT_COUNT];
    uint8_t* eventPtr = events;

    const uint8_t* eventPtrStart = events;
    const uint8_t* eventPtrEnd = events + MAX_EVENT_COUNT;
    
    void push(uint8_t event)
    {
        // Add event to pool
        if(eventPtr != eventPtrEnd)
        {
            *eventPtr = event;

            eventPtr++;

            // // if((event >> 5) == 3)
            // if(1)
            // {
            //     Serial.print((event & 0b11111));
            //     Serial.print(" OK ");
            //     Serial.println((event >> 5));
            // }

            // eventPtr--;
        }
    }

    uint8_t pop()
    {
        if(eventPtr == eventPtrStart)
        {
            return 0;
        }

        eventPtr--;

        return *eventPtr;
    }

    uint8_t peek()
    {
        if(eventPtr == eventPtrStart)
        {
            return 0;
        }

        return *(eventPtr - 1);
    }

    bool available()
    {
        return peek() != 0;
    }

    void unpack(uint8_t event, evdat* data)
    {
        if(event == 0)
        {
            data->valid = 0;
            return;
        }

        data->type = event >> 5;
        data->arg = event & (0b11111);
        data->valid = 1;
    }

    evdat popEvent()
    {
        evdat evt;

        unpack(pop(), &evt);

        return evt;
    }
} // namespace event
