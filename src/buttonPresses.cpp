#include <Arduino.h>
#include <Button.h>
#include "../include/buttonPresses.h"

bool manageButtonPressAndLongPress(Button &button, unsigned long &timePressStart)
{

    // Will return true once on single press
    if ( button.pressed() ) 
    {
        timePressStart = millis();
        return true;
    }

    if ( button.read() == Button::RELEASED )
        timePressStart = 0;

    // will return true continuously once LONG_PRESS_DELAY passed pressing button
    if ( timePressStart > 0 
        && button.read() == Button::PRESSED
        && (millis() - timePressStart) > LONG_PRESS_DELAY )
    {
        return true;
    }

    return false;
}
