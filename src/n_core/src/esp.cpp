#include <avr/wdt.h>
#include "esp.hpp"
#include "interface.hpp"
#include "event.hpp"
#include "macros.hpp"
#include "sense.hpp"

namespace esp
{
    const size_t MAX_ESP_BUFFER = 256;

    const char* ESP_OK_MESSAGE = "E_CORE_BOOT_V1";

    uint8_t esp_buffer[MAX_ESP_BUFFER+1];
    uint8_t esp_buffer_len = 0;
    bool esp_available = false;

    unsigned long enable_esp_time = 0;
    bool enabled = false;
    bool connected = false;

    // Time of reset on special press
    unsigned long reset_on = 0;

    void setup()
    {
        // ESP enable pin
        PORTC |= (1 << 7);
        DDRC |= (1 << 7);

        enable_esp_time = millis() + 1000;
    }

    void loop()
    {
        capture_serial();

        if(!enabled)
        {
            interface::discard_keypad_events();
            
            if(millis() > enable_esp_time)
            {
                enabled = true;

                PORTC &= ~(1 << 7);

                Serial.println("Enabled ESP");

                interface::set_rgb(255, 200, 0);

                interface::lcd.setCursor(0,0);
                interface::lcd.print("   Loading...   ");

                wdt_disable();
                wdt_enable(WDTO_1S);
            }

            return;
        }

        if(!connected)
        {
            interface::discard_keypad_events();

            #ifndef NO_ESP
            while(esp_available)
            {
                if(!strcmp((char*)esp_buffer, ESP_OK_MESSAGE))
                {
                    connected = true;
                    wdt_disable();

                    interface::set_rgb(255, 255, 255);
                    interface::lcd.clear();

                    break;
                }

                esp_available = false;
                esp_buffer_len = 0;

                capture_serial();
            }
            #else                    
            connected = true;
            wdt_disable();

            interface::set_rgb(255, 255, 255);
            interface::lcd.clear();
            #endif
        }

        // Execute serial commands

        while(esp_available)
        {
            uint16_t s_v, s_a, s_bat;
            bool pullup;

            switch(esp_buffer[0])
            {
                case 'C': // Clear LCD
                    interface::lcd.clear();
                    break;
                case 'X': // Set LCD cursor
                    interface::lcd.setCursor(
                        asltoi(esp_buffer, 1), 
                        asltoi(esp_buffer, 2)
                    );
                    break;
                case 'L': // Write to LCD
                    for(int i = 1; i < esp_buffer_len; i++)
                    {
                        interface::lcd.write(esp_buffer[i]);
                    }
                    break;
                case 'R': // Set LCD color
                    interface::set_rgb(
                        asltoi2(esp_buffer, 1),
                        asltoi2(esp_buffer, 3),
                        asltoi2(esp_buffer, 5)
                    );
                    break;
                case 'B': // Beep
                    interface::beep(asltoi(esp_buffer, 1));
                    break;
                case 'M': // Read values
                    s_v = sense::measure_v();
                    s_a = sense::measure_a();
                    s_bat = sense::measure_bat();

                    ESP_SERIAL.write('M');
                    ESP_SERIAL.write(itosl16_0(s_v));
                    ESP_SERIAL.write(itosl16_1(s_v));
                    ESP_SERIAL.write(itosl16_2(s_v));
                    ESP_SERIAL.write(itosl16_3(s_v));
                    ESP_SERIAL.write(itosl16_0(s_a));
                    ESP_SERIAL.write(itosl16_1(s_a));
                    ESP_SERIAL.write(itosl16_2(s_a));
                    ESP_SERIAL.write(itosl16_3(s_a));
                    ESP_SERIAL.write(itosl16_0(s_bat));
                    ESP_SERIAL.write(itosl16_1(s_bat));
                    ESP_SERIAL.write(itosl16_2(s_bat));
                    ESP_SERIAL.write(itosl16_3(s_bat));
                    ESP_SERIAL.println();
                    break;
                case 'P': // Pullup
                    pullup = asltoi(esp_buffer, 1) != 0;
                    sense::set_pullup(pullup);
                    break;
            }

            esp_available = false;
            esp_buffer_len = 0;

            capture_serial();
        }

        // Send events

        while(1)
        {
            auto evt = event::popEvent();

            if(!evt.valid)
                break;

            ESP_SERIAL.write('E');
            ESP_SERIAL.write(itosl(evt.type));
            ESP_SERIAL.write(itosl(evt.arg));
            ESP_SERIAL.println();
        }

        // auto evt = event::popEvent();
        // if(evt.valid)
        // {
        //     Serial.print("Event: ");
        //     Serial.print(evt.type);
        //     Serial.print("::");
        //     Serial.println(evt.arg);

        //     // if(evt.type == 3)
        //     // {
        //     //     if(evt.arg == 19)
        //     //     {

        //     //     }else{
        //     //         char chr = evt.arg + 'a';
        //     //     }

        //     //     interface::beep(evt.arg - 4);
        //     // }
        // }
    }

    void capture_serial()
    {
        if(esp_available)
        {
            return;
        }

        while(ESP_SERIAL.available())
        {
            auto byte = ESP_SERIAL.read();

            if(byte == '\n')
            {
                if(esp_buffer[esp_buffer_len-1] == '\r')
                {
                    // End of transmission
                    esp_available = true;
                    esp_buffer_len--;

                    esp_buffer[esp_buffer_len] = 0;

                    // Serial.print("RCV: ");
                    // Serial.write(esp_buffer, esp_buffer_len);

                    return;
                }
            }

            esp_buffer[esp_buffer_len] = byte;
            esp_buffer_len++;

            if(esp_buffer_len == MAX_ESP_BUFFER)
            {
                // Discard buffer
                esp_buffer_len = 0;
            }
        }
    }

} // namespace esp