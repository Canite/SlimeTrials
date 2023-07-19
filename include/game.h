#ifndef _GAME_H
#define _GAME_H

#include <gb/gb.h>
#include <stdio.h>

#include "input.h"
#include "physics.h"
#include "player.h"
#include "utils.h"
#include "gfx.h"

#include "../res/test.h"

// Game state
enum GameState
{
    GS_TITLE_LOAD,
    GS_TITLE,
    GS_INGAME,
    GS_PAUSE
};

struct Game
{
    uint16_t gameState;
    uint16_t gameFrame;
    char* tileMap;
    uint8_t tileMapW;
    uint8_t tileMapH;
    uint8_t mapX;
    uint8_t mapY;
    uint8_t oldMapX;
    uint8_t oldMapY;
};

extern struct Game game;

void init_game();
void game_loop();

#endif