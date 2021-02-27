#include <Arduino.h>

float beep_per_second(float time_elapsed_since_bomb_started_pu)
{
    return 1.049 * exp(0.244 * time_elapsed_since_bomb_started_pu + 1.764 * time_elapsed_since_bomb_started_pu * time_elapsed_since_bomb_started_pu);
}