#ifndef N_CORE_PINS
#define N_CORE_PINS

// Pins on the "oracle" rev. 1 board

// INTERFACE INPUT

// Keypad horizontal
// PB3, PB4, PB5, PB6, PB7
// Keypad vertical
// PD4, PD5, PD6, PD7

// Power button
// PF6

// INTERFACE OUTPUT

// RGB
// PB0, PB1, PB2

// SCL
// PD0
// SDA
// PD1

// Status LED
// PF5

// Buzzer
// PC6

// INTERNAL 

// ESP enable
// PC7

// ADC BAT, V, A
// PF0, PF1, PF4

// V pullup enable
// PF7

void pins_setup();


#endif