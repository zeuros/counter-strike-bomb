
#include <LiquidCrystal.h>

#include "messages.h"

Messages::Messages(LiquidCrystal& lcd)
{
    this->lcd = &lcd;
}

void Messages::bomb_will_boom_in(unsigned int bomb_duration_preset)
{
    lcd->clear();
    lcd->setCursor(0, 0);
    lcd->print("La bombe petera");
    lcd->setCursor(3, 1);
    lcd->print("dans " + String(bomb_duration_preset) + "s");
}

void Messages::print(String s0, String s1)
{
    lcd->setCursor(0, 0);
    lcd->print(s0);
    lcd->setCursor(0, 1);
    lcd->print(s1);
}

String Messages::fill(char character, int count)
{
    char ret[16];

    for( int i = 0 ; i < 16 ; i++)
        ret[i] = i < count ? character : ' ';

    return String(ret);
}


