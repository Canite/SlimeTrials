#ifndef _INPUT_H
#define _INPUT_H

#include <stdint.h>
#include <gb/gb.h>

#include "game.h"
#include "physics.h"
#include "player.h"
#include "utils.h"

// new and previous values of the joypad input
extern uint8_t joy;
extern uint8_t old_joy;

// check button down
#define INPUT_KEY(key) (joy&(key))
// check button press
#define INPUT_KEYPRESS(key) ((joy & ~old_joy) & (key))
// check button release
#define INPUT_KEYRELEASE(key) ((~joy & old_joy) & (key))

void process_title_input();
void process_game_input();
void process_pause_input();

#endif