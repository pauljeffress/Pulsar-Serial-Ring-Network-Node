/*
 * setupPins() 
 * 
 * Set mode and/or state of all GPIOs.
 * 
 */

#include "global_vars.h"

void setupPins(){
    // set mode of all GPIOs
    pinMode(LEFTNOTIFYPIN, OUTPUT);
    pinMode(LEFTLISTENPIN, INPUT);
    pinMode(HOSTNOTIFYPIN, OUTPUT);
    pinMode(HOSTLISTENPIN, INPUT);
    pinMode(RIGHTNOTIFYPIN, OUTPUT);
    pinMode(RIGHTLISTENPIN, INPUT);    

    // set initial state of all OUTPUT GPIOs
    digitalWrite(LEFTNOTIFYPIN, LOW);
    digitalWrite(HOSTNOTIFYPIN, LOW);
    digitalWrite(RIGHTNOTIFYPIN, LOW);
}