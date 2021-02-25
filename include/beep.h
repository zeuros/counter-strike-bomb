
#define SPEAKERS_PIN 9 //  that's mozzi's default output
#define BOMB_BEEP_DURATION 125

void accelerating_beep(float time_passed_since_bomb_armed_pu, unsigned long &next_beep_at);

float beep_per_second(float time_passed_since_bomb_armed_pu);

void defuse_beep(int every_ms);