#include <Arduino.h>

#include <LiquidCrystal.h>

#include <Button.h>

#include "../include/buttonPresses.h"
#include "../include/beep.h"
#include "../include/messages.h"

/******** some constants (to move) ********/
#define SETUP 0
#define BOMB_STARTED 1
#define DEFUSING 2

/******** pins definitions ********/
#define ADD_TIME_BUTTON_PIN 6
#define SUB_TIME_BUTTON_PIN 7

/******** LCD setup ********/
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
Messages messages(lcd);

/******** Buttons setup: Connect your button between pins and GND ********/
Button addTimeBtn(ADD_TIME_BUTTON_PIN, DEBOUNCE_TIME); // Add time to bomb
Button subTime(SUB_TIME_BUTTON_PIN, DEBOUNCE_TIME);    // Substract time to bomb delay
unsigned long addTimePressStart = 0;
unsigned long subTimePressStart = 0;
bool button1Press = false;
bool button2Press = false;

/******** bomb setup ********/
int mode = SETUP;
unsigned long defusing_time = 0;
unsigned long time_elapsed_since_countdown_started = 0;

/******** timers setup ********/
unsigned int bomb_duration_preset = 45;

unsigned long bomb_arm_time = 0;
unsigned long bomb_explodes_at = 0;
unsigned long next_beep_at = 0;

void armBomb(int time_s) {
    bomb_arm_time = millis();
    bomb_explodes_at = bomb_arm_time + (unsigned long)(time_s * 1000);
    next_beep_at = 0;

    mode = SETUP;
    messages.bomb_will_boom_in(time_s);
}

void setup()
{
    addTimeBtn.begin();
    subTime.begin();

    messages.welcome();

    delay(1000);

    armBomb(bomb_duration_preset);
}

bool both_buttons_pressed() { return manageButtonPressAndLongPress(addTimeBtn, addTimePressStart) 
    && manageButtonPressAndLongPress(subTime, subTimePressStart); }

void setupMode() {
    button1Press = manageButtonPressAndLongPress(addTimeBtn, addTimePressStart);
    button2Press = manageButtonPressAndLongPress(subTime, subTimePressStart);

    if (button1Press || button2Press)
    {
        if (button1Press)
        {
            bomb_duration_preset += 5;
        }

        if (button2Press)
        {
            bomb_duration_preset -= 5;
        }

        messages.bomb_will_boom_in(bomb_duration_preset);
        delay(100);
    }

    if (button1Press && button2Press) // switch to next mode
    {
        messages.hold_buttons_to_defuse();
        mode = BOMB_STARTED;
    }
}

void bombMode(){
    if (both_buttons_pressed())// defusing
    {
        defusing_time++;
        tone(SPEAKERS_PIN, 300, 120);
        delay(500);
    }
    else if (time_elapsed_since_countdown_started > bomb_duration_preset * 1000) // boom
    {
        for( int i = 0 ; i < 10 ; i++) {// beep every 90ms 10 times
            tone(SPEAKERS_PIN, 4023, 25);
            delay(65);
        }
        lcd.setCursor(0,0);
        lcd.print("****************");
        lcd.setCursor(0,1);
        lcd.print("****************");
        for( int i = 0 ; i < 10000 ; i++) {// white noise boom sound
            tone(SPEAKERS_PIN, random(0, 300), 1);
            delay(1);
        }
        armBomb(bomb_duration_preset);
    }
    else
    {
        // accelerating beep
        time_elapsed_since_countdown_started = millis() - bomb_arm_time;
        accelerating_beep((float)time_elapsed_since_countdown_started / (float)(bomb_duration_preset * 1000), next_beep_at);
        defusing_time = 0;
    }

    
}

void loop() {

    if ( mode == SETUP ) {
        // time choose
        setupMode();
    } else if ( mode == BOMB_STARTED ) {
        // accelerating beep, possibility to stop holding side butts
        bombMode();
    }
        
}
