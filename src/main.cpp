#include <Arduino.h>
#include <LiquidCrystal.h>
#include <Button.h>

#include "../include/constants.h"
#include "../include/buttonPresses.h"
#include "../include/beep.h"
#include "../include/messages.h"
#include "../include/bomb.h"

/******** LCD setup ********/
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
Messages messages(lcd);

/******** Buttons setup: Connect your button between pins and GND ********/
Button addTimeBtn(ADD_TIME_BUTTON_PIN, DEBOUNCE_TIME); // Add time to bomb
Button subTime(SUB_TIME_BUTTON_PIN, DEBOUNCE_TIME); // Substract time to bomb delay
Button defusalKit(DEFUSAL_KIT_PIN, DEBOUNCE_TIME);
unsigned long addTimePressStart = 0;
unsigned long subTimePressStart = 0;
bool leftButtonPress = false;
bool rightButtonPress = false;

/******** bomb setup ********/
int bombMode = SETUP;
unsigned long defuse_duration_preset = 10000;
unsigned int bomb_duration_preset = 45;

/******** timers setup ********/
unsigned long bomb_arm_time = 0;
unsigned long defusing_press_duration = 0;
float defuse_progress = 0;
unsigned long next_beep_at = 0;

/******** led setup ********/
unsigned long led_hide_at = 0;

bool both_buttons_pressed() { return manageButtonPressAndLongPress(addTimeBtn, addTimePressStart) && manageButtonPressAndLongPress(subTime, subTimePressStart); }

void resetBomb()
{
    next_beep_at = 0;
    defusing_press_duration = 0;
    defuse_progress = 0;
    bomb_arm_time = 0;
}

void readyToStartMode() {
    messages.bomb_ready_to_start(bomb_duration_preset);
    bombMode = READY_TO_START;
}

void bombReadyToStart(int &bombMode, unsigned long &arm_time)
{
    if (both_buttons_pressed()) {
        bombMode = BOMB_STARTED;
        arm_time = millis();
    }
}

/**
 * Triggered when bomb exploded or defused
 */
void bombFinished()
{
    delay(5000);// Once bomb finished, wait some time before restart
    resetBomb();
    readyToStartMode();
}

void bomb_countdown()
{
    unsigned long now = millis();
    unsigned long time_elapsed_since_countdown_started = now - bomb_arm_time;

    if ( both_buttons_pressed() ) // L+R buttons are maintained: defusing
    {
        delay(400);
        defusing_press_duration += 400;

        defuse_duration_preset = 10000 - (defusalKit.read() == Button::PRESSED ? 5000 : 0);
        defuse_progress = (float)defusing_press_duration / (float)(defuse_duration_preset);
        tone(SPEAKERS_PIN, (unsigned int)(1200 + 1000 * defuse_progress), 120);
        messages.print("DESAMORCAGE ... ",
                       messages.fill('>', (int)round(16 * defuse_progress)));

        if ( defuse_progress >= 1 )
        {
            messages.print("BOMBE DESAMORCEE",
                           "****************");
            delay(5000);

            bombFinished();
        }
    }
    else if (time_elapsed_since_countdown_started > bomb_duration_preset * 1000) // temps écoulé : boom
    {
        messages.print("****************",
                       "****************");

        // bidibidibi (just before)
        for (int i = 0; i < 15; i++)
        { // beep every 90ms 1 times
            tone(SPEAKERS_PIN, 4023, 25);
            delay(65);
        }

        // brrrrrrrr sound
        messages.print("*terrorists win*",
                       "****************");
        for (int i = 0; i < 5000; i++)
        { // white noise boom sound
            tone(SPEAKERS_PIN, random(60, 360), 2);
            delay(2);
        }

        bombFinished();
    } else { // countdown just going down, nothing else, we display it !
        defusing_press_duration = 0;
        messages.print("**Bombe amorcee*",
                        "  restant: " + String((int)round((bomb_duration_preset * 1000 - time_elapsed_since_countdown_started) / 1000)) + "s");
    }
    
    // accelerating beep
    if ( now > next_beep_at ) {
        float time_elapsed_since_bomb_started_pu = (float)time_elapsed_since_countdown_started / (float)(bomb_duration_preset * 1000);
        
        bomb_beep();

        digitalWrite(LED_PIN, HIGH);
        led_hide_at = now + 100;

        next_beep_at = now + 1000 * (1 / beep_per_second(time_elapsed_since_bomb_started_pu));
    }
    // blinks led if necessary
    if ( now > led_hide_at )
        digitalWrite(LED_PIN, LOW);
}

void setupMode() {

    delay(50);// Let time to press both buttons

    // scan for both left / right buttons statuses
    leftButtonPress = manageButtonPressAndLongPress(addTimeBtn, addTimePressStart);
    rightButtonPress = manageButtonPressAndLongPress(subTime, subTimePressStart);

    if (leftButtonPress && rightButtonPress) // valid timer, switch to bomb mode
    {
        readyToStartMode();
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

        messages.bomb_setup_time(bomb_duration_preset);
    }
}

void setup()
{
    addTimeBtn.begin();
    subTime.begin();
    defusalKit.begin();

    pinMode(LED_PIN, OUTPUT);

    lcd.begin(16, 2);

    messages.print("Xx_Bombinator_xX",
                    "Reglez la duree");

    delay(3000);

    bombMode = SETUP;
}

void loop() {

    switch (bombMode)
    {
        case SETUP:
            // ONLY AT STARTUP: Game Master chooses bomb duration
            setupMode();
            break;

        case READY_TO_START:
            // wait for terrorists to start the bomb (pressing L+R)
            bombReadyToStart(bombMode, bomb_arm_time);
            break;

        case BOMB_STARTED:
            // accelerating beep, possibility to stop holding side butts
            bomb_countdown();
            break;
    }
}
