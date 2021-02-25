#include <LiquidCrystal.h>


class Messages
{
    public: 
        LiquidCrystal* lcd;

        Messages(LiquidCrystal& lcd);

        void welcome();
        void bomb_will_boom_in(unsigned int bomb_duration_preset);
        void hold_buttons_to_defuse();
};