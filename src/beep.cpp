#include <Arduino.h>

#include "../include/beep.h"

/**
 * Time between beeps is an exponential function
 * 
 * A0 = 1.04865151217746
 * A1 = 0.244017811416199
 * A2 = 1.76379778668885
 * 
 * Y(bps) = A0 * E^(A1*X + A2*X^2)
 * X = % OF TIME PASSED (0.0 - 1.0)
 * Y = BEEPS PER SECOND
 */
#define BEEP_A0 1.04865151217746
#define BEEP_A1 0.244017811416199
#define BEEP_A2 1.76379778668885

void accelerating_beep(float time_passed_since_bomb_armed_pu, unsigned long &next_beep_at)
{
    if ( millis() > next_beep_at ) {

        tone(SPEAKERS_PIN, 4000, BOMB_BEEP_DURATION);

        next_beep_at = millis() + 1000 * ( 1 / beep_per_second(time_passed_since_bomb_armed_pu) );
    }
}

float beep_per_second(float time_passed_since_bomb_armed_pu) 
{
    return 1.049 * exp(0.244 * time_passed_since_bomb_armed_pu + 1.764 * time_passed_since_bomb_armed_pu * time_passed_since_bomb_armed_pu);
}

void defuse_beep(int beep_every_ms = 500) {
    static unsigned long next_beep_at = 0;

    if ( millis() > next_beep_at ) {
        tone(SPEAKERS_PIN, 2000, BOMB_BEEP_DURATION);
        next_beep_at = millis() + beep_every_ms;
    }
}