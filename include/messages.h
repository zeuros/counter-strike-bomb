#include <LiquidCrystal.h>


class Messages
{
    public: 
        LiquidCrystal* lcd;

        Messages(LiquidCrystal& lcd);

        void bomb_setup_time(unsigned int bomb_duration_preset);
        void bomb_ready_to_start(unsigned int bomb_duration_preset);
        void print(String s0, String s1 = String("                "));
        String fill(char character, int count);
};