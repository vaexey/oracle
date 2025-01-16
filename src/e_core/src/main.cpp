#include <Arduino.h>
#include <math.h>
#include "oracle.h"
#include "apps/main.hpp"
#include "macros.hpp"

void on_key_down(int type, int value)
{
    apps::main::on_key_down(value);
}

void on_key_up(int type, int value)
{

}

void setup() {
    nc::boot();
    nc::attach(EVENT_TYPE_KP_UP, on_key_up);
    nc::attach(EVENT_TYPE_KP_DOWN, on_key_down);

    nc::set_measure_pullup(false);

    nc::lcd::clear();
    nc::lcd::rgb(255, 255, 255);

    apps::main::setup();
}

void loop() {
    nc::send_measure();
    nc::query_events();

    apps::main::loop();

    delay(100);


    // nc::lcd::clear();
    // nc::lcd::print(String(nc::measured_v));
    // nc::lcd::print("  ");

    // nc::lcd::setCursor(0,1);
    // nc::lcd::print(String(nc::measured_a));
    // nc::lcd::print("  ");

    // nc::lcd::setCursor(12, 0);
    // nc::lcd::print(String(nc::percentage_bat));

    // delay(500);
}