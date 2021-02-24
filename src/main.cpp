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
#define DEFUSAL_KIT_PIN 8

/******** LCD setup ********/
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
Messages messages(lcd);

/******** Buttons setup: Connect your button between pins and GND ********/
Button addTimeBtn(ADD_TIME_BUTTON_PIN, DEBOUNCE_TIME); // Add time to bomb
Button subTime(SUB_TIME_BUTTON_PIN, DEBOUNCE_TIME);    // Substract time to bomb delay
Button defusalKit(DEFUSAL_KIT_PIN, DEBOUNCE_TIME);        // Substract time to bomb delay
unsigned long addTimePressStart = 0;
unsigned long subTimePressStart = 0;
bool leftButtonPress = false;
bool rightButtonPress = false;

/******** bomb setup ********/
int mode = SETUP;
unsigned long defusing_time = 0;
unsigned long time_elapsed_since_countdown_started = 0;
unsigned int bomb_duration_preset = 45;

/******** timers setup ********/
unsigned long bomb_arm_time = 0;
unsigned long bomb_stop_time = 0;
unsigned long next_beep_at = 0;

void resetBomb()
{
    next_beep_at = 0;
    bomb_stop_time = 0;
    bomb_arm_time = 0;
    defusing_time = 0;
    time_elapsed_since_countdown_started = 0;
}

void switchToSetupMode()
{
    resetBomb();

    // show mode
    mode = SETUP;
    messages.bomb_will_boom_in(bomb_duration_preset);
}

bool both_buttons_pressed() { return manageButtonPressAndLongPress(addTimeBtn, addTimePressStart) && manageButtonPressAndLongPress(subTime, subTimePressStart); }

void bombMode()
{
    unsigned long now = millis();

    if (both_buttons_pressed()) // defusing
    {
        if (defusing_time < now)
            defusing_time = now + 10*1e3 - (defusalKit.read() == Button::PRESSED ? 5000 : 0);

        delay(500);
        tone(SPEAKERS_PIN, 1200, 120);
        bomb_stop_time += 500;
        messages.print("**  DEFUSING  **",
                       "      "+String(round((float)now / (float)(defusing_time - now)))+"%...   ");

        if (now > defusing_time)
        {
            messages.print("**BOMB DEFUSED**",
                           "****************");
            delay(5000);
            switchToSetupMode();
        }
    }
    else if (time_elapsed_since_countdown_started > bomb_duration_preset * 1000) // boom
    {
        messages.print("****************",
                       "****************");
        for (int i = 0; i < 15; i++)
        { // beep every 90ms 1 times
            tone(SPEAKERS_PIN, 4023, 25);
            delay(65);
        }

        messages.print("*terrorists win*",
                       "****************");
        for (int i = 0; i < 5000; i++)
        { // white noise boom sound
            tone(SPEAKERS_PIN, random(60, 360), 2);
            delay(2);
        }
        switchToSetupMode();
    }
    else
    {
        // accelerating beep
        time_elapsed_since_countdown_started = now - bomb_arm_time;
        accelerating_beep((float)(time_elapsed_since_countdown_started - bomb_stop_time) / (float)(bomb_duration_preset * 1000), next_beep_at);
        defusing_time = 0;
    }
}

void setup()
{
    addTimeBtn.begin();
    subTime.begin();
    defusalKit.begin();

    lcd.begin(16, 2);

    messages.print( "Xx_Bombinator_xX",
                    "Reglez la duree");

    delay(3000);

    switchToSetupMode();
}

void setupMode() {

    delay(50);// Let time to press both buttons

    // scan for both left / right buttons statuses
    leftButtonPress = manageButtonPressAndLongPress(addTimeBtn, addTimePressStart);
    rightButtonPress = manageButtonPressAndLongPress(subTime, subTimePressStart);

    if (leftButtonPress && rightButtonPress) // valid timer, switch to bomb mode
    {
        bomb_arm_time = millis();
        mode = BOMB_STARTED;
        messages.print("***MAINTENEZ****",
                       "*POUR DESARMER**");
    }
    else if (leftButtonPress || rightButtonPress)
    {
        if (leftButtonPress)
        {
            bomb_duration_preset += 5;
            tone(SPEAKERS_PIN, 1500, 120);
        }

        if (rightButtonPress)
        {
            bomb_duration_preset -= 5;
            tone(SPEAKERS_PIN, 1000, 120);
        }

        messages.bomb_will_boom_in(bomb_duration_preset);
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
