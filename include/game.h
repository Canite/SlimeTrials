#ifndef _GAME_H
#define _GAME_H

#include <gb/gb.h>
#include <stdio.h>

#include "input.h"
#include "physics.h"
#include "player.h"
#include "utils.h"
#include "gfx.h"

// Game state
enum GameState
{
    GS_TITLE_LOAD,
    GS_TITLE,
    GS_INGAME,
    GS_PAUSE
};

extern uint16_t nGameState;

void game_loop();

#endif