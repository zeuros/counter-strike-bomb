
#include <LiquidCrystal.h>

#include "messages.h"

Messages::Messages(LiquidCrystal& lcd)
{
    this->lcd = &lcd;
}

void Messages::welcome()
{
    lcd->begin(16, 2);
    lcd->setCursor(0, 0);
    lcd->print("Xx_Bombinator_xX");
    lcd->setCursor(0, 1);
    lcd->print("Reglez la duree");
}

void Messages::bomb_will_boom_in(unsigned int bomb_duration_preset)
{
    lcd->clear();
    lcd->setCursor(0, 0);
    lcd->print("La bombe petera");
    lcd->setCursor(3, 1);
    lcd->print("dans " + String(bomb_duration_preset) + "s");
}

void Messages::hold_buttons_to_defuse()
{
    lcd->clear();
    lcd->setCursor(0, 0);
    lcd->print("***MAINTENEZ****");
    lcd->setCursor(0, 1);
    lcd->print("*POUR DESARMER**");
}


