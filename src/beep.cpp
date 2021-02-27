#include <Arduino.h>

#include "../include/beep.h"

void bomb_beep()
{
    tone(SPEAKERS_PIN, 4000, BOMB_BEEP_DURATION);
}

void defuse_beep(int beep_every_ms = 500) {
    static unsigned long next_beep_at = 0;

    if ( millis() > next_beep_at ) {
        tone(SPEAKERS_PIN, 2000, BOMB_BEEP_DURATION);
        next_beep_at = millis() + beep_every_ms;
    }
}