
#include <LiquidCrystal.h>

#include "messages.h"

Messages::Messages(LiquidCrystal& lcd)
{
    this->lcd = &lcd;
}

void Messages::bomb_setup_time(unsigned int bomb_duration_preset)
{
    lcd->clear();
    lcd->setCursor(0, 0);
    lcd->print("Setup time (L+R)");
    lcd->setCursor(0, 1);
    lcd->print("Time set: " + String(bomb_duration_preset) + "s");
}

void Messages::bomb_ready_to_start(unsigned int bomb_duration_preset)
{
    lcd->clear();
    lcd->setCursor(0, 0);
    lcd->print("PRESS L+R => GO");
    lcd->setCursor(0, 1);
    lcd->print("  Duree " + String(bomb_duration_preset) + "s");
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


