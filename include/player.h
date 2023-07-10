#ifndef _PLAYER_H
#define _PLAYER_H

#include <gb/gb.h>
#include <gbdk/platform.h>
#include <gbdk/console.h>
#include <stdint.h>

#include "utils.h"

struct Player
{
    int16_t x;
    int16_t y;
    uint8_t w;
    uint8_t h;
    int16_t xSpd;
    int16_t ySpd;
    uint8_t facing;
    uint8_t rot;
    uint8_t animFrame;
    uint8_t grounded;
};

extern struct Player player;

void init_player();
void handle_input();

#endif