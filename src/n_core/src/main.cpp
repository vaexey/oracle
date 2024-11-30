#include <Arduino.h>
// #include "pins.hpp"
#include "interface.hpp"
#include "event.hpp"
#include "sense.hpp"
#include "esp.hpp"


void setup() {
    // pins_setup();
    // DDRB = 0b111;
    // PORTB = 0b111; // ~BGR

    // lcd.init();
    // lcd.setCursor(0, 0);
    // lcd.print("boot");

    Serial.begin(19200);
    Serial1.begin(19200);

    // while(!Serial);

    Serial.println("witam");
    
    interface::setup();
    sense::setup();
    esp::setup();
}

void loop() {
    // keypad::loop();

    // if(evt.valid)
    // {
    //     Serial.print("Event: ");
    //     Serial.print(evt.type);
    //     Serial.print("::");
    //     Serial.println(evt.arg);

    //     if(evt.type == 3)
    //     {
    //         if(evt.arg == 19)
    //         {
    //             lcd.clear();
    //             lcd.setCursor(0,0);
    //         }else{
    //             char chr = evt.arg + 'a';

    //             lcd.print(chr);
    //         }
    //     }
    // }

    // Serial.print(sense::measure_v());
    // Serial.print("  ");
    // Serial.print(sense::measure_a());
    // Serial.print("  ");
    // Serial.println(sense::measure_bat());

    // delay(300);
    // delay(10);

    // lcd.setCursor(0,0);
    // lcd.print(sense::measure_v());
    // lcd.print("   ");
    // delay(700);

    // PORTF ^= 0b100000;

    // while(Serial1.available())
    // {
    //     Serial.write(Serial1.read());
    // }

    interface::loop();
    esp::loop();
    delay(10);
}