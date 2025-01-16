#include <Arduino.h>
#include <avr/wdt.h>
#include "interface.hpp"
#include "event.hpp"
#include "macros.hpp"
#include "esp.hpp"

namespace interface
{
    const uint8_t DEFAULT_ROW_STATE = 0xFF << 5;

    // Last saved states of rows (defaults to all UP)
    volatile uint8_t rows[4] = { DEFAULT_ROW_STATE, DEFAULT_ROW_STATE, DEFAULT_ROW_STATE, DEFAULT_ROW_STATE };

    // Pointer to current selected row
    volatile uint8_t* rowPtr = rows;

    const volatile uint8_t* rowPtrEnd = rows + 4;

    // Current selected row offset
    uint8_t rowOffset = 0;

    // Counter for SoftPWM
    uint8_t pwm_counter = 0;

    // Beep states
    uint8_t beep_state = 0;
    int beep_freq = 2000;
    // 0 - no beep
    unsigned long beep_time = 0;
    int beep_interval = 0;

    // PWM values
    uint8_t pwm_r = 0;
    uint8_t pwm_g = 0;
    uint8_t pwm_b = 0;
    uint8_t pwm_s = 0;

    // LCD
    LiquidCrystal_I2C lcd(0x27, 16, 2);

    byte loadingChar0[] = {
        B11111,
        B10000,
        B10111,
        B10100,
        B10100,
        B10111,
        B10000,
        B11111
    };

    byte loadingChar1[] = {
        B11111,
        B00001,
        B11101,
        B00101,
        B00101,
        B11101,
        B00001,
        B11111
    };

    void setup()
    {
        // RGB LED SETUP
        DDRB |= 0b111;
        PORTB |= 0b111;

        // LCD SETUP
        lcd.init();

        lcd.createChar(0, loadingChar0);
        lcd.createChar(1, loadingChar1);
        
        lcd.setCursor(0, 0);
        lcd.print("     oracle     ");

        lcd.setCursor(0, 1);
        lcd.print("       ");
        lcd.write(0);
        lcd.write(1);
        lcd.print("       ");

        // KEYPAD SETUP

        // Set port directions
        // Keypad horizontal (input) is on port B
        // Keypad vertical (output) is on port D

        DDRB &= 0b00000111;
        DDRD |= 0b11110000;

        // Set input pullups and clear output (high state) + RGB value to 111
        PORTB |= 0b11111000;
        PORTD |= 0b11110000;

        // Setup keypad interrupts

        // Mask out nonrelated pins
        PCMSK0 = 0b11111000;

        // Enable
        enable_PCINT();

        // BUZZER SETUP
        pinMode(5, OUTPUT);
        // Play welcome tone
        // tone(5, 1959, 100);
        beep(1);

        // STATUS LED SETUP
        pinMode(A2, OUTPUT);
        digitalWrite(A2, HIGH);

        set_rgb(0, 255, 0);

        // Setup TIMER1 for SoftPWM interrupts
        TCNT1 = 63974;

        TCCR1A = 0; // No output
        TCCR1B = bit(CS10); // Prescaler 1x
        TIMSK1 = (1 << TOIE1); // Interrupt

        sei();
    }

    void loop()
    {
        // keypad_roll();

        // pwm_r++;
        // pwm_g = pwm_r;
        // pwm_b = pwm_r;

        // while(1)
        // {
        //     for(int i = 0; i < 255; i++)
        //     {
        //         pwm_g++; delay(10);
        //     }
        //     for(int i = 0; i < 255; i++)
        //     {
        //         pwm_r--; delay(10);
        //     }
        //     for(int i = 0; i < 255; i++)
        //     {
        //         pwm_b++; delay(10);
        //     }
        //     for(int i = 0; i < 255; i++)
        //     {
        //         pwm_g--; delay(10);
        //     }
        //     for(int i = 0; i < 255; i++)
        //     {
        //         pwm_r++; delay(10);
        //     }
        //     for(int i = 0; i < 255; i++)
        //     {
        //         pwm_b--; delay(10);
        //     }
        // }
    }

    ISR(TIMER1_OVF_vect)
    {
        TCNT1 = 65536 - 512;
        
        pwm_roll();
    }

    void pwm_roll()
    {
        pwm_counter++;

        if(pwm_counter == 0)
        {
            PORTB &= ~(0b111);

            keypad_roll();
            beep_roll();
        }

        if(pwm_counter == pwm_r)
        {
            PORTB |= (0b001);
        }
        if(pwm_counter == pwm_g)
        {
            PORTB |= (0b010);
        }
        if(pwm_counter == pwm_b)
        {
            PORTB |= (0b100);
        }
    }

    void enable_PCINT()
    {
        PCICR |= 0b1;
    }

    void disable_PCINT()
    {
        PCICR &= ~0b1;
    }

    void keypad_roll()
    {
        // ignoreInt = true;
        disable_PCINT();

        // Roll to next row
        rowOffset++;
        rowPtr++;

        // Reset on row 4
        if(rowPtr == rowPtrEnd)
        {
            rowPtr = rows;
            rowOffset = 0;
        }

        // Roll row
        PORTD = (
            (PORTD | 0b11110000) & ~(0b10000 << rowOffset)
        );

        on_keypad();

        enable_PCINT();
    }
    
    // Keypad interrupt routine wrapper
    ISR(PCINT0_vect)
    {
        on_keypad();
    }

    void on_keypad()
    {
        // Load current row state
        uint8_t buttons = (~PINB) >> 3;

        // Load previous row state
        uint8_t previous = *rowPtr;

        if(buttons != previous)
        {
            // Check triggered

        // Serial.println(rows[0]);
            // Serial.print(previous);
            // Serial.print(" -> ");
            // Serial.print(buttons);
            // Serial.print(" on ");
            // Serial.print(rowOffset);
            // Serial.println(" check");

            // Save row status
            *rowPtr = buttons;

            // Difference
            uint8_t diff = buttons ^ previous;

            // Check btn 0 (row 0)
            if(diff & (1 << 0))
            {
                uint8_t value = (buttons >> 0) & 1;

                event::push(EVENT_KP_OF(rowOffset + 0 * 4, value));
            }

            // Check btn 1 (row 3)
            if(diff & (1 << 1))
            {
                uint8_t value = (buttons >> 1) & 1;

                event::push(EVENT_KP_OF(rowOffset + 3 * 4, value));
            }

            // Check btn 2 (row 2)
            if(diff & (1 << 2))
            {
                uint8_t value = (buttons >> 2) & 1;

                event::push(EVENT_KP_OF(rowOffset + 2 * 4, value));
            }

            // Check btn 3 (row 1)
            if(diff & (1 << 3))
            {
                uint8_t value = (buttons >> 3) & 1;

                event::push(EVENT_KP_OF(rowOffset + 1 * 4, value));
            }

            // Check btn 4 (row 4)
            if(diff & (1 << 4))
            {
                uint8_t value = (buttons >> 4) & 1;

                event::push(EVENT_KP_OF(rowOffset + 4 * 4, value));

                if(rowOffset + 4 * 4 == 19) // Special reset button
                {
                    esp::onProgrammingButton(value);
                }
            }
        }
    }

    void beep_roll()
    {
        if(beep_state != 0)
        {
            if(millis() > beep_time)
            {
                beep_state--;

                beep_time = millis() + 2 * beep_interval;
                
                tone(5, beep_freq, beep_interval);
            }
        }
    }

    void queueBeep(int count, int freq, int interval)
    {
        beep_freq = freq;
        beep_interval = interval;

        beep_state = count;
    }

    void beep(int type)
    {
       // 0 - default
       // 1 - info
       // 2 - warn
       // 3 - error
       // 4 - error (critical)

        if(type == 0)
            type = 1;

        if(type == 1)
        {
            queueBeep(1, 1959, 100);
            return;
        }

        if(type == 2)
        {
            queueBeep(2, 1959, 60);
            return;
        }

        if(type == 3)
        {
            queueBeep(5, 979, 30);
            return;
        }

        if(type == 4)
        {
            queueBeep(1, 979, 500);
            return;
        }

        if(type == 10)
        {
            queueBeep(10, 2938, 15);
            return;
        }
    }

    void set_rgb(uint8_t r, uint8_t g, uint8_t b)
    {
        pwm_r = r;
        pwm_g = g;
        pwm_b = b;
    }

    void discard_keypad_events()
    {
        event::evdat evt;

        while(1)
        {
            evt = event::popEvent();

            if(!evt.valid)
            {
                break;
            }

            if(evt.type == EVENT_TYPE_KP_DOWN)
            {
                interface::beep(2);
            }
        }
    }
}
