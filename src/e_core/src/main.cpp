#include <Arduino.h>
#include <math.h>
#include "oracle.h"

void keypad(int type, int arg);
void redraw();

void setup() {

    nc::boot();

    // Serial.println("!");
    // Serial.println("E_CORE_BOOT_V1");
    // Serial.println("C");
    // Serial.println("L   I am alive   ");
    // Serial.println("Xab");
    // Serial.println("L:DD             ");

    // pinMode(LED_BUILTIN, OUTPUT);

    nc::on(3, keypad);
    redraw();

}

unsigned long numA = 0;
unsigned long numB = 0;
unsigned long result = 0;

int stage = 0;

void loop() {
    nc::query_events();

    if(stage == 3)
    {
        nc::send_measure();
        redraw();

        delay(500);
    }
}

void redraw()
{
    if(stage == 3)
    {
        float val = roundf(0.00274647887f * 100.0f * nc::measured_bat) / 100.0f;

        nc::lcd::rgb(255, 255, 255);
        nc::lcd::setCursor(0,0);
        nc::lcd::print(String(val));
        nc::lcd::print("  ");

        return;
    }

    if(stage == 0)
    {
        nc::lcd::rgb(255, 0, 0);
    }
    if(stage == 1)
    {
        nc::lcd::rgb(0, 255, 0);
    }
    if(stage == 2)
    {
        nc::lcd::rgb(0, 0, 255);
    }

    nc::lcd::clear();

    if(stage == 2)
    {
        nc::lcd::print("=");
        nc::lcd::print(String(result));
        return;
    }
    
    if(stage == 0)
    {
        nc::lcd::print(">");
    } else {
        nc::lcd::print(" ");
    }

    nc::lcd::print(String(numA));
    
    if(stage < 1)
        return;

    nc::lcd::setCursor(0,1);
    
    if(stage == 1)
    {
        nc::lcd::print(">");
    } else {
        nc::lcd::print(" ");
    }
    
    nc::lcd::print(String(numB));

}

void press_digit(int dig)
{
    if(stage == 0)
    {
        numA = numA * 10 + dig;
    }

    if(stage == 1)
    {
        numB = numB * 10 + dig;
    }

    redraw();
}

void keypad(int type, int arg)
{
    nc::beep(1);



    if(arg == 7)
    {
        if(stage == 3){
            stage = 0;
        } else {
            stage = 3;
        }
        redraw();
        
        return;
    }

    if(stage == 2 || arg == 11 || stage == 3)
    {
        stage = 0;
        numA = 0;
        numB = 0;

        redraw();
        return;
    }

    if(arg == 0) { press_digit(1); return; }
    if(arg == 1) { press_digit(2); return; }
    if(arg == 2) { press_digit(3); return; }
    if(arg == 4) { press_digit(4); return; }
    if(arg == 5) { press_digit(5); return; }
    if(arg == 6) { press_digit(6); return; }
    if(arg == 8) { press_digit(7); return; }
    if(arg == 9) { press_digit(8); return; }
    if(arg == 10) { press_digit(9); return; }
    if(arg == 12) { press_digit(0); return; }

    if(arg == 15)
    {
        stage = 1;
    }

    if(arg == 19)
    {
        stage = 2;

        result = numA + numB;
    }

    redraw();
}